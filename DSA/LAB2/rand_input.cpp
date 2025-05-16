#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

using namespace std;

char a[1000000];
int m, len;

void play(int t)
{
    int l = t - 1, r = t + 1;
    while (l >= 0 && a[l] == a[t])
        --l;
    while (r < len && a[r] == a[t])
        ++r;
    if (r - l > 3)
    {
        memmove(a + l + 1, a + r, len - r + 1);
        len -= (r - l - 1);
        if (l >= 0)
            play(l);
    }
}

int main()
{
    srand(time(0));
    len = rand() % 500001;
    m = rand() % 500001;

    for (int i = 0; i < len; i++)
    {
        a[i] = rand() % 2 + 65;
        if (i >= 2 && a[i] == a[i - 1] && a[i - 1] == a[i - 2])
            a[i]++;
        if (a[i] == 'B' + 1)
            a[i] = 'A';
    }

    a[len] = '\0';
    puts(a);
    printf("%d\n", m);

    while (m--)
    {
        char col = rand() % 2 + 65;
        int pos = rand() % len;
        printf("%d %c\n", pos, col);
        memmove(a + pos + 1, a + pos, len - pos + 1);
        a[pos] = col;
        ++len;
        play(pos);
    }
    return 0;
}
