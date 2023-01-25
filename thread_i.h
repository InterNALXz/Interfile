#ifndef _THREAD_I_
#define _THREAD_I_
#include "pthread.h"

typedef struct iThreadM_ {
    struct SloThi_* sli;
    pthread_t pth;
    struct iThreadM_* next;
}iThreadM;

typedef struct SloThi_ {
    struct SloThi_* next;
    int (*SFunc)(iThreadM *, void *);
    int id;
}SloTh_i;

typedef struct ThrFucN_ {
    struct ThrFucN_* next;
    const char *name;
}ThrFucN;

#define EXPEND_PTHREAD_SIZE 1<<0

iThreadM *IMThreadCreate(const char *name, int flag);
void IMThreadDestroyAll(iThreadM *head);

int file_thread(int port);
#endif
