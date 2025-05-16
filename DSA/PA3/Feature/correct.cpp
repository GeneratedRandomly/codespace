#include <iostream>
#include <vector>
#include <algorithm>
#include "correct.h"

using namespace std;

vector<int> w, x;

void init(int n, const int *weights, const int *values) {
    w.assign(weights + 1, weights + n + 1);
    x.assign(values + 1, values + n + 1);
}

void modify_weight(int i, int new_w) {
    w[i-1] = new_w;
}

void modify_value(int k, int new_x) {
    vector<pair<int, int>> wx;
    for (int i = 0; i < w.size(); ++i) {
        wx.emplace_back(w[i], i);
    }
    sort(wx.rbegin(), wx.rend());
    x[wx[k - 1].second] = new_x;
}

long long calculate(int k) {
    vector<pair<int, int>> wx;
    for (int i = 0; i < w.size(); ++i) {
        wx.emplace_back(abs(w[i]), i);
    }
    sort(wx.rbegin(), wx.rend());
    long long result = 0;
    for (int i = 0; i < k; ++i) {
        int idx = wx[i].second;
        result += (long long)w[idx] * x[idx];
    }
    return result;
}

// int main()
// {
//     int n, m;
//     scanf("%d%d", &n, &m);
//     int *ww = new int[n + 1];
//     int *xx = new int[n + 1];
//     for (int i = 1; i <= n; i++)
//         scanf("%d%d", &ww[i], &xx[i]);
//     init(n, ww, xx);
//     delete[] ww;
//     delete[] xx;

//     int cur = 0;
//     for (int i = 0; i < m; i++)
//     {
//         char s[10];
//         scanf("%s", s);
//         if (s[0] == 'W')
//         {
//             int i, x;
//             scanf("%d%d", &i, &x);
//             modify_weight(i, x);
//         }
//         if (s[0] == 'V')
//         {
//             int k, x;
//             scanf("%d%d", &k, &x);
//             modify_value(k, x);
//         }
//         if (s[0] == 'C')
//         {
//             int k;
//             scanf("%d", &k);
//             long long ans = calculate(k);
//             printf("%lld\n", ans);
//         }
//     }
// }