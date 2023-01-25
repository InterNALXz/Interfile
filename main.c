/* just do job */
#include "file_i.h"
#include "manuse.h"
#include "do_pcre_match_i.h"
#include "do_string_i.h"
#include "thread_i.h"

#define MAX_PATH 512

static char current_path[MAX_PATH] = {0};

static void Get_Cpath(void) {
	getcwd(current_path, sizeof(current_path));

	ILOGDEBUG("path is %s", current_path);
}

int Get_Cpath_Lenth(void) {
	return strlen(current_path);
}

int main(int argc, const char *argv[]) {
	Get_Cpath();

	infile inf_main = {0};

	int ret = parse_args(argv, argc, &inf_main);
	if (ret < 0)
		return ret;

	YamlFileNodeInit();
	parse_yaml_file(inf_main.conf_name);
	
	so_pcre_init();

	iThreadM *main_ti = IMThreadCreate("control terminal", 0);
	if (main_ti == NULL) {
		ILOGDERR(ERROR_THREAD, "control terminal not start");
	}

	ICotx *icx = calloc(1, sizeof(ICotx));

	HyperStringInit(icx);

	file_thread(0);
	IMThreadDestroyAll(main_ti);
	HyperStringDeinit(icx);

	free(icx);
	so_pcre_deinit();
	YamlFileNodeDeInit();
    return 0;
}
