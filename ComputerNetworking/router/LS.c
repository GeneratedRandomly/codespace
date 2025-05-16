#include <stdio.h>
#include <limits.h>

#define MAX_NODES 101
#define INF INT_MAX

int graph[MAX_NODES][MAX_NODES];
int router_delay[MAX_NODES];
int distances[MAX_NODES];

void dijkstra(int source, int n)
{
    int visited[MAX_NODES] = {0};

    // 初始化距离数组
    for (int i = 1; i <= n; i++)
        distances[i] = INF;
    distances[source] = 0;

    for (int count = 1; count <= n; count++)
    {
        int min_distance = INF, u = -1;

        // 找到未访问节点中距离最小的节点
        for (int i = 1; i <= n; i++)
        {
            if (!visited[i] && distances[i] < min_distance)
            {
                min_distance = distances[i];
                u = i;
            }
        }

        if (u == -1)
            break; // 所有可达节点已访问

        visited[u] = 1;

        // 更新邻居节点的距离
        for (int v = 1; v <= n; v++)
        {
            if (!visited[v] && graph[u][v] != INF)
            {
                int new_distance = distances[u] + graph[u][v];
                if (new_distance < distances[v])
                    distances[v] = new_distance;
            }
        }
    }
}

int main()
{
    freopen("LS.in", "r", stdin);
    freopen("LS.out", "w", stdout);

    int n, m;
    scanf("%d %d", &n, &m);

    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            graph[i][j] = INF;
        }
    }

    for (int i = 1; i <= n; i++)
    {
        int p, q, t;
        scanf("%d %d %d", &p, &q, &t);
        router_delay[i] = p + q + t;
    }

    for (int i = 1; i <= m; i++)
    {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);
        graph[x][y] = z;
        graph[y][x] = z;
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (graph[i][j] != INF)
                graph[i][j] += router_delay[i];
        }
    }

    for (int i = 0; i <= n; i++)
        graph[i][i] = 0;

    int source;
    scanf("%d", &source);

    dijkstra(source, n);

    for (int i = 1; i <= n; i++)
    {
        if (i == source)
            printf("0\n");
        else
            printf("%d\n", distances[i]);
    }

    return 0;
}