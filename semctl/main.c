//
//  main.c
//  semctl
//
//  Created by Théo MAILLART on 23/10/2016.
//  Copyright © 2016 Théo MAILLART. All rights reserved.
//

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include "semaphore.h"

int main(int argc, const char * argv[]) {

    
    int i,val;
    char file[256]="/usr/local/share/semctl/";
    extern int errno;

    
    val=semget(ftok("/usr/local/share/semctl/voiture", 'a'), 1, 0); // big probleme here
    return val;

    i=mkdir("/usr/local/share/semctl",0777);
    /*
    if (errno==13) {
        printf("Permission denied\n");
        return -1;
    }*/
    
    
    if (strcmp(argv[1], "create")==0) {
        i=1;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        
        creat(strcat(file, argv[2]),0666);
        if(init_semaphore(strcat(file, argv[2]), i)<0){
            printf("Semaphore %s already exists\n",argv[2]);
            return -1;
        }
            
    }else if (strcmp(argv[1], "delete")==0) {
        i=1;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        
        get_semid(strcat(file, argv[2]), i);
        detruire_semaphore(i);
        remove(strcat(file, argv[2]));
    }else if (strcmp(argv[1], "P")==0) {
        i=1;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        
        get_semid(strcat(file, argv[2]), i);
        P(i);
    }else if (strcmp(argv[1], "V")==0){
        i=1;
        if (argc<3)
            return -1;
        if (argc==4)
            i=(int) strtol(argv[3], NULL, 10);
        
        get_semid(strcat(file, argv[2]), i);
        V(i);
    }else if (strcmp(argv[1], "init")==0){
        i=1;
        if (argc<4)
            return -1;
        if (argc==5)
            i=(int) strtol(argv[4], NULL, 10);
        
        val=(int) strtol(argv[3], NULL, 10);
        
        get_semid(strcat(file, argv[2]), i);
        val_sem(i, val);
    }
    
    
    
    
    
    return 0;
}
