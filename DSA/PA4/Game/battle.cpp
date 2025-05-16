#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
using namespace std;

void run_command(const string &command)
{
    int result = system(command.c_str());
    if (result != 0)
    {
        cerr << "Command failed: " << command << endl;
        exit(1);
    }
}

bool compare_outputs()
{
    ifstream output_a("output_a.txt");
    ifstream output_game("output_game.txt");

    string line_a, line_game;
    while (getline(output_a, line_a) && getline(output_game, line_game))
    {
        if (line_a != line_game)
        {
            return false;
        }
    }
    return !(getline(output_a, line_a) || getline(output_game, line_game));
}

int main()
{
    int test_count = 0;
    while (true)
    {
        // 生成测试数据
        run_command("./generate_test > input.txt");

        // 运行 a.cpp 程序
        run_command("./a < input.txt > output_a.txt");

        // 运行 game.cpp 程序
        run_command("./game < input.txt > output_game.txt");

        // 比较输出
        if (!compare_outputs())
        {
            cout << "Test failed on iteration " << test_count << "!" << endl;
            cout << "Input data:" << endl;
            ifstream input("input.txt");
            cout << input.rdbuf();
            cout << "Output from a.cpp:" << endl;
            ifstream output_a("output_a.txt");
            cout << output_a.rdbuf();
            cout << "Output from game.cpp:" << endl;
            ifstream output_game("output_game.txt");
            cout << output_game.rdbuf();
            break;
        }
        else
        {
            cout << "Test passed on iteration " << test_count << endl;
        }
        test_count++;
    }

    return 0;
}