#include <stdio.h>
#include <climits>
// attribution

// copilot has helped me a lot when it:
// created a vector version;
// and teached me why it wrote the code like that;
// help me to write quickselect function;

// oiwiki helped me to understand kd tree more deeply.

#define ll long long
// 每个节点代表一个超长方体
struct KDNode
{
    int *point = nullptr; // 该节点的坐标
    KDNode *left = nullptr, *right = nullptr;
};

int **points = nullptr; // 直接输入到这个数组里，每个kdnode的point指向这个数组的一个元素
int n, dimension;       // n是点的个数，dimension是维数
KDNode *root = nullptr;

void swap(int *&a, int *&b)
{
    int *temp = a;
    a = b;
    b = temp;
}

int partition(int low, int high, int axis) //[low, high]//每次发现比pivot小的，稳定地排到左边，最后把pivot放到中间
{
    int *pivot = points[high];
    int i = low - 1;
    for (int j = low; j < high; ++j)
    {
        if (points[j][axis] <= pivot[axis])
        {
            ++i;
            swap(points[i], points[j]);
        }
    }
    swap(points[i + 1], points[high]);
    return i + 1;
}

int quickselect(int low, int high, int k, int axis) //[low, high]
{
    if (low == high)
        return low;
    int pivotIndex = partition(low, high, axis);
    if (pivotIndex == k)
        return pivotIndex;
    else if (pivotIndex < k)
        return quickselect(pivotIndex + 1, high, k, axis);
    else
        return quickselect(low, pivotIndex - 1, k, axis);
}

KDNode *build(int **points, int start, int end, int depth) //[start, end)
{
    if (start >= end) // 空区间
        return nullptr;
    int axis = depth % dimension;
    int midIndex = (start + end) >> 1;
    quickselect(start, end - 1, midIndex, axis); // 快速选择，使中位数到midIndex处，左小右大，类似nth_element
    KDNode *node = new KDNode();
    node->point = points[midIndex];
    node->left = build(points, start, midIndex, depth + 1);
    node->right = build(points, midIndex + 1, end, depth + 1);
    return node;
}

ll distance(int *a, int *b)
{
    ll dist = 0;
    for (int i = 0; i < dimension; ++i)
        dist += (ll)(a[i] - b[i]) * (ll)(a[i] - b[i]);
    return dist;
}

void nearest(KDNode *node, int *target, int depth, ll &bestDist)
{
    if (!node)
        return;
    int axis = depth % dimension;
    ll dist = distance(node->point, target);
    if (dist < bestDist)
        bestDist = dist;
    ll x = target[axis];
    ll x_n = node->point[axis]; 
    KDNode *next = (x < x_n) ? node->left : node->right;
    KDNode *other = (x < x_n) ? node->right : node->left;
    nearest(next, target, depth + 1, bestDist);
    if ((x - x_n) * (x - x_n) < bestDist)
        nearest(other, target, depth + 1, bestDist);
}

int main()
{
    scanf("%d %d", &dimension, &n);
    points = new int *[n];
    for (int i = 0; i < n; ++i)
    {
        points[i] = new int[dimension];
        for (int j = 0; j < dimension; ++j)
            scanf("%d", &points[i][j]);
    }

    root = build(points, 0, n, 0);

    int q;
    scanf("%d", &q);
    int *x = new int[dimension];
    for (int i = 0; i < q; ++i)
    {
        for (int j = 0; j < dimension; ++j)
            scanf("%d", &x[j]);
        ll bestDist = LLONG_MAX;
        nearest(root, x, 0, bestDist);
        printf("%lld\n", bestDist);
    }

    return 0;
}