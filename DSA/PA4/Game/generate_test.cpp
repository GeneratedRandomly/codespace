#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    srand(time(0));
    int N = 100000;               // 随机生成1到100之间的关卡数量
    int M = 100000; // 随机生成1到N*(N-1)/2之间的道路数量
    cout << N << " " << M << endl;

    for (int i = 0; i < N; ++i)
        cout << rand() % 10000 + 1 << " "; // 随机生成每个关卡的通关时间
    cout << endl;

    for (int i = 0; i < M; ++i)
    {
        int u = rand() % N + 1;
        int v = rand() % N + 1;
        while (u == v)
            v = rand() % N + 1;
        cout << u << " " << v << endl;
    }

    return 0;
}