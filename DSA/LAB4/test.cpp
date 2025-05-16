#include "limit.h"
#include <algorithm>
#include <ctime>
#include <iostream>
void insertionSort(int array[], int L, int R)
{
    for (int i = L + 1; i < R; ++i)
        for (int j = i; j > L; --j)
            if (array[j] < array[j - 1])
                std::swap(array[j], array[j - 1]);
            else
                break;
}
void dualPivotQuickSort(int array[], int lo, int hi)
{
    if (hi - lo < LIMIT_INSERTION)
        return insertionSort(array, lo, hi);

    if (array[lo] > array[hi - 1])
        std::swap(array[lo], array[hi - 1]);

    int pivot1 = array[lo];
    int pivot2 = array[hi - 1];

    int lt = lo + 1;
    int gt = hi - 2;
    int i = lo + 1;

    while (i <= gt)
    {
        if (array[i] < pivot1)
        {
            std::swap(array[i], array[lt]);
            lt++;
        }
        else if (array[i] > pivot2)
        {
            std::swap(array[i], array[gt]);
            gt--;
            i--;
        }
        i++;
    }

    lt--;
    gt++;

    std::swap(array[lo], array[lt]);
    std::swap(array[hi - 1], array[gt]);

    dualPivotQuickSort(array, lo, lt);
    dualPivotQuickSort(array, lt + 1, gt);
    dualPivotQuickSort(array, gt + 1, hi);
}

int main()
{
    const int SIZE = 100; // 数据规模

    int array[SIZE];
    srand(time(0));
    for (int i = 0; i < SIZE; ++i)
        array[i] = SIZE - i;
    dualPivotQuickSort(array, 0, SIZE);
    for (int i = 0; i < SIZE; ++i)
        std::cout << array[i] << " ";
    return 0;
}