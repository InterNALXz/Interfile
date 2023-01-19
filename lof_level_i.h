#include <stdarg.h>

enum {
LOG_LESS,
LOG_DEBUG,
LOG_NOTICE,
LOG_INFO,
LOG_WARNING,
LOG_ERROR,
};

int ILOGOUT(int l, const char *file, const int line, 
	const char *func, const char *fmt, ...);

#define ILOGDEBUG(...) ILOGOUT(LOG_DEBUG, __FILE__, \
	__LINE__, __FUNCTION__, __VA_ARGS__)
