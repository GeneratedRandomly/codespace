#include <stdio.h>
#include <cstring>

using namespace std;

const int MOD = 1e9 + 7;
const int BASE = 31;
char **T; // 主矩阵
int M, N, g_m, g_n, g_k;
int **s;        // 子矩阵的哈希值，s[i][j]表示T(i,j)开始的m*n矩阵的哈希值
char **P;       // 目标矩阵
int result = 0; // 目标矩阵的匹配结果

int computeHash(char **matrix, int a = 0, int b = 0) // 从T(a,b)开始的m*n矩阵的哈希值
{
    int hashValue = 0;
    for (int i = a; i < a + g_m; ++i)
    {
        for (int j = b; j < b + g_n; ++j)
            hashValue = (hashValue * BASE + matrix[i][j]) % MOD;
    }
    return hashValue;
}

bool isMatch(int a, int b)
{
    for (int i = 0; i < g_m; ++i)
    {
        for (int j = 0; j < g_n; ++j)
        {
            if (P[i][j] != T[a + i][b + j])
                return false;
        }
    }
    return true;
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

    s = new int *[M - g_m + 1];
    for (int i = 0; i < M - g_m + 1; ++i)
    {
        s[i] = new int[N - g_n + 1];
        for (int j = 0; j < N - g_n + 1; ++j)
            s[i][j] = computeHash(T, i, j);
    }

    P = new char *[g_m];
    for (int i = 0; i < g_m; ++i)
        P[i] = new char[g_n + 1];

    while (g_k--)
    {
        for (int i = 0; i < g_m; ++i)
            scanf("%s", P[i]);

        int targetHash = computeHash(P);
        for (int i = 0; i < M - g_m + 1; ++i)
        {
            for (int j = 0; j < N - g_n + 1; ++j)
            {
                if (s[i][j] == targetHash)
                {
                    if (isMatch(i, j))
                        result++;
                }
            }
        }
        printf("%d\n", result);
        result = 0;
    }
    return 0;
}