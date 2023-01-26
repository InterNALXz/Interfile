#include "file_i.h"
#include "thread_i.h"
#include "org_function_i.h"

int file_thread(int port) {
	switch (port) {
		case 0:
		case 1:
		default:
			break;
	
	}

	return 0;
}

static int expend_thread_size = 115200;

// TODO: other inti function list
ThrFucN *ThreadFuncInit(void) {
	ThrFucN *th = calloc(1, sizeof(ThrFucN));
	if (th == NULL) {
		ILOGDERR(ERROR_ALLOC_ERR, "map thread function mem error");
		return NULL;
	}

	th->name = NULL;
	th->next = NULL;
	return th;
}

int ThreadFunAdd(ThrFucN *head) {
	for(ThrFucN *tmp = head; tmp != NULL; tmp = tmp->next) {}

	return 0;
}
// end TODO

iThreadM *IMThreadCreate(const char *name, int flag) {
	if (name == NULL)
		return NULL;

	modfun *ifu = GetIFuncByName(name);
	if (ifu == NULL) {
		ILOGDERR(ERROR_THREAD, "get finction name %s error", name);
		return NULL;
	}

	iThreadM *itm = calloc(1, sizeof(iThreadM));

	pthread_attr_t pat;

	pthread_attr_init(&pat);

	pthread_attr_setdetachstate(&pat, PTHREAD_CREATE_JOINABLE);

	if(flag & EXPEND_PTHREAD_SIZE) {
		if (pthread_attr_setstacksize(&pat, (size_t)expend_thread_size)) {
			ILOGDERR(ERROR_THREAD, "set thread size error");
		}
	}

	int jt = pthread_create(&itm->pth, &pat, ifu->funct, (void *)itm);
	if (jt) {
		ILOGDERR(ERROR_THREAD, "thread create error");
	}

	return itm;
}

void IMThreadDestroyAll(iThreadM *head) {
	iThreadM *tf;

	for(iThreadM *tmp = head; tmp != NULL; ) {
		tf = tmp;
		pthread_join(tf->pth, NULL);
		tmp = tmp->next;
		free(tf);
	}
}

#ifdef UNIT_TEST_I
static void *demo01(void *arg) {
	ILOGNOTICE("demo 01 ready");
	return NULL;
}

void IMThreadTest01(void) {
	int l = RegisterIFun("demo 01", demo01, 0);
	ILOGDEBUG("add index %d in mf_i", l);
	iThreadM *t = IMThreadCreate("demo 01", 0);
	IMThreadDestroyAll(t);
}
#endif