//
//  semaphore.c
//  semctl
//
//  Created by Théo MAILLART on 23/10/2016.
//  Copyright © 2016 Théo MAILLART. All rights reserved.
//

#include "semaphore.h"

void get_semid(const char* name,int n_sem){
    semid=semget(ftok(name, 'a'), n_sem, PROT);
}

int init_semaphore(const char* name,int n_sem){
    return semid=semget(ftok(name, 'a'),n_sem,IPC_CREAT|PROT|IPC_EXCL);//IPC_EXCL
}

int detruire_semaphore(int n_sem){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Le groupe de sémaphores ne peut être détruit, il n'a jamais été créé.\n");
        return -1;
    }
    return semctl(semid,n_sem,IPC_RMID,0);
}

int val_sem(int sem, int val){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Le groupe de sémaphores ne peut être détruit, il n'a jamais été créé.\n");
        return -1;
    }
    if (sem<0) return -2;
    
    union semun bla;
    bla.val = val;
    return semctl(semid,sem,SETVAL,bla);
}

int P(int sem){
    if (semid == -2 || semid == -1) {
        fprintf(stderr, "Le groupe de sémaphores ne peut être détruit, il n'a jamais été créé.\n");
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
        fprintf(stderr, "Le groupe de sémaphores ne peut être détruit, il n'a jamais été créé.\n");
        return -1;
    }
    if (sem<0) return -2;
    struct sembuf buffer;
    buffer.sem_num=sem;
    buffer.sem_op=1;
    buffer.sem_flg=0; //a verifier les flags -> blocage avec ipcnowait
    return semop(semid,&buffer,1);
}
