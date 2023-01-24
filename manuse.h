#ifndef _MANUSE_H_
#define _MANUSE_H_
#include "hs.h"
#include "magic.h"


typedef struct type_s_i {
	int level;
	const char *name;
	unsigned long long policy; 
}type_i;

#define I_FREE(a) if ((a)) \
	free((a));

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

int Get_Cpath_Lenth(void);
#endif

