#include "manuse.h"
#include "log_level_i.h"
#include "do_pcre_match_i.h"
#include "pcre2.h"

#define PCRE_PARSE_FUNCTION "^\\s*[A-z0-9]+\\s+[A-z0-9]*\\s*[A-z0-9]*$"
static pcre2_code *parse_regex = NULL;
pcre2_match_context *context = NULL;
static pcre2_match_data *parse_regex_study = NULL;

void so_pcre_init(void) {
    int ab;
    PCRE2_SIZE ac;
    int opt = 0;

    parse_regex = pcre2_compile((PCRE2_SPTR8)PCRE_PARSE_FUNCTION, PCRE2_ZERO_TERMINATED, opt, &ab, &ac, NULL);
    if (parse_regex == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "pcre study error!");
        return;
    }

    parse_regex_study = pcre2_match_data_create_from_pattern(parse_regex, NULL);
    if (parse_regex_study == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "pcre error!");
        return;
    }
// todo:if we want to free more pcre,we could add it in list
    return;
}

void so_pcre_deinit(void) {
    if (parse_regex)
        pcre2_code_free(parse_regex);

    if (parse_regex_study)
        pcre2_match_data_free(parse_regex_study);

    return;
}