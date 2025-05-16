#include <stdio.h>
#include <limits.h>

#define MAX_NODES 101
#define MAX_EDGES 101
#define INF INT_MAX

typedef struct
{
    int u, v, delay; // 边的两个节点和传播延迟
} Edge;

int router_delay[MAX_NODES];
Edge edges[MAX_EDGES];

int distance_vector_routing(int n, int m, int source, int to)
{
    // 初始化距离向量
    int distances[MAX_NODES];

    for (int i = 0; i <= n; i++)
        distances[i] = INF;
    distances[source] = 0;

    // 迭代 N-1 次
    int cnt = n - 1;
    while (cnt--)
    {
        int update_flag = 0;
        for (int j = 0; j < m; j++)
        {
            int u = edges[j].u;
            int v = edges[j].v;
            int delay = edges[j].delay;

            // 更新从 x 到 y 的距离
            if (distances[u] != INF && distances[u] + router_delay[u] + delay < distances[v])
            {
                distances[v] = distances[u] + router_delay[u] + delay;
                update_flag = 1;
            }

            // 更新从 y 到 x 的距离
            if (distances[v] != INF && distances[v] + router_delay[v] + delay < distances[u])
            {
                distances[u] = distances[v] + router_delay[v] + delay;
                update_flag = 1;
            }
        }
        if (!update_flag)
            break;
    }

    return distances[to];
}

int main()
{
    freopen("DV.in", "r", stdin);
    freopen("DV.out", "w", stdout);

    int n, m, source, to;

    scanf("%d %d", &n, &m);

    for (int i = 1; i <= n; i++)
    {
        int p, q, t;
        scanf("%d %d %d", &p, &q, &t);
        router_delay[i] = p + q + t;
    }

    for (int i = 0; i < m; i++)
    {
        Edge *e = &edges[i];
        scanf("%d %d %d", &e->u, &e->v, &e->delay);
    }

    scanf("%d %d", &source, &to);

    int result = distance_vector_routing(n, m, source, to);

    printf("%d\n", result);

    return 0;
}