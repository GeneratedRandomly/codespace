#include <stdio.h>
#include <cstring>
#include <iostream>
int max(int a, int b) { return a > b ? a : b; }

template <typename T>
class vector
{
private:
    T *data = nullptr;
    int size = 0;
    int capacity = 0;
    void expand() // refer to DSA class PPT by Prof.Deng
    {
        T *old_data = data;
        data = new T[capacity <<= 1];
        for (int i = 0; i < size; i++)
            data[i] = old_data[i];
        delete[] old_data;
    }

public:
    vector() : capacity(3), size(0) { data = new T[capacity]; }
    ~vector() { delete[] data; }
    void start(int s)
    {
        delete[] data;
        data = new T[size = capacity = s];
    }
    void insert(const T &value, int index)
    {
        if (size == capacity)
            expand();
        for (int i = size; i > size - index; --i)
            data[i] = data[i - 1];
        data[size - index] = value;
        ++size;
    }
    void remove(int index)
    {
        for (int i = size - index - 1; i < size - 1; ++i)
            data[i] = data[i + 1];
        --size;
    }
    T &operator[](int index) { return data[size - index - 1]; }
    int get_size() const { return size; }
    /*void push_back_unsafe(const T &value)
    {
        if (size == capacity)
            expand();
        data[size++] = value;
    }*/
    void push_back(const T &value) { insert(value, size); }
    void operator=(int *other)
    {
        T *old = data;
        data = other;
        delete[] old;
    }
};

struct TreeNode
{
public:
    int height = 0;
    int size = 1;
    int father = 0;
    vector<int> sons;
};

TreeNode nodes[1000005];
int node_count, instruction_count = 0;
int delta_height = 0;

void initialize()
{
    for (int i = 1; i <= node_count; ++i)
    {
        int son_amount;
        scanf("%d", &son_amount);
        nodes[i].sons.start(son_amount);
        for (int j = 0; j < son_amount; ++j)
        {
            int index;
            scanf("%d", &index);
            nodes[i].sons[son_amount - j - 1] = index;
            nodes[index].father = i;
        }
    }
}
void set_sons_info(int node)
{
    for (int i = 0; i < nodes[node].sons.get_size(); ++i)
    {
        int son = nodes[node].sons[i];
        set_sons_info(son);
        nodes[node].size += nodes[son].size;
        nodes[node].height = max(nodes[node].height, nodes[son].height + 1);
    }
}
int get_index_by_scanf()
{
    int n;
    scanf("%d", &n);
    int x;
    int index = 1;
    bool read = true;
    while (n--)
    {
        scanf("%d", &x);
        if (read && x < nodes[index].sons.get_size())
            index = nodes[index].sons[x];
        else
        {
            read = false;
            /*using namespace std;
            string s;
            getline(cin, s);
            break;*/
        }
    }
    return index;
}
void cut_recursive_set_height(int son_index /*already set*/, int father_index /*not set yet*/)
{
    if (delta_height == 0 || father_index == 0) // ensure father_index isn't 0
        return;
    TreeNode &son = nodes[son_index];
    TreeNode &father = nodes[father_index];
    if (son.height - delta_height + 1 < father.height) // son is not the decisive son of father(old son.height + 1 < father.height)
        return;
    // son is decisive son of father, and we will set father's height
    // father's height may lower but won't be higher
    int brother_max_height = 0;
    for (int i = 0; i < father.sons.get_size(); i++)
    {
        TreeNode &brother_i = nodes[father.sons[i]];
        if (father.height == brother_i.height + 1) // father's height can be ensafed by a brother
            return;
        brother_max_height = max(brother_max_height, brother_i.height);
    }
    delta_height = (brother_max_height + 1) - father.height; // (new father.height) - old father.height
    father.height += delta_height;
    if (father.father)
        cut_recursive_set_height(father_index, father.father);
}
void link_recursive_set_height(int son_index /*already set*/, int father_index /*not set yet*/)
{
    if (delta_height == 0 || father_index == 0) // ensure father_index isn't 0
        return;
    TreeNode &son = nodes[son_index];
    TreeNode &father = nodes[father_index];
    if (son.height + 1 <= father.height) // son won't be the decisive son of father
        return;
    // son is decisive son of father, and we will set father's height
    // father's height may higher but won't be lower
    delta_height = (son.height + 1) - father.height; // (new father.height) - old father.height
    father.height += delta_height;
    if (father.father)
        link_recursive_set_height(father_index, father.father);
}
void cut(int son_index)
{
    int father_index = nodes[son_index].father;
    if (father_index == 0)
        return;
    TreeNode &father = nodes[father_index];
    // remove son from father
    vector<int> &brothers = father.sons;
    for (int i = 0; i < brothers.get_size(); i++) // 清理son
    {
        if (brothers[i] == son_index)
        {
            brothers.remove(i);
            break;
        }
    }
    // reset father's size
    int reduce_size = nodes[son_index].size;
    father.size -= reduce_size;
    // reset father's height
    int old_height = father.height; // save old height
    father.height = 0;              // father height reset
    for (int i = 0; i < brothers.get_size(); i++)
        father.height = max(father.height, nodes[brothers[i]].height + 1);
    delta_height = father.height - old_height;                 // <= 0, new father.height - old father.height
    if (delta_height)                                          // if height changed
        cut_recursive_set_height(father_index, father.father); // reset all heights, start from grandpa
    while (father_index = nodes[father_index].father)          // iteratively reset size from grandpa
        nodes[father_index].size -= reduce_size;
}
void link(int son_index, int father_index, int i)
{
    TreeNode &father = nodes[father_index];
    TreeNode &son = nodes[son_index];
    // set son-father relation
    vector<int> &sons = father.sons;
    i >= sons.get_size() ? sons.push_back(son_index) : sons.insert(son_index, i); // add son in father
    son.father = father_index;                                                    // add father in son
    // reset father's size
    int add_size = son.size;
    father.size += add_size;
    // reset father's height
    int old_height = father.height; // save old height
    father.height = max(father.height, son.height + 1);
    delta_height = son.height + 1 - old_height;                 // new son.height(=nodes[son_index].height) - old son.height(=0)
    if (delta_height)                                           // if height changed
        link_recursive_set_height(father_index, father.father); // reset all heights, start from grandpa
    while (father_index = nodes[father_index].father)           // iteratively reset size from grandpa
        nodes[father_index].size += add_size;
}
void move()
{
    int source = get_index_by_scanf();
    cut(source);
    printf("cut");
    int target = get_index_by_scanf();
    int k;
    scanf("%d", &k);
    link(source, target, k);
}
void get_height()
{
    int node = get_index_by_scanf();
    printf("%d\n", nodes[node].height);
}
void get_size()
{
    int node = get_index_by_scanf();
    printf("%d\n", nodes[node].size);
}
int main()
{
    scanf("%d %d", &node_count, &instruction_count);
    initialize();
    set_sons_info(1);
    for (int i = 0; i < instruction_count; ++i)
    {
        int op;
        scanf("%d", &op);
        if (op == 0)
            move();
        else if (op == 1)
            get_height();
        else if (op == 2)
            get_size();
    }
    return 0;
}