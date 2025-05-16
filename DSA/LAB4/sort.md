## 任务一
我随机生成了三次10000000个随机数，使用不同的LIMIT_INSERTION调用快排，结果如下表。
|LIMIT_INSERTION|1|2|3|4|5|6|7|8|9|10|
|---|---|---|---|---|---|---|---|---|---|---|
|**USER TIME**|**3.050s**|**3.067s**|**2.963s**|**2.963s**|**2.915s**|**2.878s**|**2.801s**|**2.881s**|**2.843s**|**2.811s**|
|**USER TIME**|**3.025s**|**3.008s**|**2.918s**|**3.073s**|**2.943s**|**2.860s**|**2.859s**|**2.826s**|**2.886s**|**2.856s**|
|**USER TIME**|**3.048s**|**3.063s**|**2.917s**|**2.912s**|**2.845s**|**2.848s**|**2.882s**|**2.827s**|**2.914s**|**2.823s**|
|**LIMIT_INSERTION**|**11**|**12**|**13**|**14**|**15**|**20**|**50**|**100**|**500**|**1000**|
|**USER TIME**|**2.814s**|**2.827s**|**2.848s**|**2.804s**|**2.841s**|**2.881s**|**2.950s**|**3.420s**|**6.965s**|**11.485s**|
|**USER TIME**|**2.852s**|**2.850s**|**2.824s**|**2.829s**|**2.913s**|**2.820s**|**3.041s**|**3.336s**|**6.905s**|**11.557s**|
|**USER TIME**|**2.894s**|**2.829s**|**2.877s**|**2.833s**|**2.877s**|**2.795s**|**3.011s**|**3.418s**|**6.994s**|**11.5132s**|
确实可以看出，随着快排转变为插入排序的长度限LIMIT变大，效率先变高，然后变低。
## 任务二
内省排序代码：
~~~cpp
inline int myrand(int lo, int hi)...
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
void insertionSort(int array[], int L, int R)...
int partition(int array[], int lo, int hi)...
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
void introSort(int array[], int size) // [0, size)
{
    int depthleft = 2 * log2(size - lo);
    introQuicksort(array, 0, size, depthleft);
}
~~~
首先，即使是随机测例，内省排序的效率也更高，插入限制小于等于50时，都在两秒以内，1000时也只有7.5秒。我们构造数据，让pivot每次都会选中区间极值，我们用一个数组跟踪被排序数组每个元素的索引即可。数据构造代码如下。
~~~cpp
#include <fstream>
#include "limit.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>
const int maxn = 100000;
int myindex[maxn];
int cnt = 0;
inline int myrand(int lo, int hi) { return lo + rand() % (hi - lo); }
void insertionSort(int array[], int L, int R)
{
    for (int i = L + 1; i < R; ++i)
        for (int j = i; j > L; --j)
            if (array[j] < array[j - 1])
            {
                std::swap(array[j], array[j - 1]);
                std::swap(myindex[j], myindex[j - 1]);
            }
            else
                break;
}
int partition(int array[], int lo, int hi)
{ // LGU
    int r = myrand(lo, hi);
    std::swap(array[lo], array[r]);
    std::swap(myindex[lo], myindex[r]);
    int pivot = array[lo];
    int mi = lo;
    for (int k = lo + 1; k < hi; ++k)
        if (array[k] < pivot)
        {
            std::swap(array[++mi], array[k]);
            std::swap(myindex[mi], myindex[k]);
        }
    std::swap(array[lo], array[mi]);
    std::swap(myindex[lo], myindex[mi]);
    return mi;
}
void classic_quicksort(int array[], int lo, int hi)
{
    if (hi - lo < LIMIT_INSERTION)
    {
        insertionSort(array, lo, hi);
        return;
    }
    int mi = partition(array, lo, hi);
    classic_quicksort(array, lo, mi);
    classic_quicksort(array, mi + 1, hi);
}
int main()
{
    int array[maxn];
    memset(array, 0, sizeof(myindex));
    for (int i = 0; i < maxn; ++i)
        myindex[i] = i;
    srand(0);
    classic_quicksort(array, 0, maxn);
    std::ofstream dataout("data.txt");
    for (int i = 0; i < maxn; ++i)
        array[myindex[i]] = i;
    for (int i = 0; i < maxn; ++i)
        dataout << array[i] << " ";
    return 0;
}
~~~
对于LIMIT_INSERTION=10进行三次测试，结果如下。
|introSort|0.010s|0.005s|0.005s|
|---|---|---|---|
|**quickSort**|**2.868s**|**2.864s**|**2.909s**|
差别极大。
## 任务三
dualPivotQuickSort代码如下。
~~~cpp
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
~~~
性能测试结果：
第一次：快排1.956s，双轴快排1.729s  
第二次：快排1.948s，双轴快排1.700s  
第三次：快排1.918s，双轴快排1.744s  
数据使用的是LIMIT_INSERTION=10，数据规模10000000，完全随机，三次不同。  
复杂度分析：  
粗略来看，双轴快排$T(n)=n+3T(n/3)$，所以有$T(n)=n+n\log_3n$。递推来看的话，$T(n)=n-1+\frac{1}{n}\sum_{i=0}^{n-1}\{T(i)+\frac{1}{n-i-1}\sum_{j=0}^{n-i-2}[T(j)+T(n-i-j-1)]\}$，复杂很多，难以分析。双轴快排似乎对快排做了常数优化，但它的“partition”部分的复杂度常数也比较高，根据[论文](https://arxiv.org/pdf/1511.01138)，双轴快排的复杂度比快排要高，但利用了缓存特性，所以速度更快。
## 任务四
这部分代码和数据生成是放在一起的。
~~~cpp
struct node
{
    int index;
    int value;
    bool operator<(const node &other) const { return value < other.value; }
};
void quickSortWithK(std::vector<node> &array, int lo, int hi, int k)
{
    if (lo < hi)
    {
        //找到k%大的元素，复制子数组以免影响原数组
        int size = hi - lo + 1;
        std::vector<node> subArray(array.begin() + lo, array.begin() + hi + 1);
        int iii = size * k / 100;
        std::nth_element(subArray.begin(),
                         subArray.begin() + iii,
                         subArray.end());
        int pivotIndex = subArray[iii].index;
        //以这个元素作为枢轴快排
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
int main()
{
    const int n = 5000000; // 数组大小
    std::vector<node> data(n);
    for (int i = 0; i < n; ++i)
        data[i] = {i, i};
    //随机打乱
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
~~~
数据生成之后就随机打乱了，数据规模5000000，对k=10到90每10都进行测试，出现了预期结果。
|k值|10|20|30|40|50|60|70|80|90|
|---|---|---|---|---|---|---|---|---|---|
|时间/秒|4.84|5.30|5.41|5.36|5.35|5.38|5.38|4.89|4.73|
|时间/秒|4.73|5.12|5.22|5.38|5.28|5.28|5.34|4.91|4.72|
我认为，在k比较极端的时候分支预测经常正确，减少了很多错误重来花费的时间，所以k=10或90时速度很快。随着k向中间移动，分支预测的错误率快速增加，耗时上升。当k靠近50时，分支预测对时间的影响变小，但由于取中位数枢轴，快排深度下降，时间又稍微减少。k和(1-k)的结果并不对称，我认为这是CPU分支预测算法导致的。我们能看到，每次k和(1-k)的时间对比虽然不确定的（有时30比70大，有时又反过来）但我们也注意到k=20就是比k=80慢，猜测可能是静态分支预测的影响，可能CPU会混合多种分支预测算法。