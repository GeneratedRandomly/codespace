#include <fstream>
#include "limit.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>
const int maxn = 10000000;
// int myindex[maxn];
// int cnt = 0;
// inline int myrand(int lo, int hi) { return lo + rand() % (hi - lo); }
// void insertionSort(int array[], int L, int R)
// {
//     for (int i = L + 1; i < R; ++i)
//         for (int j = i; j > L; --j)
//             if (array[j] < array[j - 1])
//             {
//                 std::swap(array[j], array[j - 1]);
//                 std::swap(myindex[j], myindex[j - 1]);
//             }
//             else
//                 break;
// }
// int partition(int array[], int lo, int hi)
// { // LGU
//     int r = myrand(lo, hi);
//     std::swap(array[lo], array[r]);
//     std::swap(myindex[lo], myindex[r]);
//     int pivot = array[lo];
//     int mi = lo;
//     for (int k = lo + 1; k < hi; ++k)
//         if (array[k] < pivot)
//         {
//             std::swap(array[++mi], array[k]);
//             std::swap(myindex[mi], myindex[k]);
//         }
//     std::swap(array[lo], array[mi]);
//     std::swap(myindex[lo], myindex[mi]);
//     return mi;
// }
// void classic_quicksort(int array[], int lo, int hi)
// {
//     if (hi - lo < LIMIT_INSERTION)
//     {
//         insertionSort(array, lo, hi);
//         return;
//     }
//     int mi = partition(array, lo, hi);
//     classic_quicksort(array, lo, mi);
//     classic_quicksort(array, mi + 1, hi);
// }
int main()
{
    // int array[maxn];
    // memset(array, 0, sizeof(myindex));
    // for (int i = 0; i < maxn; ++i)
    //     myindex[i] = i;
    srand(time(0));
    int *array = new int[maxn];
    for (int i = 0; i < maxn; ++i)
        array[i] = rand();
    // classic_quicksort(array, 0, maxn);
    std::ofstream dataout("data.txt");
    // for (int i = 0; i < maxn; ++i)
    //     array[myindex[i]] = i;
    // for (int i = 0; i < maxn; ++i)
    //     dataout << array[i] << " ";
    for (int i = 0; i < maxn; ++i)
        dataout << array[i] << " ";
    return 0;
}