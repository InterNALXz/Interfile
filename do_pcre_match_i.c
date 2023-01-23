#include "pcre.h"
#include "manuse.h"
#include "log_level_i.h"
#include "do_pcre_match_i.h"

#define PCRE_PARSE_FUNCTION "^\\s*[A-z0-9]+\\s+[A-z0-9]*\\s*[A-z0-9]*$"
static pcre *parse_regex = NULL;
static pcre_extra *parse_regex_study = NULL;

void so_pcre_init(void) {
    const char *ab;
    int ac;
    int opt = 0;

    parse_regex = pcre_compile(PCRE_PARSE_FUNCTION, opt, &ab, &ac, NULL);
    if (parse_regex == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "pcre study error!");
        return;
    }

    parse_regex_study = pcre_study(parse_regex, 0, &ab);
    if (parse_regex_study == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "pcre error!");
        return;
    }
// todo:if we want to free more pcre,we could add it in list
    return;
}

void so_pcre_deinit(void) {
    if (parse_regex)
        pcre_free(parse_regex);

    if (parse_regex_study)
        pcre_free_study(parse_regex_study);

    return;
}