#include <stdio.h>
#include <cstring>

using namespace std;

const int MOD1 = 1000000241;
const int MOD2 = 1000000933;
const int BASE1 = 31;
const int BASE2 = 37;

char **T; // 主矩阵
int M, N, g_m, g_n, g_k;
char **P;       // 目标矩阵
int **S1;       // 子矩阵的第一哈希值，表示T(i,j)开始的m*n矩阵的哈希值
int **S2;       // 子矩阵的第二哈希值，表示T(i,j)开始的m*n矩阵的哈希值
int result = 0; // 目标矩阵的匹配结果

int computeHash(char **matrix, int a, int b, int MOD, int BASE) // 从T(a,b)开始的m*n矩阵的哈希值
{
    int hashValue = 0;
    for (int i = a; i < a + g_m; ++i)
    {
        for (int j = b; j < b + g_n; ++j)
            hashValue = (hashValue * BASE + matrix[i][j]) % MOD;
    }
    return hashValue;
}

int main()
{
    scanf("%d %d %d %d %d", &M, &N, &g_m, &g_n, &g_k);

    T = new char *[M];
    for (int i = 0; i < M; ++i)
    {
        T[i] = new char[N + 1];
        scanf("%s", T[i]);
    }

    S1 = new int *[M - g_m + 1];
    S2 = new int *[M - g_m + 1];
    for (int i = 0; i < M - g_m + 1; ++i)
    {
        S1[i] = new int[N - g_n + 1];
        S2[i] = new int[N - g_n + 1];
        for (int j = 0; j < N - g_n + 1; ++j)
        {
            S1[i][j] = computeHash(T, i, j, MOD1, BASE1);
            S2[i][j] = computeHash(T, i, j, MOD2, BASE2);
        }
    }

    P = new char *[g_m];
    for (int i = 0; i < g_m; ++i)
        P[i] = new char[g_n + 1];

    while (g_k--)
    {
        for (int i = 0; i < g_m; ++i)
            scanf("%s", P[i]);

        int targetHash1 = computeHash(P, 0, 0, MOD1, BASE1);
        int targetHash2 = computeHash(P, 0, 0, MOD2, BASE2);
        for (int i = 0; i < M - g_m + 1; ++i)
        {
            for (int j = 0; j < N - g_n + 1; ++j)
            {
                if (S1[i][j] == targetHash1 && S2[i][j] == targetHash2)
                    result++;
            }
        }
        printf("%d\n", result);
        result = 0;
    }
    return 0;
}