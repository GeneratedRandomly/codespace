#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

void generateData(int n)
{
    srand(time(0));
    std::ofstream outfile("data.txt");
    for (int i = 0; i < n; ++i)
        outfile << rand() << " ";
    outfile.close();
}

void generateWorstCase(int n) {

    std::ofstream outfile("data.txt");
    for (int i = 0; i < n; ++i) {
        outfile << i << " ";
    }
    outfile.close();
}

int main()
{
    const int SIZE = 10000000; // 数据规模
    
    //generateData(SIZE);
    generateWorstCase(SIZE);
    return 0;
}