//
//  semaphore.h
//  semctl
//
//  Created by Théo MAILLART on 23/10/2016.
//

#ifndef semaphore_h
#define semaphore_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>

#define PROT 0666

union semun {
    int              val;    /* Valeur pour SETVAL */
    struct semid_ds *buf;    /* Tampon pour IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Tableau pour GETALL, SETALL */
    struct seminfo  *__buf;  /* Tampon pour IPC_INFO
                                (spécifique à Linux) */
};

static int semid=-2;
static int n_sem=0;

int init_semaphore(const char* name,int n_sems);

void get_semid(const char* name);

int remove_semaphore();

int val_sem(int sem, int val);

int P(int sem);

int V(int sem);


#endif /* semaphore_h */
