#include "parse_i.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include "log_level_i.h"
#include "unit_test_i.h"
#include <yaml.h>
#include <sys/stat.h>

#define MAX_ARGS_LENTH 8

int parse_args(const char **argv, int argc, infile *mian_i) {
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
			case 'c': {
				if (arg_count < 2) {
					ILOGDERR(ERROR_OPT_USAGE, "we get wrong yaml configure");
					return -1;
				}

				offset++;
				opt_args = *(argv + offset);

				mian_i->conf_name = opt_args;
				ILOGDEBUG("conf name is %s", mian_i->conf_name);
			}

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

	return ret;
	
}

static YamlNode* i_root = NULL;

void YamlFileNodeInit(void) {
	i_root = calloc(1, sizeof(YamlNode));
	if (i_root == NULL) {
		ILOGDERR(ERROR_ALLOC_ERR, "cannot alloc node mem");
	}
}

void YamlFileNodeDeInit(void) {
	I_FREE(i_root);
}

static int deal_node_parse(yaml_parser_t *ypar, YamlNode *i_root) {
	yaml_event_t yeve = {0};

	int over = 0;

	while(!over) {
		if (!yaml_parser_parse(ypar, &yeve)) {
			ILOGDERR(ERROR_ALLOC_ERR, "parse yaml error");
			return -1;
		}
		ILOGDEBUG("event type is %d", yeve.type);

		switch (yeve.type) {
			case YAML_STREAM_START_EVENT:
				break;
			case YAML_STREAM_END_EVENT:
				over = 1;
				break;
			case YAML_DOCUMENT_START_EVENT:
				goto next;
			case YAML_DOCUMENT_END_EVENT:
				over = 1;
				break;
			case YAML_ALIAS_EVENT:
				break;
			case YAML_SCALAR_EVENT: {
				char *value = (char *)yeve.data.scalar.value;
				char *tagg = (char *)yeve.data.scalar.tag;
				ILOGDEBUG("value is %s, tag is %s", value, tagg);
			}
				goto next;
			
			case YAML_SEQUENCE_START_EVENT: 
				if (deal_node_parse(ypar, i_root) != 0) {
					return -1;
				}
				goto next;
			
			case YAML_SEQUENCE_END_EVENT:
				over = 1;
				goto next;
			case YAML_MAPPING_START_EVENT: 
				if (deal_node_parse(ypar, i_root) != 0) {
					return -1;
				}
				goto next;
			
			case YAML_MAPPING_END_EVENT:
				over = 1;
				goto next;
			default:
				break;

			next:
				yaml_event_delete(&yeve);
				break;

			fail:
				yaml_event_delete(&yeve);
				over = 1;
				break;
		}
	}

	return 0;
}

int parse_yaml_file(const char *name) {
	if (i_root == NULL)	{
		ILOGDERR(ERROR_NORMAL, "do not init node or got something wrong");
		return -1;
	}

	FILE *yfile = NULL;
	yaml_parser_t ypar;

	if (yaml_parser_initialize(&ypar) != 1) {
		ILOGDERR(ERROR_NORMAL, "init yaml parser error");
		return -1;
	}

	struct stat stat_ico;
	if (stat(name, &stat_ico) == 0) {
		if (stat_ico.st_mode & __S_IFDIR) {
			ILOGDERR(ERROR_NORMAL, "not a file, just a dir");
			goto error;
		}
	}

	yfile = fopen(name, "r");
	if (yfile == NULL) {
		ILOGDERR(ERROR_NORMAL, "open file failure");
		goto error;
	}

	yaml_parser_set_input_file(&ypar, yfile);

	deal_node_parse(&ypar, i_root);
	yaml_parser_delete(&ypar);
	fclose(yfile);
	return 0;

error:
	yaml_parser_delete(&ypar);
	return -1;

}

