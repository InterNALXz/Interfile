#include "parse_i.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include "log_level_i.h"
#include "unit_test_i.h"

#define MAX_ARGS_LENTH 8

int parse_args(const char **argv, int argc) {
	if (argc == 1) {
		ILOGDERR(ERROR_PARSE_OPT, "job done,please add option");
		return -1;
	}

	ILOGDEBUG("argc is %d", argc);
	
	int ret = 0;
	int offset = 1;
	
	const char *opt_args = *(argv + offset);

	int arg_count = argc - 1;
	char args_t[MAX_ARGS_LENTH] = {0};

	do {
		if (opt_args[0] != '-') {
			ILOGDERR(ERROR_PARSE_OPT, "something going wrong!Not right arg format");
			return -1;
		}
		strncpy(args_t, opt_args, MAX_ARGS_LENTH);
		ILOGDEBUG("args is %s", args_t);

		switch (args_t[1]) {
			case 'h':
				ILOGDEBUG("args is -o for file output");
				ret = 0;
				arg_count = 0;
				break;
			case 'l': {
				if (arg_count < 2) {
					ILOGDERR(ERROR_OPT_USAGE, "we get wrong useage");
					return -1;
				}

				offset++;
				opt_args = *(argv + offset);

				if (!isdigit((int)(*opt_args))) {
					ILOGDERR(ERROR_PARSE_OPT, "something going wrong!Not right arg format");
					return -1;
				}

				int ll = atoi(opt_args);
				ChangeLogLevel(ll);
				ILOGDEBUG("change loglevel success");
			}
				arg_count--;
				break;
			case 'o':
				if (arg_count < 2) {
					ILOGDERR(ERROR_OPT_USAGE, "we get wrong useage");
					return -1;
				}
				offset++;
				arg_count--;
				break;
			case 't':
#ifdef UNIT_TEST_I
				unit_test_all_i();
#endif
				return -1;
				break;
			default:
				ret = -1;
				break;
		}
		offset++;
		opt_args = *(argv + offset);
		arg_count--;
	} while(arg_count > 0);

	return 0;
	
}

