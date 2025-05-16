#ifndef NODE_H
#define NODE_H

#include <cstddef> // For size_t
#include <cstdlib> // For rand
#include <cmath>   // For sqrt
#include "Judge.h"
#include "Tree.h"

const char ME = 0;
const char OTHER = 1;
class Tree;

class Node
{
    friend class Tree;

private:
    static const int maxsize = 600000;
    static Node *pool;        // 内存池
    static size_t usedMemory; // 已使用内存
    static int height;        // 棋盘高度
    static int width;         // 棋盘宽度
    static int no_x, no_y;    // 被去除的点位
    static double c;          // UCT公式中的常数

    Node *father = nullptr;    // 单个父节点
    Node **children = nullptr; // 子节点组（懒惰加载width个）

    int visit_cnt = 0;               // 总访问次数
    double profit = 0.0;             // 当前节点的胜率
    int expandable_num = 0;          // 可扩展节点数
    int *expandable_nodes = nullptr; // 从当前节点开始可扩展节点的列索引

    char player = ME;     // 当前节点的落子方
    char finalCache = -1; // 是否为终局节点，-1未计算，0非终局，1终局

    int x = -1, y = -1; // 落子位置

public:
    Node() = default;
    Node(int x, int y, int player, Node *father);
    Node *set(int _x, int _y, int _player, Node *_father)
    {
        x = _x;
        y = _y;
        player = _player;
        father = _father;
        visit_cnt = 0;
        profit = 0.0;
        expandable_num = 0;
        finalCache = -1;

        born();
        return this;
    }
    void born()
    {
        this->children = new Node *[width];
        this->expandable_nodes = new int[width];
        for (int i = 0; i < width; i++)
        {
            if (Tree::top[i] != 0)
                this->expandable_nodes[expandable_num++] = i;
            this->children[i] = nullptr;
        }
    }
    ~Node();
    bool isTerminal()
    {
        if (finalCache != -1)
            return finalCache;
        return finalCache = (Tree::boards[1 - player].win() || !expandable_num);
    }
    static Node *New(int x, int y, int player, Node *father)
    {
        if (usedMemory < maxsize)
            return pool[usedMemory++].set(x, y, player, father);
        else
            return new Node(x, y, player, father);
    }
    Node *expand()
    {
        int expand_column = rand() % expandable_num;
        int new_y = expandable_nodes[expand_column], new_x = --Tree::top[new_y];
        Tree::boards[player].place(new_x, new_y);
        jumpOverHole(new_x, new_y);
        children[new_y] = New(new_x, new_y, 1 - player, this);
        std::swap(expandable_nodes[expand_column], expandable_nodes[--expandable_num]);
        return children[new_y];
    }
    void jumpOverHole(int x, int y)
    {
        if (x - 1 == no_x && y == no_y)
            Tree::top[y]--;
    }
    Point must() // 计算必胜和必应点，若无则返回(-1, -1)
    {
        for (int i = 0; i < width; i++)
        {
            if (Tree::top[i] > 0)
            {
                Board board = Tree::boards[ME];
                int x = Tree::top[i] - 1;
                board.place(x, i);
                if (board.win())
                    return Point(x, i);
                board = Tree::boards[OTHER];
                board.place(x, i);
                if (board.win())
                    return Point(x, i);
            }
        }
        return Point(-1, -1);
    }
    void backup(double delta)
    {
        Node *p = this;
        while (p)
        {
            p->visit_cnt++;
            p->profit += delta;
            p = p->father;
        }
    }
    Node *bestChild(double c)
    {
        Node *bestNode = nullptr;
        double maxChildProfit = -RAND_MAX;
        for (int i = 0; i < width; i++)
        {
            if (auto child = children[i])
            {
                double childProfit = child->profit * (player == OTHER ? 1 : -1);
                childProfit = (childProfit / child->visit_cnt) + c * sqrt(2 * log(double(visit_cnt)) / child->visit_cnt);
                if (childProfit > maxChildProfit)
                {
                    maxChildProfit = childProfit;
                    bestNode = children[i];
                }
            }
        }
        return bestNode;
    }
};

const int maxsize = 600000;
Node *Node::pool = new Node[maxsize];
size_t Node::usedMemory = 0;
double Node::c = 1 / sqrt(2);

Node::Node(int _x, int _y, int _player, Node *_father)
    : x(_x), y(_y), player(_player), father(_father) { born(); }

Node::~Node()
{
    delete[] expandable_nodes;
    expandable_nodes = nullptr;
    for (int i = 0; i < width; ++i)
        if (children[i])
            delete children[i];
    delete[] children;
    children = nullptr;
}

#endif