
#ifndef _DO_STRING_I_
#define _DO_STRING_I_
#include "manuse.h"

typedef struct HSTX {
	hs_database_t *db;
	hs_scratch_t *scratch;
}ICotx;

int HyperStringInit(ICotx *icnox);
int HyperStringDeinit(ICotx *icnox);
#endif