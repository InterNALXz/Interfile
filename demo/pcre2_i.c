#include "stdio.h"
#include "string.h"
#include <stdbool.h>
#include <pcre2.h>

//typedef pcre2_code pcre2_code_;
//

typedef struct DetectParseRegex {
    pcre2_code *regex;
    pcre2_match_context *context;
    pcre2_match_data *match;
    struct DetectParseRegex *next;
} DetectParseRegex;

static const char *decode_pattern = "\\s*(bytes\\s+(\\d+),?)?";
static DetectParseRegex decode_pcre;

bool PCRE_REGXES(const char *parse_str, DetectParseRegex *detect_parse, int opts){
    int en;
    PCRE2_SIZE eo;

    detect_parse->regex =
            pcre2_compile((PCRE2_SPTR8)parse_str, PCRE2_ZERO_TERMINATED, opts, &en, &eo, NULL);
    if (detect_parse->regex == NULL) {
        PCRE2_UCHAR errbuffer[256];
        pcre2_get_error_message(en, errbuffer, sizeof(errbuffer));
        return false;
    }
    detect_parse->match = pcre2_match_data_create_from_pattern(detect_parse->regex, NULL);
    return true;
}

void DetectParseFreeRegex(DetectParseRegex *r)
{
    if (r->regex) {
        pcre2_code_free(r->regex);
    }
    if (r->context) {
        pcre2_match_context_free(r->context);
    }
    if (r->match) {
        pcre2_match_data_free(r->match);
    }
}

void main(int argc, char *argv[]){
    char buf[25] = {0};

    size_t pcre2_len;
    const char *bytes_str = NULL;

    if ( !PCRE_REGXES(decode_pattern, &decode_pcre, 0) )
        printf("wrong!\n");

    if (argc > 1)
        memcpy(buf, argv[1], 25);
    else
        memcpy(buf, "no huge!", 25);

    printf("regex is %p, match is %p\n", decode_pcre.regex, decode_pcre.match);

    int pcre_rc = pcre2_match(decode_pcre.regex, (PCRE2_SPTR8)buf, strlen(buf), 0, 0,
            decode_pcre.match, NULL);

    printf("n is %d\n", pcre_rc);
    if(pcre_rc < 3)
        printf("error\n");
    if (pcre_rc >= 3) {
        if (pcre2_substring_get_bynumber(
                    decode_pcre.match, 2, (PCRE2_UCHAR8 **)&bytes_str, &pcre2_len) == 0) {
            printf("bytes is %lu\n", bytes_str);
        }
    }
    char *endptr = NULL;
    unsigned long till = strtoull(bytes_str, &endptr, 10);

    printf("till is %lu\n", till);

    DetectParseFreeRegex(&decode_pcre);
    printf("%s\n", buf);
}