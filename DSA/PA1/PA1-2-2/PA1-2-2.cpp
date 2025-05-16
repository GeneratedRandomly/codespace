#include <stdio.h>
void merge(int *arr, int l, int r)
{
    int m = (l + r) / 2;
    int *tmp = new int[r - l + 1];
    int i, j, k;
    i = j = k = 0;
    int n1 = m - l + 1;
    int n2 = r - m;
    while (i < n1 && j < n2)
    {
        if (arr[l + i] <= arr[m + 1 + j])
        {
            tmp[k] = arr[l + i];
            i++;
        }
        else
        {
            tmp[k] = arr[m + 1 + j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        tmp[k] = arr[l + i];
        i++;
        k++;
    }
    while (j < n2)
    {
        tmp[k] = arr[m + 1 + j];
        j++;
        k++;
    }
    for (int i = 0; i < r - l + 1; i++)
        arr[l + i] = tmp[i];
}
void sort(int *arr, int l, int r)
{
    if (l >= r)
        return;
    int m = (l + r) / 2;
    sort(arr, l, m);
    sort(arr, m + 1, r);
    merge(arr, l, r);
}
int binarySearch(int *x, int *y, int px, int py, int n)
{
    int l = 0;
    int r = n - 1;
    int ans = n;
    while (l <= r)
    {
        int m = (l + r) / 2;
        long long y0 = (long long)y[m];
        long long x0 = (long long)x[m];
        if (x0 * y0 - x0 * py - y0 * px > 0) // P is on the left of line xy_m
        {
            r = m - 1;
            ans = m;
        }
        else
            l = m + 1;
    }
    return ans;
}
int main()
{
    int n, m;
    scanf("%d", &n);
    int *x = new int[n];
    int *y = new int[n];
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);
    for (int i = 0; i < n; i++)
        scanf("%d", &y[i]);
    sort(x, 0, n - 1);
    sort(y, 0, n - 1);
    scanf("%d", &m);
    int *px = new int[m];
    int *py = new int[m];
    for (int i = 0; i < m; i++)
    {
        scanf("%d %d", &px[i], &py[i]);
        printf("%d\n", binarySearch(x, y, px[i], py[i], n));
    }
    return 0;
}