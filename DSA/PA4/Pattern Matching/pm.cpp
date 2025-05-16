#include <stdio.h>
#include <cstring>
#include <climits>
#define ll long long

using namespace std;

const ll MOD = 100000000000001219LL;
const ll BASE1 = 1319;
const ll INVBASE1 = 64139499620925725LL;
const ll BASE2 = 100493;
const ll INVBASE2 = 34835262157563638LL;

char **T; // 主矩阵
char **P; // 目标矩阵

ll **T_hashed1;
ll **T_hashed_prefix1;
ll *hash_bucket;

int M, N, g_m, g_n, g_k, cnt;
ll result = 0; // 目标矩阵的匹配结果

ll *base1;    // 用于存储BASE^m的值
ll *base2;    // 用于存储BASE^m的值
ll *invbase1; // 用于存储BASE^m的值
ll *invbase2; // 用于存储BASE^m的值

ll add(ll a, ll b) { return (a + b) % MOD; }

ll mul(ll a, ll b) { return (__int128_t(a) * b) % MOD; }

ll hardComputeHash() // 计算目标矩阵的哈希值
{
    ll hashValue = 0;
    for (int i = 0; i < g_m; ++i)
        for (int j = 0; j < g_n; ++j)
            hashValue = add(mul(mul(base1[i], base2[j]), P[i][j]), hashValue);
    return hashValue;
}

ll query(int a, int b, int c, int d)
{
    ll hashValue = add(add(add(T_hashed_prefix1[c][d],
                               MOD - T_hashed_prefix1[a][d]),
                           MOD - T_hashed_prefix1[c][b]),
                       T_hashed_prefix1[a][b]);
    return mul(mul(invbase1[a], invbase2[b]), hashValue);
}

void init()
{
    scanf("%d %d %d %d %d", &M, &N, &g_m, &g_n, &g_k);
    T = new char *[M];
    for (int i = 0; i < M; ++i)
    {
        T[i] = new char[N + 1];
        scanf("%s", T[i]);
    }

    P = new char *[g_m];
    for (int i = 0; i < g_m; ++i)
        P[i] = new char[g_n + 1];

    base1 = new ll[M + 1]; // base1[i] = BASE1^i
    base2 = new ll[N + 1]; // base2[i] = BASE2^i
    base1[0] = 1;
    base2[0] = 1;
    for (int i = 1; i <= M; ++i)
        base1[i] = mul(base1[i - 1], BASE1);
    for (int i = 1; i <= N; ++i)
        base2[i] = mul(base2[i - 1], BASE2);

    invbase1 = new ll[M + 1]; // base1[i] = BASE1^-i
    invbase2 = new ll[N + 1]; // base2[i] = BASE2^-i
    invbase1[0] = 1;
    invbase2[0] = 1;
    for (int i = 1; i <= M; ++i)
        invbase1[i] = mul(invbase1[i - 1], INVBASE1);
    for (int i = 1; i <= N; ++i)
        invbase2[i] = mul(invbase2[i - 1], INVBASE2);

    T_hashed1 = new ll *[M];
    T_hashed_prefix1 = new ll *[M + 1];
    for (int i = 0; i < M; ++i)
        T_hashed1[i] = new ll[N];
    for (int i = 0; i <= M; ++i)
    {
        T_hashed_prefix1[i] = new ll[N + 1];
        memset(T_hashed_prefix1[i], 0, sizeof(int) * (N + 1));
    }

    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            T_hashed1[i][j] = mul(mul(base1[i], base2[j]), T[i][j]);

    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j)
            T_hashed_prefix1[i][j] = add(add(add(T_hashed_prefix1[i - 1][j],
                                                 T_hashed_prefix1[i][j - 1]),
                                             MOD - T_hashed_prefix1[i - 1][j - 1]),
                                         T_hashed1[i - 1][j - 1]);

    hash_bucket = new ll[(M - g_m + 1) * (N - g_n + 1)];
    for (int i = 0; i < M - g_m + 1; ++i)
        for (int j = 0; j < N - g_n + 1; ++j)
            hash_bucket[cnt++] = query(i, j, i + g_m, j + g_n);
}

void readPattern()
{
    for (int i = 0; i < g_m; ++i)
        scanf("%s", P[i]);
}

int main()
{
    init();

    while (g_k--)
    {
        readPattern();

        ll targetHash1 = hardComputeHash();
        for (int i = 0; i < cnt; ++i)
            if (targetHash1 == hash_bucket[i])
                result++;
        printf("%lld\n", result);
        result = 0;
    }
    return 0;
}