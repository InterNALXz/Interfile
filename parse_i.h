#ifndef _PARSE_H_
#define _PARSE_H_
#include "manuse.h"

typedef struct YamlNode_ {
    char *name;
    char *val;

    struct YamlNode_ *parent;
    struct YamlNode_ *head;
    struct YamlNode_ *next;
}__attribute__((aligned(sizeof(long)))) YamlNode;

int parse_args(const char **argv, int argc, infile *mian_i);
void YamlFileNodeInit(void);
void YamlFileNodeDeInit(void);
int parse_yaml_file(const char *name);
#endif

