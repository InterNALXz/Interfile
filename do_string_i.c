#include "file_i.h"
#include "do_string_i.h"

static type_i string_i[] = {
    {0, "MPEG", 0},
    {1, "MP4", 0},
    {2, "Zip", 0},
};

static int g_string_sizei = 3;

typedef struct ContentData{
	char **exp;
	int *ids;
    int *flags;
}CoD;

void NoUseFreeI(CoD *kitty, int flag) {
	if (kitty->exp) {
		if (flag & 1) {
			for(int i = 0;i < g_string_sizei;i++) {
				I_FREE(kitty->exp[i]);
			}
		}

		free(kitty->exp);
	}

	I_FREE(kitty->ids);

    I_FREE(kitty->flags);

	if (flag & 2)
		free(kitty);
}

/*       convert str to regrex          */
char *HSRenderPattern(const uint8_t *pat, uint16_t pat_len)
{
    if (pat == NULL) {
        return NULL;
    }

    const size_t hex_len = (pat_len * 4) + 1;
    char *str = malloc(hex_len);
    if (str == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "alloc pattern error");
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

CoD *BuildHSDataBase(void) {
    CoD *cd = calloc(1, sizeof(CoD));

    if (cd == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "Build hs database error");
        goto out;
    }

    cd->exp = calloc(g_string_sizei, sizeof(char *));
    cd->flags = calloc(g_string_sizei, sizeof(int *));
    cd->ids = calloc(g_string_sizei, sizeof(int *));

    if (cd->exp == NULL || cd->flags == NULL || cd->ids == NULL) {
        ILOGDERR(ERROR_ALLOC_ERR, "Build hs database error");
        goto error;
    }

    for (int i = 0;i < g_string_sizei;i++) {
		cd->ids[i] = i;
		cd->exp[i] = HSRenderPattern(string_i[i].name, strlen(string_i[i].name));

        if (cd->exp[i] == NULL) {
            goto error;
        }

        cd->flags[i] = HS_FLAG_SINGLEMATCH;
        cd->flags[i] |= HS_FLAG_CASELESS;
	}

out:
    return cd;

error:
    NoUseFreeI(cd, 3);
    return NULL;
}

int HyperStringInit(ICotx *icnox) {
    if (icnox == NULL) {
        ILOGDERR(ERROR_WRONG_ARG, "init string error");
        return -1;
    }

    int ret = 0;

    CoD *inx = BuildHSDataBase();
    if (inx == NULL)
        return -1;

    hs_database_t *db = NULL;
    hs_compile_error_t *compile_err = NULL;
    hs_error_t err = hs_compile_ext_multi((const char *const *)inx->exp, inx->flags, inx->ids, NULL, g_string_sizei, HS_MODE_BLOCK, NULL, &db,
                                &compile_err);
								
	if (err != 0){
		ILOGDERR(ERROR_BUILD_MUT, "Build Error, Error:{%s}", compile_err->message);
        hs_free_compile_error(compile_err);
		ret = -1;
        goto out;
	}

	hs_scratch_t *scratch = NULL;
    err = hs_alloc_scratch(db, &scratch);
	if (err < 0) {
		ILOGDERR(ERROR_ALLOC_ERR, "alloc scratch err!");
		ret = -1;
        goto out;
	}
	icnox->db = db;
	icnox->scratch = scratch;

out:
	NoUseFreeI(inx, 3);

    if (ret == -1) {
        I_FREE(db)
        I_FREE(scratch)
    }

	return ret;
}

int HyperStringDeinit(ICotx *icnox) {
	hs_free_scratch(icnox->scratch);

	hs_free_database(icnox->db);
}

static int MatchEvent(unsigned int id, unsigned long long from,
                      unsigned long long to, unsigned int flags, void *context)
{
	ILOGNOTICE("Got a resault is matched!file type is %s", string_i[id].name);
	return 0;
}

static int HSScan(ICotx *icnox, const char *haystack, int haystack_len) {
	uint64_t match_offset = 0;
    hs_error_t err = hs_scan(icnox->db, (const char *)haystack, haystack_len, 0,
                             icnox->scratch, MatchEvent, &match_offset);
	if (err != 0 &&err != -3)
	{
		ILOGDERR(ERROR_HS_GET, "scan wrong!errcode is [%d]", err);
		return -1;
	}

	return 0;
}

#ifdef UNIT_TEST_I
#define TEST_STRING01 "Audio file with ID3 version 2.3.0, contains:MPEG ADTS, layer III, v1, 256 kbps, 44.1 kHz, Stereo"
#define TEST_STRING02 "ISO Media, MP4 Base Media v1 [IS0 14496-12:2003]"
#define TEST_STRING03 "Zip archive data, at least v2.0 to extract"
void unit_strring_match01(void) {
    ICotx icnox = {0};
    HyperStringInit(&icnox);
    HSScan(&icnox, TEST_STRING01, strlen(TEST_STRING01));
    HSScan(&icnox, TEST_STRING02, strlen(TEST_STRING02));
    HSScan(&icnox, TEST_STRING03, strlen(TEST_STRING03));
    HyperStringDeinit(&icnox);
}
#endif
