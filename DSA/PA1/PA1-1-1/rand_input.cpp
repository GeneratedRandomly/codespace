#include <bits/stdc++.h>

int main()
{
    using namespace std;
    srand(time(0));
    int q = 1 + rand() % 500000;
    int k = 1 + rand() % 100;
    int n = q + rand() % k;
    int m = q + rand() % k;
    cout << n << ' ' << m << ' ' << k << '\n';

    std::string LCS, A, B;
    for (int i = 0; i < q; i++)
        LCS += (33 + rand() % 94);

    A = B = LCS;

    for (int i = 0; i < n - q; i++)
    {
        A.insert(A.begin() + rand() % A.size(), 33 + rand() % 37);
    }

    for (int i = 0; i < m - q; i++)
    {
        B.insert(B.begin() + rand() % B.size(), 33 + rand() % 37);
    }
    cout << A << '\n'
         << B << '\n';

    return 0;
}