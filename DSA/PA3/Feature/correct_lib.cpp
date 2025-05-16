#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
#include "correct.h" // 确保包含了函数声明

using namespace std;

const int N = 500010;

int main() {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) {
        return 1; // 处理错误
    }

    int *w = new int[n + 1];
    int *x = new int[n + 1];

    for (int i = 1; i <= n; i++) {
        if (scanf("%d%d", &w[i], &x[i]) != 2) {
            return 1; // 处理错误
        }
        assert(-1e9 <= w[i] && w[i] <= 1e9);
        assert(-1e3 <= x[i] && x[i] <= 1e3);
    }
    init(n, w, x);
    memset(w, 0, n * sizeof(int));
    memset(x, 0, n * sizeof(int));
    delete[] w;
    delete[] x;

    int cur = 0;
    for (int i = 0; i < m; i++) {
        char s[10];
        if (scanf("%s", s) != 1) {
            return 1; // 处理错误
        }
        if (s[0] == 'W') {
            int i, w;
            if (scanf("%d%d", &i, &w) != 2) {
                return 1; // 处理错误
            }
            modify_weight(i, w);
        } else if (s[0] == 'V') {
            int k, x;
            if (scanf("%d%d", &k, &x) != 2) {
                return 1; // 处理错误
            }
            modify_value(k, x);
        } else if (s[0] == 'C') {
            int k;
            if (scanf("%d", &k) != 1) {
                return 1; // 处理错误
            }
            printf("%lld\n", calculate(k));
        }
    }

    return 0;
}