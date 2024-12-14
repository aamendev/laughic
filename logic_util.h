#ifndef LOGICUTIL
#define LOGICUTIL
#include <stdlib.h>
void swap(int* a, int* b);
void sortX(int* x0, int* y0, int* x1, int* y1, int* x2, int* y2);
void sort(int* data, int stride, int count, int sortElement);
int area(int x0, int y0, int x1, int y1, int x2, int y2);
void bary(int x0, int y0, int x1, int y1, int x2, int y2, int x, int y, float* bary);
float rand_float();
#endif
