#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
struct node
{
    int index;
    int value;
    bool operator<(const node &other) const { return value < other.value; }
};

void quickSortWithK(std::vector<node> &array, int lo, int hi, int k);

int main()
{
    const int n = 1000000; // 数组大小
    std::vector<node> data(n);
    for (int i = 0; i < n; ++i)
        data[i] = {i, i};

    std::random_shuffle(data.begin(), data.end());

    std::vector<int> k_values = {10, 20, 30, 40, 50, 60, 70, 80, 90};
    for (int k : k_values)
    {
        std::vector<node> data_copy = data;
        clock_t start = clock();
        quickSortWithK(data_copy, 0, data_copy.size() - 1, k);
        clock_t end = clock();
        double elapsed = double(end - start) / CLOCKS_PER_SEC;
        std::cout << "k = " << k << ", time = " << elapsed << "s" << std::endl;
    }

    return 0;
}

void quickSortWithK(std::vector<node> &array, int lo, int hi, int k)
{
    if (lo < hi)
    {
        int size = hi - lo + 1;

        std::vector<node> subArray(array.begin() + lo, array.begin() + hi + 1);

        int iii = size * k / 100;
        std::nth_element(subArray.begin(),
                         subArray.begin() + iii,
                         subArray.end());
        int pivotIndex = subArray[iii].index;

        std::swap(array[lo], array[pivotIndex]);
        node pivot = array[lo];
        int i = lo + 1;
        for (int j = lo + 1; j <= hi; ++j)
            if (array[j] < pivot)
                std::swap(array[i++], array[j]);

        std::swap(array[lo], array[i - 1]);
        quickSortWithK(array, lo, i - 2, k);
        quickSortWithK(array, i, hi, k);
    }
}