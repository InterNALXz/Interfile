#include "hs.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct HSTX {
	hs_database_t *db;
	hs_scratch_t *scratch;
}SCtx;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

char *strbuf[5] = {
	"hello",
	"world",
	"this",
	"come in",
	"over"
};

typedef struct ContentData{
	char **exp;
	int *ids;
}CD;

/*       convert str to regrex          */
char *HSRenderPattern(const uint8_t *pat, uint16_t pat_len)
{
    if (pat == NULL) {
        return NULL;
    }
    const size_t hex_len = (pat_len * 4) + 1;
    char *str = malloc(hex_len);
    if (str == NULL) {
        return NULL;
    }
    memset(str, 0, hex_len);
    char *sp = str;
    for (uint16_t i = 0; i < pat_len; i++) {
        snprintf(sp, 5, "\\x%02x", pat[i]);
        sp += 4;
    }
    *sp = '\0';
    return str;
}

static int BuildMUlitDateBase(SCtx *sctx, int len) {
	CD cas = {0};

	cas.exp = malloc(5*sizeof(char *));
	cas.ids = malloc(5*sizeof(int *));

	for(int i = 0;i<5;i++) {
		cas.ids[i] = i;
		cas.exp[i] = HSRenderPattern(strbuf[i], strlen(strbuf[i]));
	}

	int nocase = 0;
	unsigned flags = nocase ? HS_FLAG_CASELESS : 0;

    hs_database_t *db = NULL;
    hs_compile_error_t *compile_err = NULL;
    hs_error_t err = hs_compile_ext_multi(cas.exp, flags, cas.ids, NULL, 5, HS_MODE_BLOCK, NULL, &db,
                                &compile_err);
								
	if (err != 0){
		printf("Build Error\n");
		return -1;
	}

	for(int i = 0;i<5;i++) {
		free(cas.exp[i]);
	}

	hs_scratch_t *scratch = NULL;
    err = hs_alloc_scratch(db, &scratch);
	if (err < 0) {
		printf("alloc err!");
		return -1;
	}
	sctx->db = db;
	sctx->scratch = scratch;
}

static int MatchEvent(unsigned int id, unsigned long long from,
                      unsigned long long to, unsigned int flags, void *context)
{
	printf("is matched!\n");
	return 0;
}


static void HSfree(SCtx *sctx) {
	hs_free_scratch(sctx->scratch);

	hs_free_database(sctx->db);
}

static int HSScan(SCtx *sctx, const char *haystack, int haystack_len) {
	uint64_t match_offset = 0;
    hs_error_t err = hs_scan(sctx->db, (const char *)haystack, haystack_len, 0,
                             sctx->scratch, MatchEvent, &match_offset);
	if (err != 0 &&err != -3)
	{
		printf("scan wrong!errcode is [%d]\n", err);
		HSfree(sctx);
		return -1;
	}
	HSfree(sctx);
	return 0;
}

int main(){
	SCtx sctx = {0};
	char *exp1 = HSRenderPattern("right!", 7);
	BuildMUlitDateBase(&sctx, 5);

	free(exp1);
	char *matchstr = "i am right!over";
	HSScan(&sctx, matchstr, strlen(matchstr));
	return 0;
}