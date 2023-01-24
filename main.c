/* just do job */
#include "file_i.h"
#include "manuse.h"
#include "do_pcre_match_i.h"
#include "do_string_i.h"

#define MAX_PATH 512

static char current_path[MAX_PATH] = {0};

static int Get_Cpath(void) {
	getcwd(current_path, sizeof(current_path));

	ILOGDEBUG("path is %s", current_path);
}

int Get_Cpath_Lenth(void) {
	return strlen(current_path);
}

int main(int argc, const char *argv[]) {
	Get_Cpath();

	int ret = parse_args(argv, argc);
	if (ret < 0)
		return ret;
	
	so_pcre_init();

	ICotx *icx = calloc(1, sizeof(ICotx));

	HyperStringInit(icx);

	file_thread(0);
	HyperStringDeinit(icx);

	free(icx);
	so_pcre_deinit();
    return 0;
}
