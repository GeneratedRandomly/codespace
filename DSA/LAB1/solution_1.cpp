#include <cstdio>

int matrix[2001][2001];

int main() {
    int n, m, q;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            scanf("%d", &matrix[i][j]);
        }
    }
    scanf("%d", &q);
    int sum = 0;
    for (int i = 1; i <= q; ++i) {
        sum = 0;
        int x, y, a, b;
        scanf("%d %d %d %d", &x, &y, &a, &b);
        for (int j = 0; j < a; ++j) {
            for (int k = 0; k < b; ++k) {
                sum += matrix[x + j][y + k];
            }
        }
        printf("%d\n", sum);
    }
    return 0;
}