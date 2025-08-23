#ifndef PROCESSING_UTILS_H
#define PROCESSING_UTILS_H
#include "../types.h"
void p_generic_inplace_min(u32* a, u32 b, i32* h, u32 s1);
void p_generic_inplace_max(u32* a, u32 b, i32* h, u32 s1);

void p_s_generic_inplace_max(i64* a, i64 b, i32* h, u32 s1);
void p_s_generic_inplace_min(i64* a, i64 b, i32* h, u32 s1);
#endif

