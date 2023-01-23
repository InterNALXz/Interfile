#include "log_level_i.h"
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include "manuse.h"

#define MAX_MSG_LENTH 2048
#define MAX_FILE_PATH_LENTH 512


static int g_log_level = 3;

type_i log_level_map[] = {
	{0, NULL, 0},
	{1, "DEBUG", 0},
	{2, "INFO", 0},
	{3, "NOTICE", 0},
	{4, "WARNING", 0},
	{5, "ERROR", 0},
	{6, NULL, 0},
};

void ChangeLogLevel(int ll) {
	if (ll >= LOG_MAX) 
		return;

	g_log_level = ll;
}

int ILOGOUT(int l, const char *file, const int line, 
	const char *func, const char *err_str, const char *fmt, ...) {
	if (file == NULL || func == NULL || fmt == NULL)
		return -1;
	
	if (g_log_level > l)
		return 0;

	int lenth_d = 0;
	char *file_path = NULL;

	if ((lenth_d = Get_Cpath_Lenth()) == 0)
		file_path = "main.c";
	else
		file_path = strdup(file);
	
	char *file_name = &(file_path[lenth_d + 1]);

	char msg_i[MAX_MSG_LENTH] = {0};

	va_list pi;
	va_start(pi, fmt);
	vsnprintf(msg_i, MAX_MSG_LENTH, fmt, pi);
	va_end(pi);

	if (LOG_ERROR == l) {
		fprintf(stderr, "\033[0;32;31m""Error""\033[m"":[""\033[0;35m""%s""\033[m""] In %s =%s= ---> %d <%s>\n", err_str, file_name, func, line, msg_i);
		goto i_status;
	}

	if (err_str != NULL)
		fprintf(stderr, "\033[0;32;31m""Error""\033[m"":[""\033[0;35m""%s""\033[m""] In %s =%s= ---> %d <%s>\n", err_str, file_name, func, line, msg_i);
	else
		fprintf(stdout, "Log [""\033[0;35m""%s""\033[m""] In %s =%s= ---> %d : <%s>\n", log_level_map[l].name, file_name, func, line, msg_i);

i_status:
	if (file_path != NULL)
		free(file_path);

	return 0;
}

