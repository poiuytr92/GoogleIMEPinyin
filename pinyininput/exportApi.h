#pragma once

#ifdef _USRDLL
#define CREATEDLL_API extern "C"  __declspec(dllexport)
#else
#define CREATEDLL_API  extern "C" __declspec(dllimport)
#endif

CREATEDLL_API int ImInit(const char* dict_path, const char* dict_user_input);

CREATEDLL_API void ImSetMaxLens(int max_sps_len,int max_hzs_len);

CREATEDLL_API void ImResetSearch();

CREATEDLL_API int ImSearch(const char* input, int sps_len);

CREATEDLL_API void ImGetCandidate(int cand_id,short* input, int max_len, int* cand_len);
