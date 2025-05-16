#include <cstdio>
#include <cstring>
#include <cstdlib>

// 为字符串分配空间
char *str_a = (char *)malloc(501000 * sizeof(char));
char *str_b = (char *)malloc(501000 * sizeof(char));
int a_length, b_length, K;
//直接在堆上开辟2*501000大小的数组，防止爆栈
int lcs_dp[2][501000];
// 准备求最大、最小值函数
int max(int a, int b)
{
    return a > b ? a : b;
}
int min(int a, int b)
{
    return a < b ? a : b;
}
// 给定x时，求xa=x时，xb有意义的范围，超出此范围则时间成本超过K
int max_bound(int x)
{
    return min(b_length - 1, x + K + 1);
}
int min_bound(int x)
{
    return max(0, x - K - 1);
}
//交换a、b字符串
void swapab()
{
    int tmp = a_length;
    a_length = b_length;
    b_length = tmp;
    char *c_tmp = str_a;
    str_a = str_b;
    str_b = c_tmp;
}

int main()
{
    // 读取数据
    scanf("%d %d %d", &a_length, &b_length, &K);
    scanf("%s", str_a);
    scanf("%s", str_b);
    // 对比a和b的字符串长度，若a比b长，则交换两个字符串，保证a为较短字符串，减少时间复杂度
    if (a_length > b_length)
    {
        swapab();
    }
    //若a与b长度之差大于K，则时间成本一定大于K，直接返回-1
    if (b_length - a_length > K)
    {
        printf("%d\n", -1);
        return 0;
    }
    memset(lcs_dp, 0, sizeof(lcs_dp));
    //初始化第一排元素，若有b有一个元素与a[0]相同，则之后的最长公共子序列长度均为1
    for (int j = 0; j < b_length; j++)
    {
        if (str_b[j] == str_a[0])
        {
            for (int i = j; i < b_length; i++)
            {
                lcs_dp[0][i] = 1;
            }
            break;
        }
        else
        {
            lcs_dp[0][j] = 0;
        }
    }
    //prev表示前一排，curr表示正在运算的当前排
    int prev = 1;
    int curr = 0;
    for (int xa = 1; xa < a_length; xa++)
    {
        //当前排运算完成后变为前一排
        prev = !prev;
        curr = !curr;
        //求关于xa的有意义的运算范围
        int maxnum = max_bound(xa);
        int minnum = min_bound(xa);
        for (int xb = minnum; xb <= maxnum; xb++)
        {
            if (xb == 0)
            {
                //对xb = 0的情况特殊处理
                lcs_dp[curr][xb] = (lcs_dp[prev][0] || (str_a[xa] == str_b[0]));
                continue;
            }
            //使用状态转移方程由之前结果得到当前结果
            if (str_a[xa] == str_b[xb])
            {
                lcs_dp[curr][xb] = lcs_dp[prev][xb - 1] + 1;
            }
            else
            {
                lcs_dp[curr][xb] = max(lcs_dp[prev][xb], lcs_dp[curr][xb - 1]);
            }
        }
    }
    //求时间成本
    int step = a_length + b_length - 2 * lcs_dp[curr][b_length - 1];
    //时间成本与K比较并输出
    printf("%d\n", step <= K ? step : -1);
    return 0;
}