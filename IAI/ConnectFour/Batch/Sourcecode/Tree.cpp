#include "Tree.h"
#include <cstdlib>
#include <ctime>

// 静态成员定义
int Tree::M = 0;
int Tree::N = 0;
int Tree::noX = 0;
int Tree::noY = 0;
int *Tree::original_top = nullptr;
int *Tree::top = nullptr;
Board *Tree::original_boards[2] = {nullptr, nullptr}; // 0: myboard, 1: enemyboard
Board *Tree::boards[2] = {nullptr, nullptr};          // 0: myboard, 1: enemyboard

Tree::Tree(const int &_M, const int &_N, const int &_noX, const int &_noY)
{
    timer = new Timer();
    M = _M;
    N = _N;
    noX = _noX;
    noY = _noY;
    Node::width = N;
    Node::height = M;
    Node::no_x = noX;
    Node::no_y = noY;
    root = nullptr;
}

Tree::~Tree()
{
    if (root && !root->from_pool)
        delete root;
    if (timer)
        delete timer;
    delete[] original_top;
    delete[] top;
}

void Tree::init(const int *_top, const int *_board)
{
    original_top = new int[N];
    top = new int[N];
    for (int i = 0; i < N; ++i)
        original_top[i] = top[i] = _top[i];
    original_boards[0] = new Board(M, N);
    original_boards[1] = new Board(M, N);
    boards[0] = new Board(M, N);
    boards[1] = new Board(M, N);
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (_board[i * N + j] == 2)
                original_boards[0]->place(i, j);
            else if (_board[i * N + j] == 1)
                original_boards[1]->place(i, j);
        }
    }
}

Point Tree::search(int lastX, int lastY)
{
    root = Node::New(lastX, lastY, OTHER, nullptr);
    Point must = root->must();
    if (must.x != -1 && must.y != -1)
        return must;
    srand(time(nullptr));
    while (!timer->isTimeOut())
    {
        *boards[0] = *original_boards[0];
        *boards[1] = *original_boards[1];
        memcpy(top, original_top, N * sizeof(int));
        Node *v = treePolicy(root);
        double delta = defaultPolicy(v);
        backup(v, delta);
    }
    Node *child = root->bestChild(0);
    return Point(child->x, child->y);
}

Node *Tree::treePolicy(Node *node)
{
    while (!node->isTerminal())
    {
        if (node->expandable_num > 0)
            return node->expand();
        else
            node = node->bestChild(Node::c);
    }
    return node;
}

double Tree::defaultPolicy(Node *node)
{
    while (true)
    {
        if (boards[1 - (int)node->player]->win())
            return node->player == ME ? -1.0 : 1.0; // 对手赢了
        if (boards[(int)node->player]->win())
            return node->player == ME ? 1.0 : -1.0;
        bool tie = true;
        for (int i = 0; i < N; i++)
        {
            if (top[i] > 0)
            {
                tie = false;
                break;
            }
        }
        if (tie)
            return 0.0; // 平局

        int randomY = rand() % N;
        while (top[randomY] == 0) // 找到一个可落子的位置
        {
            randomY = rand() % N;
        }
        int x = --top[randomY];
        if (x == noX && randomY == noY)
        {
            top[randomY]--;
            x--;
        }
        boards[(int)node->player]->place(x, randomY);
    }
}

void Tree::backup(Node *node, double delta)
{
    node->backup(delta);
}