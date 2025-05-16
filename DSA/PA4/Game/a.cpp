#include <stdio.h>
#include <cstring>

const int MAXN = 100000;
const int MAXM = 100000;
const int MOD1 = 1000000007;

struct Edge
{
    int to, next;
} edges[2 * MAXM];

int edge_count = 0;
int head[MAXN + 1];
int t[MAXN + 1];
int dist[MAXN + 1];
int ways[MAXN + 1];
bool visited[MAXN + 1];
int queue[MAXN + 1];

void add_edge(int u, int v)
{
    edges[edge_count] = {v, head[u]};
    head[u] = edge_count++;
}

void dijkstra(int n)
{
    memset(dist, 0x3f, sizeof(dist));
    memset(ways, 0, sizeof(ways));
    memset(visited, 0, sizeof(visited));

    dist[1] = t[1];
    ways[1] = 1;

    int q_start = 0, q_end = 0;
    queue[q_end++] = 1;

    while (q_start != q_end)
    {
        int u = -1, min_dist = 0x3f3f3f3f;

        for (int i = q_start; i < q_end; ++i)
        {
            if (!visited[queue[i]] && dist[queue[i]] < min_dist)
            {
                min_dist = dist[queue[i]];
                u = queue[i];
            }
        }

        if (u == -1)
            break;

        visited[u] = true;

        for (int i = head[u]; i != -1; i = edges[i].next)
        {
            int v = edges[i].to;
            int new_dist = dist[u] + t[v];

            if (new_dist < dist[v])
            {
                dist[v] = new_dist;
                ways[v] = ways[u];
                queue[q_end++] = v;
            }
            else if (new_dist == dist[v])
                ways[v] = (ways[v] + ways[u]) % MOD1;
        }
    }
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);

    memset(head, -1, sizeof(head));

    for (int i = 1; i <= n; ++i)
        scanf("%d", &t[i]);

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        add_edge(u, v);
        add_edge(v, u);
    }

    dijkstra(n);

    printf("%d\n", dist[n]);
    printf("%d\n", ways[n]);

    return 0;
}