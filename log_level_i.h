#include "stdarg.h"

enum {
LOG_LESS,
LOG_DEBUG,
LOG_INFO,
LOG_NOTICE,
LOG_WARNING,
LOG_ERROR,
};

enum {
ERROR_NONE,
ERROR_OPT_USAGE,
ERROR_ALLOC_ERR,
ERROR_MAX,
};

int ILOGOUT(int l, const char *file, const int line, 
	const char *func, const char *err_str, const char *fmt, ...);

#define ILOGDEBUG(...) ILOGOUT(LOG_DEBUG, __FILE__, \
	__LINE__, __FUNCTION__, NULL, __VA_ARGS__)

#define ILOGDERR(ecode, ...) ILOGOUT(LOG_ERROR, __FILE__, \
	__LINE__, __FUNCTION__, #ecode, __VA_ARGS__)
