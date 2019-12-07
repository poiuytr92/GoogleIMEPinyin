#include "stdafx.h"
#include "exportApi.h"
#include <iostream>
using namespace std;

#include "./include/pinyinime.h"

using namespace ime_pinyin;

static size_t output_len;
// fix cand_len
static size_t fix_cand_len(size_t cand, int output_len)
{
#define MAX_SINGLE 40   // single hz MAX is 40
	size_t i;
	size_t decode_len;
	size_t single = 0;
	size_t multi = 0;
	char16 *cand_buf = new char16[output_len];

	im_get_sps_str(&decode_len);
	if (decode_len == 1) { // only one spell char
		if (cand > 10) {
			cand = 10; // max is 10.
		}
		delete cand_buf;
		return  cand;
	}

	for (i = 0; i < cand; i++) {
		im_get_candidate(i, cand_buf, output_len);
		if (strlen((char *)cand_buf) > 2) {
			multi++;
		}
		else {
			single++;
			if (single > MAX_SINGLE) {
				break;
			}
		}
	}

	cand = multi + single;
	delete cand_buf;
	return  cand;
}


int ImInit(const char* dict_path,const char* dict_user_path)
{
	cout << "InitPinYin" << endl;
	//³õÊ¼»¯ "D:\\code\\cc-keyboard\\cckeyboard\\x64\\Debug\\dict\\dict_pinyin.dat"
	//"D:\\code\\cc-keyboard\\cckeyboard\\x64\\Debug\\dict\\dict_pinyin_user.dat"
	bool result = im_open_decoder(dict_path, dict_user_path);

	return result;
}


void ImSetMaxLens(int max_sps_len, int max_hzs_len)
{
	im_set_max_lens(max_sps_len, max_hzs_len);

	output_len = max_hzs_len;
}

void ImResetSearch()
{
	im_reset_search();
}

int ImSearch(const char* input, int sps_len)
{
	size_t cand = im_search(input, sps_len);

	cand = fix_cand_len(cand, output_len);

	return cand;
}

void ImGetCandidate(int cand_id, short* input, int max_len, int* cand_len)
{
	char16 *cand_buf = new char16[max_len];

	char16 *cand = im_get_candidate(cand_id, cand_buf, max_len);
	
	size_t candResultLen = utf16_strlen(cand);

	//candResultLen = fix_cand_len(candResultLen, output_len);

	*cand_len = candResultLen;

	int candLen = *cand_len;

	memcpy(input, cand, sizeof(char16) * candLen);

	input[candLen + 1] = 0;

	delete cand_buf;
}