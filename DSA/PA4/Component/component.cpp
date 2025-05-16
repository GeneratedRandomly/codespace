#include <stdio.h>

void swap(int &a, int &b)
{
    int t = a;
    a = b;
    b = t;
}

int g_n, g_m, g_k, g_q;

struct pddNode
{
    int weight;
    int child = 0, sibling = 0;
};

struct pdd
{
    pddNode *nodes = nullptr;

    pdd(int n) { nodes = new pddNode[n]; }

    int meldpdd(int x, int y) // 合并两个堆
    {
        if (x == 0)
            return y;
        if (y == 0)
            return x;
        if (nodes[x].weight > nodes[y].weight)
            swap(x, y);
        // swap后x为权值小的堆，y为权值大的堆
        // 将y设为x的儿子
        nodes[y].sibling = nodes[x].child;
        nodes[x].child = y;
        return x; // 新的根节点为 x
    }

    int mergepdd(int x) // 合并兄弟堆
    {
        if (x == 0 || nodes[x].sibling == 0)
            return x;                               // 如果该树为空或他没有下一个兄弟，就不需要合并了，return。
        int y = nodes[x].sibling;                   // y 为 x 的下一个兄弟
        int c = nodes[y].sibling;                   // c 是再下一个兄弟
        nodes[x].sibling = nodes[y].sibling = 0;    // 拆散
        return meldpdd(mergepdd(c), meldpdd(x, y)); // 核心部分，递归合并
    }

    int delete_minpdd(int x)
    {
        int t = mergepdd(nodes[x].child);
        return t;
    }

    int getweight(int x) { return nodes[x].weight; }
};

struct dsu
{
    int *father = nullptr, *dsusize = nullptr, *pddsize = nullptr, *pddroot = nullptr;
    // pddsize not always == dsusize!!
    // only need to ensure dsuroot's pddroot/dsusize
    // but pddsize of all nodes must be correct

    pdd *p;

    dsu(int n)
    {
        father = new int[n];
        dsusize = new int[n];
        pddsize = new int[n];
        pddroot = new int[n];
        p = new pdd(n);
        for (int i = 0; i < n; ++i)
        {
            father[i] = i;
            dsusize[i] = 1;
            pddsize[i] = 1;
        }
    }

    int get_size(int x) { return dsusize[findroot(x)]; }

    int findroot(int x) { return father[x] == x ? x : father[x] = findroot(father[x]); } // x所在树的根节点，路径压缩

    int get_minpdd(int x) { return p->getweight(pddroot[findroot(x)]); }

    void mergedsu(int x, int y) // 合并x和y所在的树，如果x和y已经在同一棵树上则不合并，节点数少的树合并到节点数多的树上
    {
        x = findroot(x), y = findroot(y);
        if (x == y)
            return;
        if (dsusize[x] < dsusize[y])
            swap(x, y);
        father[y] = x; // y < x
        dsusize[x] += dsusize[y];
        while (pddsize[x] + pddsize[y] > g_k)
        {
            if (p->getweight(pddroot[x]) < p->getweight(pddroot[y]))
            {
                pddroot[x] = p->delete_minpdd(pddroot[x]);
                pddsize[x]--;
            }
            else
            {
                pddroot[y] = p->delete_minpdd(pddroot[y]);
                pddsize[y]--;
            }
        }
        pddroot[x] = p->meldpdd(pddroot[x], pddroot[y]);
        pddsize[x] += pddsize[y];
    }
};

int main()
{
    scanf("%d %d %d %d", &g_n, &g_m, &g_k, &g_q);

    dsu d(g_n + 1);

    for (int i = 1; i <= g_n; ++i)
    {
        int w;
        scanf("%d", &w);
        d.p->nodes[i].weight = w;
        d.pddroot[i] = i;
    }

    for (int i = 0; i < g_m; ++i)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        d.mergedsu(u, v);
    }

    for (int i = 0; i < g_q; ++i)
    {
        int op;
        scanf("%d", &op);
        if (op == 1)
        {
            int u, v;
            scanf("%d %d", &u, &v);
            d.mergedsu(u, v);
        }
        else if (op == 2)
        {
            int u;
            scanf("%d", &u);
            d.get_size(u) < g_k ? printf("-1\n") : printf("%d\n", d.get_minpdd(u));
        }
    }

    return 0;
}