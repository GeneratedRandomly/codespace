#include <stdio.h>
#include <cstring>

const int MOD1 = 1e9 + 7;
const int MAXN = 100000;
const int M = 100000;

struct pddNode
{
    int time;
    int index;
};

void swap(pddNode &a, pddNode &b)
{
    pddNode temp = a;
    a = b;
    b = temp;
}

class vector
{
    int *data;
    int size;
    int capacity;

public:
    vector() : size(0), capacity(1) { data = new int[1]; }

    void expand()
    {
        capacity <<= 1;
        int *new_data = new int[capacity];
        for (int i = 0; i < size; ++i)
            new_data[i] = data[i];
        delete[] data;
        data = new_data;
    }

    void push_back(int x)
    {
        if (size == capacity)
            expand();
        data[size++] = x;
    }

    int &operator[](int i) { return data[i]; }

    int *begin() { return data; }

    int *end() { return data + size; }
};

class PriorityQueue
{
    pddNode *data;
    int size;
    int capacity;
    void heapify_up(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (data[index].time >= data[parent].time)
                break;
            swap(data[index], data[parent]);
            index = parent;
        }
    }
    void heapify_down(int index)
    {
        while (2 * index + 1 < size)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = left;
            if (right < size && data[right].time < data[left].time)
                smallest = right;
            if (data[index].time <= data[smallest].time)
                break;
            swap(data[index], data[smallest]);
            index = smallest;
        }
    }

public:
    PriorityQueue(int cap) : size(0), capacity(cap) { data = new pddNode[capacity]; }
    void push(pddNode element)
    {
        if (size == capacity)
        {
            capacity <<= 1;
            pddNode *new_data = new pddNode[capacity];
            for (int i = 0; i < size; ++i)
                new_data[i] = data[i];
            delete[] data;
            data = new_data;
        }
        data[size] = element;
        heapify_up(size);
        size++;
    }
    pddNode pop()
    {
        pddNode top = data[0];
        data[0] = data[--size];
        heapify_down(0);
        return top;
    }
    bool empty() { return size == 0; }
};

int t[MAXN + 1];
vector adj[MAXN + 1];
int min_time[MAXN + 1];
int ways[MAXN + 1];
bool visited[MAXN + 1];
PriorityQueue q(M + 10);

int main()
{
    int N, M;
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; ++i)
        scanf("%d", &t[i]);

    while (M--)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= N; ++i)
    {
        min_time[i] = 0x3f3f3f3f;
        ways[i] = 0;
        visited[i] = false;
    }

    min_time[1] = t[1];
    ways[1] = 1;
    q.push({t[1], 1});

    while (!q.empty())
    {
        pddNode node = q.pop();
        int u = node.index;
        if (visited[u])
            continue;
        visited[u] = true;
        for (int v : adj[u])
        {
            if (min_time[u] + t[v] < min_time[v])
            {
                min_time[v] = min_time[u] + t[v];
                ways[v] = ways[u];
                q.push({min_time[v], v});
            }
            else if (min_time[u] + t[v] == min_time[v])
                ways[v] = (ways[v] + ways[u]) % MOD1;
        }
    }

    printf("%d\n", min_time[N]);
    printf("%d\n", ways[N]);

    return 0;
}