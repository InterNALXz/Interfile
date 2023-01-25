
#ifndef _DO_STRING_I_
#define _DO_STRING_I_
#include "manuse.h"

typedef struct HSTX {
	hs_database_t *db;
	hs_scratch_t *scratch;
}ICotx;

#define STRING_HYPER_NOCASE 1<<0

int HyperStringInit(ICotx *icnox);
int HyperStringDeinit(ICotx *icnox);
#endif