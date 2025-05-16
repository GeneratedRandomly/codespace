#include <cstdlib>
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>
#define LIMIT_INSERTION 10
inline int myrand(int lo, int hi) { return lo + rand() % (hi - lo); }

void insertionSort(int array[], int L, int R)
{
    for (int i = L + 1; i < R; ++i)
        for (int j = i; j > L; --j)
            if (array[j] < array[j - 1])
                std::swap(array[j], array[j - 1]);
            else
                break;
}

void dualPivotQuickSort(int array[], int lo, int hi) // [lo, hi)
{
    if (hi - lo < LIMIT_INSERTION)
        return insertionSort(array, lo, hi);

    std::swap(array[lo], array[myrand(lo, hi)]);
    std::swap(array[hi - 1], array[myrand(lo, hi)]);

    if (array[lo] > array[hi - 1])
        std::swap(array[lo], array[hi - 1]);
    int pivot1 = array[lo];
    int pivot2 = array[hi - 1];

    int lessthan = lo + 1;
    int greaterthan = hi - 2;
    int iter = lessthan;

    while (iter <= greaterthan)
    {
        if (array[iter] < pivot1)
            std::swap(array[iter], array[lessthan++]);
        else if (array[iter] > pivot2)
        {
            while (array[greaterthan] > pivot2 && iter < greaterthan)
                greaterthan--;
            std::swap(array[iter], array[greaterthan--]);
            if (array[iter] < pivot1)
                std::swap(array[iter], array[lessthan++]);
        }
        iter++;
    }

    lessthan--;
    greaterthan++;

    std::swap(array[lo], array[lessthan]);
    std::swap(array[hi - 1], array[greaterthan]);

    dualPivotQuickSort(array, lo, lessthan);
    dualPivotQuickSort(array, lessthan + 1, greaterthan);
    dualPivotQuickSort(array, greaterthan + 1, hi);
}

int partition(int array[], int lo, int hi)
{ // LGU
    std::swap(array[lo], array[myrand(lo, hi)]);
    int pivot = array[lo];
    int mi = lo;
    for (int k = lo + 1; k < hi; ++k)
        if (array[k] < pivot)
            std::swap(array[++mi], array[k]);
    std::swap(array[lo], array[mi]);
    return mi;
}

void classic_quicksort(int array[], int lo, int hi)
{
    if (hi - lo < LIMIT_INSERTION)
        return insertionSort(array, lo, hi);
    int mi = partition(array, lo, hi);
    classic_quicksort(array, lo, mi);
    classic_quicksort(array, mi + 1, hi);
}

void heapify(int array[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && array[left] > array[largest])
        largest = left;
    if (right < n && array[right] > array[largest])
        largest = right;
    if (largest != i)
    {
        std::swap(array[i], array[largest]);
        heapify(array, n, largest);
    }
}

void heapSort(int array[], int lo, int hi) // [lo, hi)
{
    int n = hi - lo;
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(array, n, i + lo);
    for (int i = n - 1; i > 0; i--)
    {
        std::swap(array[lo], array[i + lo]);
        heapify(array, i, lo);
    }
}

void introQuicksort(int array[], int lo, int hi, int depthleft) // [lo, hi)
{
    if (depthleft == 0)
        return heapSort(array, lo, hi);
    if (hi - lo < LIMIT_INSERTION)
        return insertionSort(array, lo, hi);
    int mi = partition(array, lo, hi);
    introQuicksort(array, lo, mi, depthleft - 1);
    introQuicksort(array, mi + 1, hi, depthleft - 1);
}

void introSort(int array[], int lo, int hi) // [lo, hi)
{
    int depthleft = 2 * log2(hi - lo);
    introQuicksort(array, lo, hi, depthleft);
}

int main()
{

    srand(0);
    std::ifstream infile("data.txt");

    std::vector<int> data;
    int value;
    while (infile >> value)
        data.push_back(value);
    infile.close();

    int size = data.size();
    int *array = data.data();

    // introSort(array, 0, size);
    //classic_quicksort(array, 0, size);
    dualPivotQuickSort(array, 0, size);

    return 0;
}