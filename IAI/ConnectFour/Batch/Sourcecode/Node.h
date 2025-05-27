#ifndef NODE_H
#define NODE_H

#include <cstddef>
#include <cstdlib>
#include <cmath>
#include "Judge.h"
#include "Point.h"

const char ME = 0;
const char OTHER = 1;
class Tree;

class Node
{
    friend class Tree;

private:
    static const int maxsize;
    static Node *pool;
    static size_t usedMemory;
    static int height;
    static int width;
    static int no_x, no_y;
    static double c;

    Node *father;
    Node **children;

    int visit_cnt;
    double profit;
    int expandable_num;
    int *expandable_nodes;

    char player;
    char finalCache;
    bool from_pool;

    int x, y;

public:
    Node();
    Node(int x, int y, int player, Node *father);
    Node *set(int _x, int _y, int _player, Node *_father);
    ~Node();
    bool isTerminal();
    static Node *New(int x, int y, int player, Node *father);
    Node *expand();
    Point must();
    void backup(double delta);
    Node *bestChild(double c);
};

#endif