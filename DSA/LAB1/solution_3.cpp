#include <cstdio>

int matrix[2001][2001];
int sum[2001][2001];

int main() {
    int n, m, q;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            scanf("%d", &matrix[i][j]);
            sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + matrix[i][j];
        }
    }
    scanf("%d", &q);
    int sub_sum = 0;
    for (int i = 1; i <= q; ++i) {
        sub_sum = 0;
        int x, y, a, b;
        scanf("%d %d %d %d", &x, &y, &a, &b);
        sub_sum = sum[x+a-1][y+b-1] - sum[x-1][y+b-1] - sum[x+a-1][y-1] + sum[x-1][y-1];
        printf("%d\n", sub_sum);
    }
    return 0;
}