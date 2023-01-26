#include "file_i.h"
#include "org_function_i.h"

#define MF_MAX 10

static modfun mf_i[] = {0};

static int mf_count = 0;

// TODO: register also need unregister
int RegisterIFun(const char *name, void *(*fun)(void *), int flag) {
    if (name == NULL || fun == NULL) {
        ILOGDERR(ERROR_WRONG_ARG, "register inter function error");
        return -1;
    }

    modfun *tl = &mf_i[mf_count];

    tl->name = name;
    tl->funct = fun;
    tl->flag = flag;
    ILOGDEBUG("register name is %s, function is %p, flag is %d", tl->name, tl->funct, tl->flag);
    
    return (mf_count++);
}

modfun *GetIFuncByName(const char *name) {
    int i = 0;
    modfun *tl = NULL;

    for(; i < mf_count; i++) {
        tl = &mf_i[i];
        ILOGDEBUG("get name %s", tl->name);

        if (tl->name == NULL)
            continue;

        if (strcmp(tl->name, name) == 0)
            return tl;
    }

    return NULL;
}
