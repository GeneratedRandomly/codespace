#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n, m;
    cin >> n >> m;
    cout << n << " " << m << "\n";
    printf("1 2\n%d ", n - 2);
    for (int i = 3; i <= n; i++)
        printf("%d ", i);
    putchar('\n');
    for (int i = 3; i <= n; i++)
        printf("0\n");
    for (int i = 1; i <= m / 2; i++)
    {
        printf("0\n2 0 0\n0\n1\n");
        printf("0\n1 1\n1 0\n0\n");
    }
    return 0;
}