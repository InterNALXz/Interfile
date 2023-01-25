#ifndef _LOG_LEVEL_I_
#define _LOG_LEVEL_I_
#include "stdarg.h"

enum {
LOG_LESS,
LOG_DEBUG,
LOG_INFO,
LOG_NOTICE,
LOG_WARNING,
LOG_ERROR,
LOG_MAX,
};

enum {
ERROR_NONE,
ERROR_OPT_USAGE,
ERROR_ALLOC_ERR,
ERROR_PARSE_OPT,
ERROR_PCRE,
ERROR_BUILD_MUT,
ERROR_WRONG_ARG,
ERROR_HS_GET,
ERROR_THREAD,
ERROR_NORMAL,
ERROR_MAX,
};

int ILOGOUT(int l, const char *file, const int line, 
	const char *func, const char *err_str, const char *fmt, ...);

void ChangeLogLevel(int ll);

#define ILOGDEBUG(...) ILOGOUT(LOG_DEBUG, __FILE__, \
	__LINE__, __FUNCTION__, NULL, __VA_ARGS__)

#define ILOGINFO(...) ILOGOUT(LOG_INFO, __FILE__, \
	__LINE__, __FUNCTION__, NULL, __VA_ARGS__)

#define ILOGNOTICE(...) ILOGOUT(LOG_NOTICE, __FILE__, \
	__LINE__, __FUNCTION__, NULL, __VA_ARGS__)

#define ILOGWARNING(ecode, ...) ILOGOUT(LOG_WARNING, __FILE__, \
	__LINE__, __FUNCTION__, #ecode, __VA_ARGS__)

#define ILOGDERR(ecode, ...) ILOGOUT(LOG_ERROR, __FILE__, \
	__LINE__, __FUNCTION__, #ecode, __VA_ARGS__)
#endif
