#include <stdio.h>

int rangeleft, rangeright, g_n, g_m, idx = 1, root = 1;

struct treenode
{
    int lc, rc, lazy;
    long long value = 0;
} node[10700000];

void pushdown(int i, int nodestart, int nodeend)
{
    treenode &cur = node[i];
    if (cur.lazy)
    {
        int middle = nodestart + ((nodeend - nodestart) >> 1);
        int &lc = cur.lc, &rc = cur.rc;
        if (!lc)
            lc = ++idx;
        if (!rc)
            rc = ++idx;
        node[lc].value += cur.lazy * (middle - nodestart);
        node[lc].lazy += cur.lazy;
        node[rc].value += cur.lazy * (nodeend - middle);
        node[rc].lazy += cur.lazy;
        cur.lazy = 0;
    }
}

void increase(int &i /*can assign a value to father's lc & rc*/, int nodestart, int nodeend) //[nodestart, nodeend)
{
    if (!i)
        i = ++idx;
    treenode &cur = node[i];
    if (rangeleft <= nodestart && nodeend <= rangeright) // subrange
    {
        cur.value += nodeend - nodestart;
        cur.lazy++;
        return;
    }
    int middle = nodestart + ((nodeend - nodestart) >> 1);
    pushdown(i, nodestart, nodeend);
    int &lc = cur.lc, &rc = cur.rc;
    if (rangeleft < middle) // lc intersection
        increase(lc, nodestart, middle);
    if (rangeright > middle) // rc intersection
        increase(rc, middle, nodeend);
    cur.value = node[lc].value + node[rc].value;
}

long long queryRange(int i, int nodestart, int nodeend)
{
    if (!i)
        return 0;
    if (rangeleft <= nodestart && nodeend <= rangeright) // subrange
        return node[i].value;
    int middle = nodestart + ((nodeend - nodestart) >> 1);
    pushdown(i, nodestart, nodeend);
    long long sum = 0;
    if (rangeleft < middle) // lc intersection
        sum += queryRange(node[i].lc, nodestart, middle);
    if (rangeright > middle) // rc intersection
        sum += queryRange(node[i].rc, middle, nodeend);
    return sum;
}

int main()
{
    scanf("%d %d", &g_n, &g_m);
    while (g_m--)
    {
        char op;
        int s, t;
        scanf(" %c %d %d", &op, &s, &t);
        rangeleft = s;
        rangeright = t + 1;
        if (op == 'H')
            increase(root, 1, g_n + 1);
        else
            printf("%lld\n", queryRange(root, 1, g_n + 1));
    }
    return 0;
}