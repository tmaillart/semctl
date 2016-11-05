//
//  main.c
//  semctl
//
//  Created by Théo MAILLART on 23/10/2016.
//

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/sysctl.h>
#include <dirent.h>
#include "semaphore.h"
#define PATH "/usr/local/share/semctl"

void garbage_collector(){

    int value[2];
    char fullpath[512];
    struct timeval uptime;
    size_t len;
    uint64_t bootime;
    DIR *d;
    struct dirent *dir;
    struct stat st;

    value[0]=CTL_KERN;
    value[1]=KERN_BOOTTIME;
    
    len=sizeof(uptime);
    sysctl(value, 2, &uptime, &len, NULL, 0);
    bootime = (uptime.tv_sec * (uint64_t)1000) + (uptime.tv_usec / 1000);

    
    d = opendir(PATH);
    if (d){
        while ((dir = readdir(d)) != NULL){
            if (dir->d_type==DT_REG){
                sprintf(fullpath,"%s/", PATH);
                strcat(fullpath, dir->d_name);
                
                if (stat(fullpath, &st))
                    perror(fullpath);
                else if (bootime>(st.st_birthtimespec.tv_sec*1000))
                    remove(fullpath);
                
            }
        }
        closedir(d);
    }
    

}



int main(int argc, const char * argv[]) {
    //Get information about semaphore behaviour after reboot
    
    
    if (argc<2){
        printf("You to specify a sub command\n");
        return -1;
    }
    
    int i,val,fd;
    char file[256]=PATH;
    extern int errno;
    
    i=mkdir(file,0777);
    /*
    if (errno==13) {
        printf("Permission denied\n");
        return -1;
    }*/
    garbage_collector();    
    
    if (strcmp(argv[1], "create")==0) {//semctl create <name> -<n>
        i=1;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        if (i<1){
            printf("You have to specify a positive number of semaphore\n");
            return -1;
        }
        strcat(file, argv[2]);
        
        if(access(file, F_OK )!= -1) {
            // file exists
            printf("Semaphore %s already exists\n",argv[2]);
            return -1;
        } else {
            // file doesn't exist
            fd=open(file,0666|O_CREAT);
            if (fd<0)
                return -1;
            
            write(fd, &i, sizeof(int));
            if(init_semaphore(file, i)<0){
                printf("Semaphore system error\n");
                close(fd);
                return -1;
            }
            close(fd);
        }
        
            
    }else if (strcmp(argv[1], "delete")==0) {//semctl delete <name>
        if (argc<3)
            return -1;
        strcat(file, argv[2]);
        get_semid(file);
        remove_semaphore();
        remove(file);
    }else if (strcmp(argv[1], "P")==0) {//semctl P <name> -<nth>
        i=0;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        if (i<0){
            printf("You have to specify a positive number of semaphore\n");
            return -1;
        }
        
        get_semid(strcat(file, argv[2]));
        P(i);
    }else if (strcmp(argv[1], "V")==0){//semctl V <name> -<nth>
        i=0;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        if (i<0){
            printf("You have to specify a positive number of semaphore\n");
            return -1;
        }
        
        get_semid(strcat(file, argv[2]));
        V(i);
    }else if (strcmp(argv[1], "init")==0){//semctl init <name> <val> -<nth>
        i=0;
        if (argc<4)
            return -1;
        if (argc==5)
            i=(int) strtol(argv[4], NULL, 10);
        if (i<0){
            printf("You have to specify a positive number of semaphore\n");
            return -1;
        }
        
        val=(int) strtol(argv[3], NULL, 10);
        
        get_semid(strcat(file, argv[2]));
        val_sem(i, val);
    }
    
    
    
    
    
    return 0;
}
