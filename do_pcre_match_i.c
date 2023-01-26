#include "manuse.h"
#include "log_level_i.h"
#include "do_pcre_match_i.h"
#include "pcre2.h"
#include "string.h"
#include "unit_test_i.h"

#define PCRE_PARSE_FUNCTION "^\\s*([A-z0-9]+)\\s+([A-z0-9]+)\\s*([A-z0-9]*)\\s*([A-z0-9]*)$"
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
// TODO:if we want to free more pcre,we could add it in list
    return;
}

void so_pcre_deinit(void) {
    if (parse_regex)
        pcre2_code_free(parse_regex);

    if (parse_regex_study)
        pcre2_match_data_free(parse_regex_study);

    if (context) {
        pcre2_match_context_free(context);
    }

    return;
}

#ifdef UNIT_TEST_I
void unit_pcre_match01(void) {
    char *a = "1 0 EXCEL 5";
    size_t pcre2_len;
    unsigned char *bytes_str = NULL;
    so_pcre_init();

    int pcre_rc = pcre2_match(parse_regex, (PCRE2_SPTR8)a, strlen(a), 0, 0,
            parse_regex_study, NULL);

    ILOGDEBUG("n is %d", pcre_rc);

    if (pcre_rc >= 4) {
        if (pcre2_substring_get_bynumber(
                    parse_regex_study, 1, (PCRE2_UCHAR8 **)&bytes_str, &pcre2_len) == 0) {
            ILOGNOTICE("bytes is %s", bytes_str);
            pcre2_substring_free(bytes_str);
        } else 
            return;

        if (pcre2_substring_get_bynumber(
                    parse_regex_study, 2, (PCRE2_UCHAR8 **)&bytes_str, &pcre2_len) == 0) {
            ILOGNOTICE("bytes is %s", bytes_str);
            pcre2_substring_free(bytes_str);
        } else 
            return;

        if (pcre2_substring_get_bynumber(
                    parse_regex_study, 3, (PCRE2_UCHAR8 **)&bytes_str, &pcre2_len) == 0) {
            ILOGNOTICE("bytes is %s", bytes_str);
            pcre2_substring_free(bytes_str);
        } else 
            return;

        
        if (pcre2_substring_get_bynumber(
                    parse_regex_study, 4, (PCRE2_UCHAR8 **)&bytes_str, &pcre2_len) == 0) {
            ILOGNOTICE("bytes is %s", bytes_str);
            pcre2_substring_free(bytes_str);
        } else 
            return;

        ILOGNOTICE("pass unit_pcre_match01");
    }
    so_pcre_deinit();
}
#endif