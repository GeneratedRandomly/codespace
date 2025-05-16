#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void generateMatrix(ofstream &out, int rows, int cols, const char charset[], int charsetSize) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            out << charset[rand() % charsetSize];
        }
        out << endl;
    }
}

void generateTargetMatrix(ofstream &out, char **T, int M, int N, int m, int n) {
    int startRow = rand() % (M - m + 1);
    int startCol = rand() % (N - n + 1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            out << T[startRow + i][startCol + j];
        }
        out << endl;
    }
}

int main() {
    srand(time(0));

    int M = 2000; // 主矩阵行数
    int N = 1000; // 主矩阵列数
    int m = 10;   // 目标矩阵行数
    int n = 10;   // 目标矩阵列数
    int k = 100;  // 目标矩阵数量

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int charsetSize = sizeof(charset) - 1;

    ofstream out("input.txt");
    out << M << " " << N << " " << m << " " << n << " " << k << endl;

    // 生成主矩阵
    char **T = new char*[M];
    for (int i = 0; i < M; ++i) {
        T[i] = new char[N + 1];
        for (int j = 0; j < N; ++j) {
            T[i][j] = charset[rand() % charsetSize];
        }
        T[i][N] = '\0';
        out << T[i] << endl;
    }

    // 生成目标矩阵
    for (int i = 0; i < k; ++i) {
        generateTargetMatrix(out, T, M, N, m, n);
    }

    // 释放动态分配的内存
    for (int i = 0; i < M; ++i) {
        delete[] T[i];
    }
    delete[] T;

    out.close();
    return 0;
}