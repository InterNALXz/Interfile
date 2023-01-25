#ifndef _ORG_FUNCTION_H_
#define _ORG_FUNCTION_H_

typedef struct modfun_ {
    const char *name;
    void *(*funct)(void *);
    int flag;
}modfun;

int RegisterIFun(const char *name, void *(*fun)(void *), int flag);
modfun *GetIFuncByName(const char *name);

#endif