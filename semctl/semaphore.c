//
//  semaphore.c
//  semctl
//
//  Created by Théo MAILLART on 23/10/2016.
//

#include "semaphore.h"

void get_semid(const char* name){
    int fd=open(name,O_RDONLY);
    if (fd<0) {
        printf("Semaphore doesn't exist\n");
        exit(-1);
    }
    read(fd, &n_sem, sizeof(int));
    semid=semget(ftok(name, 'a'), n_sem, PROT);
    close(fd);
}

int init_semaphore(const char* name,int n_sems){
    return semid=semget(ftok(name, 'a'),n_sems,IPC_CREAT|PROT|IPC_EXCL);//IPC_EXCL
}

int remove_semaphore(){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Semaphore internal error\n");
        return -1;
    }
    return semctl(semid,n_sem,IPC_RMID,0);
}

int val_sem(int sem, int val){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Semaphore internal error\n");
        return -1;
    }
    if (sem<0) return -2;
    
    union semun bla;
    bla.val = val;
    return semctl(semid,sem,SETVAL,bla);
}

int P(int sem){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Semaphore internal error\n");
        return -1;
    }
    if (sem<0) return -2;
    struct sembuf buffer;
    buffer.sem_num=sem;
    buffer.sem_op=-1;
    buffer.sem_flg=0; //a verifier les flags -> blocage avec ipcnowait
    return semop(semid,&buffer,1);
}

int V(int sem){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Semaphore internal error\n");
        return -1;
    }
    if (sem<0) return -2;
    struct sembuf buffer;
    buffer.sem_num=sem;
    buffer.sem_op=1;
    buffer.sem_flg=0; //a verifier les flags -> blocage avec ipcnowait
    return semop(semid,&buffer,1);
}
