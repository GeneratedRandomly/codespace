#ifndef TREE_H_
#define TREE_H_

#include "Point.h"
#include "Timer.h"
#include "Board.h"
#include "Node.h"
#include <cstring>
class Node;

class Tree
{
    friend class Node;

private:
    static int M, N;
    static int noX, noY;
    static int *original_top;
    static int *top;

    static Board original_boards[2]; // 0: myboard, 1: enemyboard
    static Board boards[2];          // 0: myboard, 1: enemyboard

    Timer *timer;

    Node *root = nullptr; // 根节点

public:
    Tree(const int &M, const int &N, const int &noX, const int &noY);
    ~Tree();
    void init(const int *top, const int **_board);
    Point search();
    Point must();
    Node *treePolicy(Node *node)
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
    double defaultPolicy(Node *node)
    {
        while (!node->isTerminal())
        {
            int randomY = rand() % N;
        }
    }
    void backup(Node *node, double delta)
    {
        node->backup(delta);
    }
};

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
}

Tree::~Tree()
{
    if (root)
        delete root;
    if (timer)
        delete timer;
    delete[] original_top;
    delete[] top;
}

void Tree::init(const int *_top, const int **_board)
{
    original_top = new int[N];
    top = new int[N];
    for (int i = 0; i < N; ++i)
        original_top[i] = top[i] = _top[i];
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (_board[i][j] == 2)
                original_boards[0].place(i, j);
            else if (_board[i][j] == 1)
                original_boards[1].place(i, j);
        }
    }
    boards[0] = original_boards[0];
    boards[1] = original_boards[1];
}

Point Tree::search()
{
    root = Node::New(-1, -1, OTHER, nullptr);
    Node::usedMemory = 0;
    Point must = root->must();
    if (must.x != -1 && must.y != -1)
        return must;

    while (!timer->isTimeOut())
    {
        boards[0] = original_boards[0];
        boards[1] = original_boards[1];
        memcpy(top, original_top, N * sizeof(int));
        Node *v = treePolicy(root);
        double delta = defaultPolicy(v);
        backup(v, delta);
    }
    Node *child = root->bestChild(0);
    return Point(child->x, child->y);
}

Point Tree::must()
{
    return Point(-1, -1);
}
#endif