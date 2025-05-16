#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cassert>
#include <unordered_set>

using namespace std;

const int MAX_W = 1e9;
const int MAX_X = 1000;
const int MAX_N = 100;
const int MAX_M = 100;

void generate_test_case(int n, int m)
{
    cout << n << " " << m << endl;
    unordered_set<int> abs_weights; // 用于存储已生成的权重绝对值
    vector<int> weights(n + 1);     // 用于存储生成的权重

    for (int i = 1; i <= n; ++i)
    {
        int w;
        do
        {
            w = rand() % (2 * MAX_W + 1) - MAX_W; // [-1000, 1000]
        } while (abs_weights.count(abs(w)) > 0); // 确保权重绝对值唯一
        abs_weights.insert(abs(w));
        weights[i] = w;
        int x = rand() % (2 * MAX_X + 1) - MAX_X; // [-1000, 1000]
        cout << w << " " << x << endl;
    }

    for (int i = 0; i < m; ++i)
    {
        int op = rand() % 3;
        if (op == 0)
        {
            int idx = rand() % n + 1;
            int new_w;
            do
            {
                new_w = rand() % (2 * MAX_W + 1) - MAX_W; // [-1000, 1000]
            } while (abs_weights.count(abs(new_w)) > 0); // 确保新权重绝对值唯一
            abs_weights.erase(abs(weights[idx])); // 移除旧权重绝对值
            abs_weights.insert(abs(new_w));       // 插入新权重绝对值
            weights[idx] = new_w;
            cout << "W " << idx << " " << new_w << endl;
        }
        else if (op == 1)
        {
            int k = rand() % n + 1;
            int x = rand() % (2 * MAX_X + 1) - MAX_X;
            cout << "V " << k << " " << x << endl;
        }
        else
        {
            int k = rand() % n + 1;
            cout << "C " << k << endl;
        }
    }
}

void run_solution(const char *cmd, const char *input_file, const char *output_file) {
    string command = string(cmd) + " < " + input_file + " > " + output_file;
    if (system(command.c_str()) == -1) {
        perror("system");
        exit(EXIT_FAILURE);
    }
}

bool compare_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    assert(f1 && f2);

    char ch1 = getc(f1);
    char ch2 = getc(f2);

    while (ch1 != EOF && ch2 != EOF) {
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return false;
        }
        ch1 = getc(f1);
        ch2 = getc(f2);
    }

    bool result = (ch1 == EOF && ch2 == EOF);

    fclose(f1);
    fclose(f2);
    return result;
}

int main() {
    srand(time(0));
    const char *input_file = "input.txt";
    const char *output_file1 = "output1-feature.txt";
    const char *output_file2 = "output2-correct.txt";

    for (int i = 0; i < 100; ++i) {
        // 生成随机测试数据
        if (freopen(input_file, "w", stdout) == nullptr) {
            perror("freopen");
            exit(EXIT_FAILURE);
        }
        int n = rand() % MAX_N + 1; // 特征总数
        int m = rand() % MAX_M + 1; // 操作次数
        generate_test_case(n, m);
        fclose(stdout);

        // 运行你的代码
        run_solution("./feature", input_file, output_file1);

        // 运行暴力解
        run_solution("./correct", input_file, output_file2);

        // 比较输出
        if (!compare_files(output_file1, output_file2)) {
            cout << "Test case " << i << " failed." << endl;
            return 1;
        }
    }

    cout << "All test cases passed." << endl;
    return 0;
}