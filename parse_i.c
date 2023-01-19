#include "parse_i.h"
#include "stdio.h"
#include "string.h"
#include "log_level_i.h"

#define MAX_ARGS_LENTH 8

int parse_args(const char **argv, int argc) {
	if (argc == 1) {
		printf("job done,please add option\n");
		return -1;
	}

	ILOGDEBUG("argc is %d", argc);
	
	int ret = 0;
	
	const char *opt_args = *(argv+1);

	int arg_count = argc - 1;
	char args_t[MAX_ARGS_LENTH] = {0};

	do {
		if (opt_args[0] != '-') {
			printf("something going wrong!Not right arg format\n");
			return -1;
		}
		strncpy(args_t, opt_args, MAX_ARGS_LENTH);
		ILOGDEBUG("args is %s", args_t);

		switch (args_t[1]) {
			case 'h':
				printf("args is -o for file output\n");
				ret = 0;
				arg_count = 0;
				break;
			case 'o':
				if (arg_count < 2) {
					ILOGDERR(ERROR_OPT_USAGE, "we get wrong useage");
					return -1;
				}
				arg_count--;
				opt_args += 2;
				break;
			default:
				ret = -1;
				break;
		}

		
		arg_count--;
	} while(arg_count > 0);

	return 0;
	
}

