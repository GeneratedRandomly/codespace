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

    static Board *original_boards[2]; // 0: myboard, 1: enemyboard
    static Board *boards[2];          // 0: myboard, 1: enemyboard

    Timer *timer;

    Node *root; // 根节点

public:
    Tree(const int &M, const int &N, const int &noX, const int &noY);
    ~Tree();
    void init(const int *top, const int *_board);
    Point search(int lastX, int lastY);
    Node *treePolicy(Node *node);
    double defaultPolicy(Node *node);
    void backup(Node *node, double delta);
};

#endif