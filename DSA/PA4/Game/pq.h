using Rank = unsigned int;
template <typename T>
void swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}
template <typename T>
struct PQ
{                               // 优先级队列PQ接口
    virtual void insert(T) = 0; // 按照比较器确定的优先级次序插入词条
    virtual T delMax() = 0;     // 删除优先级最高的词条
    virtual T &getMax() = 0;    // 取出优先级最高的词条
};

#define DEFAULT_CAPACITY 3 // 默认的初始容量（实际应用中可设置为更大）
template <typename T>
class Vector
{ // 向量模板类
protected:
    Rank _size;
    Rank _capacity;
    T *_elem; // 规模、容量、数据区
public:
    void expand();                               // 向量空间不足时扩容
    void copyFrom(T const *A, Rank lo, Rank hi); // 复制数组区间A[lo, hi)
    // 构造方法
    Vector(Rank c = DEFAULT_CAPACITY) // 容量为c的空向量
    {
        _elem = new T[_capacity = c];
        _size = 0;
    }
    Vector(Rank c, Rank s, T v) // 容量为c、规模为s、所有元素初始为v；s<=c
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v)
            ;
    }
    Vector(T const *A, Rank n) { copyFrom(A, 0, n); }                           // 数组整体复制
    Vector(T const *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }               // 区间
    Vector(Vector<T> const &V) { copyFrom(V._elem, 0, V._size); }               // 向量整体复制
    Vector(Vector<T> const &V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } // 区间
    // 析构方法
    ~Vector() { delete[] _elem; } // 释放内部空间
    // 只读访问接口
    Rank size() const { return _size; }                          // 规模
    bool empty() const { return !_size; }                        // 判空
    Rank select(Rank k) { return quickSelect(_elem, _size, k); } // 从无序向量中找到第k大的元素
    Rank search(T const &e) const                                // 有序向量整体查找
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    // 可写访问接口
    T &operator[](Rank r) { return _elem[r]; }           // 重载下标操作符，可以类似于数组形式引用各元素
    Rank insert(Rank r, T const &e);                     // 插入元素
    Rank insert(T const &e) { return insert(_size, e); } // 默认作为末元素插入
}; // Vector

template <typename T> // 将e插入至[r]
Rank Vector<T>::insert(Rank r, T const &e)
{ // 0 <= r <= size
    // print( e );
    expand();                        // 如必要，先扩容
    for (Rank i = _size; r < i; i--) // 自后向前，后继元素
        _elem[i] = _elem[i - 1];     // 顺次后移一个单元
    _elem[r] = e;
    _size++;  // 置入新元素并更新容量
    return r; // 返回秩
}
template <typename T>
void Vector<T>::expand()
{ // 向量空间不足时扩容
    if (_size < _capacity)
        return; // 尚未满员时，不必扩容
    T *oldElem = _elem;
    copyFrom(oldElem, 0, _capacity); // 容量加倍
    delete[] oldElem;                // 释放原空间
}
template <typename T> // T为基本类型，或已重载赋值操作符'='
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{                                                                          // 以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[_capacity = max<Rank>(DEFAULT_CAPACITY, (hi - lo) * 2)]; // 分配空间
    for (_size = 0; lo < hi; _size++, lo++)                                // A[lo, hi)内的元素逐一
        _elem[_size] = A[lo];                                              // 复制至_elem[0, hi-lo)：T为基本类型，或已重载赋值操作符'='
} // 用const修饰，保证A中的元素不致被篡改；运行时间 = O(hi-lo)

#define Parent(i) (((i) - 1) >> 1)                                                                                             // PQ[i]的父节点（floor((i-1)/2)，i无论正负）
#define ProperParent(PQ, n, i)                                                                                                 /*父子（至多）三者中的大者*/ \
    (RChildValid(n, i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i)) : (LChildValid(n, i) ? Bigger(PQ, i, LChild(i)) : i)) // 相等时父节点优先，如此可避免不必要的交换
template <typename T>
struct PQ_ComplHeap : public PQ<T>, public Vector<T>
{                     // 完全二叉堆
    PQ_ComplHeap() {} // 默认构造
    PQ_ComplHeap(T *A, Rank n)
    {
        copyFrom(A, 0, n);
        heapify(_elem, n);
    } // 批量构造
    void insert(T); // 按照比较器确定的优先级次序，插入词条
    T delMax();     // 删除优先级最高的词条
    T &getMax();    // 读取优先级最高的词条
}; // PQ_ComplHeap
template <typename T>
Rank percolateDown(T *A, Rank n, Rank i); // 下滤
template <typename T>
Rank percolateUp(T *A, Rank i); // 上滤

template <typename T>
void PQ_ComplHeap<T>::insert(T e)
{                                  // 将词条插入完全二叉堆中
    Vector<T>::insert(e);          // 将新词条接至向量末尾
    percolateUp(_elem, _size - 1); // 再对该词条实施上滤调整
}
template <typename T>
T PQ_ComplHeap<T>::delMax()
{                                   // 取出最大词条
    swap(_elem[0], _elem[--_size]); // 堆顶、堆尾互换（_size的递减，不致引发shrink()）
    percolateDown(_elem, _size, 0); // 新堆顶下滤
    return _elem[_size];            // 返回原堆顶
}
template <typename T>
T &PQ_ComplHeap<T>::getMax() { return _elem[0]; } // 取优先级最高的词条
// 对向量前n个词条中的第i个实施下滤，i < n
template <typename T>
Rank percolateDown(T *A, Rank n, Rank i)
{
    Rank j;                                  // i及其（至多两个）孩子中，堪为父者
    while (i != (j = ProperParent(A, n, i))) // 只要i非j，则
        swap(A[i], A[j]), i = j;             // 二者换位，并继续考查下降后的i
    return i;                                // 返回下滤抵达的位置（亦i亦j）
}
template <typename T>
Rank percolateUp(T *A, Rank i)
{ // 对词条A[i]做上滤，0 <= i < _size
    while (0 < i)
    {                       // 在抵达堆顶之前，反复地
        Rank j = Parent(i); // 考查[i]之父亲[j]
        if (!(A[j] < A[i]))
            break; // 一旦父子顺序，上滤旋即完成；否则
        swap(A[i], A[j]);
        i = j; // 父子换位，并继续考查上一层
    } // while
    return i; // 返回上滤最终抵达的位置
}