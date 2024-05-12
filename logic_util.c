#include "./logic_util.h"
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sortX(int* x0, int* y0, int* x1, int* y1, int* x2, int* y2)
{
    int* arr[3] = {x0, x1, x2};
    int* arry[3] = {y0, y1, y2};
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (*(arr[j]) > *(arr[j + 1]))
            {
                swap(arr[j], arr[j + 1]);
                swap(arry[j], arry[j + 1]);
            }
        }
    }
}

void sort(int* data, int stride, int count, int sortElement)
{
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count - 1; j++)
        {
            if (data[j * stride + sortElement] > data[(j + 1) * stride + sortElement])
            {
                for (int k = 0; k < stride; k++)
                {
                    swap(&data[j * stride + k], &data[(j + 1) * stride + k]);
                }
            }
        }
    }
}

int area(int x0, int y0, int x1, int y1, int x2, int y2)
{
    return (x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)) / 2;
}

void bary(int x0, int y0, int x1, int y1, int x2, int y2, int x, int y, float* bary)
{
    float lambda1 = (y1 - y2) * (x - x2) + (x2 - x1) * (y - y2);
    lambda1 /= ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));

    float lambda2 = (y2 - y0) * (x - x2) + (x0 - x2) * (y - y2);
    lambda2 /= ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));

    bary[0] = lambda1;
    bary[1] = lambda2;
    bary[2] = 1 - lambda1 - lambda2;
}
