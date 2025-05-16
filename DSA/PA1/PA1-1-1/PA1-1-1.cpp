#include <stdio.h>
int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b, int c) { return a > b ? max(a, c) : max(b, c); }

int main()
{
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    char* A = new char[n];
    char* B = new char[m];
    scanf("%s %s", A, B);
    int dp[2 * k + 1];

    // dp[] = real_dp[i][dp_left], dp[i][dp_left + 1], ..., dp[i][dp_right].

    if (n - m > k || m - n > k || m == 0 || n == 0)
    {
        printf("%d\n", -1);
        return 0;
    }

    for (size_t i = 0; i < 2 * k + 1; i++)
        dp[i] = 0;

    int i = 1;

    // from 解题方法
    int dp_left = max(i - k, 0);
    int dp_right = min(i + k, m);

    while (i <= n)
    {
        dp_left = max(i - k, 0);
        dp_right = min(i + k, m);

        if (i <= k)
        {
            int reserve_1 = dp[0]; // dp[i-1][j-1]
            int reserve_2 = dp[1]; // dp[i-1][j]
            for (size_t j = 1; j < dp_right - dp_left + 1; j++)
            {
                reserve_2 = dp[j];
                dp[j] = max(reserve_1 + (A[i - 1] == B[j - 1]), dp[j - 1], reserve_2);
                reserve_1 = reserve_2;
            }
        }

        else
        {                          // each i, we move rightward 1.
            int reserve_1 = dp[0]; // dp[i-1][j-1]
            int reserve_2 = dp[1]; // dp[i-1][j]
            // j == 0;
            // add a rightward bias dp_left as B[0] in fact is B[dp_left] in this case. followings are the same.
            dp[0] = max(reserve_1 + (A[i - 1] == B[0 + dp_left - 1]), -1, reserve_2);
            reserve_1 = reserve_2;
            for (size_t j = 1; j < dp_right - dp_left; j++)
            {
                reserve_2 = dp[j + 1];
                dp[j] = max(reserve_1 + (A[i - 1] == B[j + dp_left - 1]), dp[j - 1], reserve_2);
                reserve_1 = reserve_2;
            }
            int end = dp_right - dp_left;
            dp[end] = max(reserve_1 + (A[i - 1] == B[end + dp_left - 1]), dp[end - 1], -1);
        }

        i++;
    }

    printf("%d\n", n + m - 2 * dp[m - dp_left] > k ? -1 : n + m - 2 * dp[m - dp_left]);

    return 0;
}