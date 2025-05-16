#include <stdio.h>
#include <cstring>

using namespace std;

#define ull unsigned long long

const ull BASE1_1 = 131;
const ull INVBASE1_1 = 281629680514649643ULL;
const ull BASE1_2 = 179;
const ull INVBASE1_2 = 17107036403551874683ULL;

const ull BASE2_1 = 0;
const ull INVBASE2_1 = 0;
const ull BASE2_2 = 0;
const ull INVBASE2_2 = 0;

char **T; // 主矩阵
char **P; // 目标矩阵

ull **T_hashed1;
ull **T_hashed_prefix1;
ull **T_hashed2;
ull **T_hashed_prefix2;
ull *hash_bucket1;
ull *hash_bucket2;

int M, N, g_m, g_n, g_k, cnt;
int result = 0; // 目标矩阵的匹配结果

ull *base1_1;    // 用于存储BASE1_1^m的值
ull *base1_2;    // 用于存储BASE1_2^m的值
ull *invbase1_1; // 用于存储INVBASE1_1^m的值
ull *invbase1_2; // 用于存储INVBASE1_2^m的值

ull *base2_1;    // 类似以上，下同
ull *base2_2;
ull *invbase2_1;
ull *invbase2_2;

ull hardComputeHash1() // 计算目标矩阵的哈希值
{
    ull hashValue = 0;
    for (int i = 0; i < g_m; ++i)
        for (int j = 0; j < g_n; ++j)
            hashValue += base1_1[i] * base1_2[j] * P[i][j];
    return hashValue;
}

ull hardComputeHash2() // 计算目标矩阵的哈希值
{
    ull hashValue = 0;
    for (int i = 0; i < g_m; ++i)
        for (int j = 0; j < g_n; ++j)
            hashValue += base2_1[i] * base2_2[j] * P[i][j];
    return hashValue;
}

ull query1(int a, int b, int c, int d)
{
    ull hashValue = T_hashed_prefix1[c][d] - T_hashed_prefix1[a][d] - T_hashed_prefix1[c][b] + T_hashed_prefix1[a][b];
    return (invbase1_1[a] * invbase1_2[b] * hashValue);
}

ull query2(int a, int b, int c, int d)
{
    ull hashValue = T_hashed_prefix2[c][d] - T_hashed_prefix2[a][d] - T_hashed_prefix2[c][b] + T_hashed_prefix2[a][b];
    return (invbase2_1[a] * invbase2_2[b] * hashValue);
}

void setbase1()
{
    base1_1 = new ull[M + 1]; // base1_1[i] = BASE1_1^i
    base1_2 = new ull[N + 1]; // base1_2[i] = BASE1_2^i
    base1_1[0] = 1;
    base1_2[0] = 1;
    for (int i = 1; i <= M; ++i)
        base1_1[i] = base1_1[i - 1] * BASE1_1;
    for (int i = 1; i <= N; ++i)
        base1_2[i] = base1_2[i - 1] * BASE1_2;

    invbase1_1 = new ull[M + 1]; // base1_1[i] = BASE1_1^-i
    invbase1_2 = new ull[N + 1]; // base1_2[i] = BASE1_2^-i
    invbase1_1[0] = 1;
    invbase1_2[0] = 1;
    for (int i = 1; i <= M; ++i)
        invbase1_1[i] = invbase1_1[i - 1] * INVBASE1_1;
    for (int i = 1; i <= N; ++i)
        invbase1_2[i] = invbase1_2[i - 1] * INVBASE1_2;
}

void setbase2()
{
    base2_1 = new ull[M + 1]; // base2_1[i] = BASE2_1^i
    base2_2 = new ull[N + 1]; // base2_2[i] = BASE2_2^i
    base2_1[0] = 1;
    base2_2[0] = 1;
    for (int i = 1; i <= M; ++i)
        base2_1[i] = base2_1[i - 1] * BASE2_1;
    for (int i = 1; i <= N; ++i)
        base2_2[i] = base2_2[i - 1] * BASE2_2;

    invbase2_1 = new ull[M + 1]; // base2_1[i] = BASE2_1^-i
    invbase2_2 = new ull[N + 1]; // base2_2[i] = BASE2_2^-i
    invbase2_1[0] = 1;
    invbase2_2[0] = 1;
    for (int i = 1; i <= M; ++i)
        invbase2_1[i] = invbase2_1[i - 1] * INVBASE2_1;
    for (int i = 1; i <= N; ++i)
        invbase2_2[i] = invbase2_2[i - 1] * INVBASE2_2;
}

void setTHash1()
{
    T_hashed1 = new ull *[M];
    T_hashed_prefix1 = new ull *[M + 1];
    for (int i = 0; i < M; ++i)
        T_hashed1[i] = new ull[N];
    for (int i = 0; i <= M; ++i)
    {
        T_hashed_prefix1[i] = new ull[N + 1];
        memset(T_hashed_prefix1[i], 0, sizeof(ull) * (N + 1));
    }
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            T_hashed1[i][j] = base1_1[i] * base1_2[j] * T[i][j];
    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j)
            T_hashed_prefix1[i][j] = T_hashed_prefix1[i - 1][j] +
                                     T_hashed_prefix1[i][j - 1] -
                                     T_hashed_prefix1[i - 1][j - 1] +
                                     T_hashed1[i - 1][j - 1];
}

void setTHash2()
{
    T_hashed2 = new ull *[M];
    T_hashed_prefix2 = new ull *[M + 1];
    for (int i = 0; i < M; ++i)
        T_hashed2[i] = new ull[N];
    for (int i = 0; i <= M; ++i)
    {
        T_hashed_prefix2[i] = new ull[N + 1];
        memset(T_hashed_prefix2[i], 0, sizeof(ull) * (N + 1));
    }
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            T_hashed2[i][j] = base2_1[i] * base2_2[j] * T[i][j];

    for (int i = 1; i <= M; ++i)
        for (int j = 1; j <= N; ++j)
            T_hashed_prefix2[i][j] = T_hashed_prefix2[i - 1][j] +
                                     T_hashed_prefix2[i][j - 1] -
                                     T_hashed_prefix2[i - 1][j - 1] +
                                     T_hashed2[i - 1][j - 1];
}

void setHashBucket1()
{
    hash_bucket1 = new ull[(M - g_m + 1) * (N - g_n + 1)];
    for (int i = 0; i < M - g_m + 1; ++i)
        for (int j = 0; j < N - g_n + 1; ++j)
            hash_bucket1[cnt++] = query1(i, j, i + g_m, j + g_n);
}

void setHashBucket2()
{
    hash_bucket2 = new ull[(M - g_m + 1) * (N - g_n + 1)];
    for (int i = 0; i < M - g_m + 1; ++i)
        for (int j = 0; j < N - g_n + 1; ++j)
            hash_bucket2[cnt++] = query2(i, j, i + g_m, j + g_n);
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

    setbase1();
    setbase2();

    setTHash1();
    setTHash2();

    setHashBucket1();
    setHashBucket2();
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

        ull targetHash1 = hardComputeHash1();
        ull targetHash2 = hardComputeHash2();
        for (int i = 0; i < cnt; ++i)
            if (targetHash1 == hash_bucket1[i] && targetHash2 == hash_bucket2[i])
                result++;
        printf("%d\n", result);
        result = 0;
    }
    return 0;
}