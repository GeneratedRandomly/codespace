#include "Node.h"
#include "Tree.h"
#include <algorithm>

const int Node::maxsize = 600000;
Node *Node::pool = new Node[Node::maxsize];
size_t Node::usedMemory = 0;
int Node::height = 0;
int Node::width = 0;
int Node::no_x = 0;
int Node::no_y = 0;
double Node::c = 1 / sqrt(2.0);

// 内存池里的节点默认构造
Node::Node() : father(nullptr), children(nullptr), visit_cnt(0), profit(0.0),
               expandable_num(0), expandable_nodes(nullptr), player(ME),
               finalCache(-1), from_pool(true), x(-1), y(-1) {}

// 人为新增的节点不在内存池里面
Node::Node(int _x, int _y, int _player, Node *_father)
    : father(_father), children(nullptr), visit_cnt(0), profit(0.0),
      expandable_num(0), expandable_nodes(nullptr), player(_player),
      finalCache(-1), from_pool(false), x(_x), y(_y)
{
    children = new Node *[width];
    expandable_nodes = new int[width];
    for (int i = 0; i < width; i++)
    {
        children[i] = nullptr;
        if (Tree::top[i] != 0)
            expandable_nodes[expandable_num++] = i;
    }
}

Node *Node::set(int _x, int _y, int _player, Node *_father)
{
    father = _father;
    visit_cnt = 0;
    profit = 0.0;
    expandable_num = 0;
    player = _player;
    finalCache = -1;
    from_pool = true;
    x = _x;
    y = _y;
    children = new Node *[width];
    expandable_nodes = new int[width];
    for (int i = 0; i < width; i++)
    {
        children[i] = nullptr;
        if (Tree::top[i] != 0)
            expandable_nodes[expandable_num++] = i;
    }
    return this;
}

Node::~Node()
{
    if (!from_pool)
    {
        delete[] expandable_nodes;
        expandable_nodes = nullptr;
        for (int i = 0; i < width; ++i)
            if (children && children[i])
                delete children[i];
        delete[] children;
        children = nullptr;
    }
}

bool Node::isTerminal()
{
    if (finalCache != -1)
        return finalCache;
    if (x == -1)
        finalCache = 0;
    return (finalCache = (Tree::boards[1 - (int)player]->win() || !expandable_num));
}

Node *Node::New(int x, int y, int player, Node *father)
{
    if (usedMemory < maxsize)
        return pool[usedMemory++].set(x, y, player, father);
    else
        return new Node(x, y, player, father);
}

Node *Node::expand()
{
    int expand_column = rand() % expandable_num;
    int new_y = expandable_nodes[expand_column];
    int new_x = --Tree::top[new_y];
    if (new_x == no_x && new_y == no_y)
    {
        Tree::top[new_y]--;
        new_x--;
    }
    Tree::boards[(int)player]->place(new_x, new_y);
    children[new_y] = New(new_x, new_y, 1 - player, this);
    std::swap(expandable_nodes[expand_column], expandable_nodes[--expandable_num]);
    return children[new_y];
}

Point Node::must()
{
    for (int i = 0; i < width; i++)
    {
        if (Tree::top[i] > 0)
        {
            Board board = *Tree::boards[(int)ME];
            int x = Tree::top[i] - 1;
            board.place(x, i);
            if (board.win())
                return Point(x, i);
            board = *Tree::boards[(int)OTHER];
            board.place(x, i);
            if (board.win())
                return Point(x, i);
        }
    }
    return Point(-1, -1);
}

void Node::backup(double delta)
{
    Node *p = this;
    while (p)
    {
        p->visit_cnt++;
        p->profit += delta;
        p = p->father;
    }
}

Node *Node::bestChild(double c)
{
    Node *bestNode = nullptr;
    double maxChildProfit = -RAND_MAX;
    for (int i = 0; i < width; i++)
    {
        if (!children || !children[i])
            continue;
        double childProfit = children[i]->profit * (player == OTHER ? 1 : -1);
        childProfit = (childProfit / children[i]->visit_cnt) + c * sqrt(2 * log(double(visit_cnt)) / children[i]->visit_cnt);
        if (childProfit > maxChildProfit)
        {
            maxChildProfit = childProfit;
            bestNode = children[i];
        }
    }
    return bestNode;
}