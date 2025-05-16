#include "feature.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cstring>

#define debug(...) fprintf(stderr, __VA_ARGS__)

const int N = 500010;

int main()
{
    int n, m;
    if(scanf("%d%d", &n, &m) != 2)
        return 1;
    assert(1 <= n && n <= 500000);
    assert(1 <= m && m <= 500000);
    int *w = new int[n + 1];
    int *x = new int[n + 1];
    for (int i = 1; i <= n; i++)
    {
        if(scanf("%d%d", &w[i], &x[i]) != 2)
            return 1;
        assert(-1e9 <= w[i] && w[i] <= 1e9);
        assert(-1e3 <= x[i] && x[i] <= 1e3);
    }
    init(n, w, x);
    memset(w, 0, n * sizeof(int));
    memset(x, 0, n * sizeof(int));
    delete[] w;
    delete[] x;

    int cur = 0;
    for (int i = 0; i < m; i++)
    {
        char s[10];
        if(scanf("%s", s) != 1)
            return 1;
        assert(s[0] == 'W' || s[0] == 'V' || s[0] == 'C');
        if (s[0] == 'W')
        {
            int i, x;
            if(scanf("%d%d", &i, &x) != 2)
                return 1;
            assert(1 <= i && i <= n);
            assert(-1e9 <= x && x <= 1e9);
            modify_weight(i, x);
        }
        if (s[0] == 'V')
        {
            int k, x;
            if(scanf("%d%d", &k, &x) != 2)
                return 1;
            assert(1 <= k && k <= n);
            assert(-1e3 <= x && x <= 1e3);
            modify_value(k, x);
        }
        if (s[0] == 'C')
        {
            int k;
            if(scanf("%d", &k) != 1)
                return 1;
            assert(1 <= k && k <= n);
            long long ans = calculate(k);
            printf("%lld\n", ans);
        }
    }
}