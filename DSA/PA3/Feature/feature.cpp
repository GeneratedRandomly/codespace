#include "feature.h"
#include <cstdlib>
#include <cmath>
#include <stdio.h>

#define ll long long

int max(int a, int b) { return a > b ? a : b; }

struct w_node
{
    int w, x;
    int height = 1;
    int size = 1;
    w_node *lc = nullptr;
    w_node *rc = nullptr;
};

struct w_abs_node
{
    int w, x;
    ll sum = 0;
    int height = 1;
    int size = 1;
    w_abs_node *lc = nullptr;
    w_abs_node *rc = nullptr;
};

class wTree
{
private:
    w_node *root = nullptr;
    int x;              // remove时把x存进来
    bool x_set = false; // remove时x是否被设置
    int height(w_node *node) { return node ? node->height : 0; }
    int size(w_node *node) { return node ? node->size : 0; }
    int balanceFactor(w_node *node) { return node ? height(node->lc) - height(node->rc) : 0; }
    void update(w_node *node)
    {
        if (node)
        {
            node->height = 1 + max(height(node->lc), height(node->rc));
            node->size = 1 + size(node->lc) + size(node->rc);
        }
    }
    w_node *rotateRight(w_node *y)
    {
        w_node *x = y->lc;
        w_node *T2 = x->rc;
        x->rc = y;
        y->lc = T2;
        update(y);
        update(x);
        return x;
    }
    w_node *rotateLeft(w_node *x)
    {
        w_node *y = x->rc;
        w_node *T2 = y->lc;
        y->lc = x;
        x->rc = T2;
        update(x);
        update(y);
        return y;
    }
    w_node *insert(w_node *node, w_node *data)
    {
        if (!node)
            return data;
        if (data->w < node->w)
            node->lc = insert(node->lc, data);
        else if (data->w > node->w)
            node->rc = insert(node->rc, data);
        else
            return node;

        update(node);
        int balance = balanceFactor(node);
        // balance > 1说明左子树高度大于右子树高度，需要右旋
        // data->w < node->lc->w说明data插入到了node左子树的左子树，可以直接右旋
        if (balance > 1 && data->w < node->lc->w)
            return rotateRight(node);
        if (balance < -1 && data->w > node->rc->w)
            return rotateLeft(node);
        // 左-右失衡
        if (balance > 1 && data->w > node->lc->w)
        {
            node->lc = rotateLeft(node->lc);
            return rotateRight(node);
        }
        // 右-左失衡
        if (balance < -1 && data->w < node->rc->w)
        {
            node->rc = rotateRight(node->rc);
            return rotateLeft(node);
        }
        return node;
    }
    w_node *minValueNode(w_node *node)
    {
        w_node *current = node;
        while (current->lc)
            current = current->lc;
        return current;
    }
    w_node *deleteNode(w_node *node, w_node *data)
    {
        if (!node)
            return node;
        if (data->w < node->w)
            node->lc = deleteNode(node->lc, data);
        else if (data->w > node->w)
            node->rc = deleteNode(node->rc, data);
        else
        {
            if (!x_set)
            {
                x = node->x;
                x_set = true;
            }
            if ((node->lc == nullptr) || (node->rc == nullptr))
            {
                w_node *temp = node->lc ? node->lc : node->rc;
                if (!temp)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                free(temp);
            }
            else
            {
                w_node *temp = minValueNode(node->rc);
                node->w = temp->w;
                node->x = temp->x;
                node->rc = deleteNode(node->rc, temp);
            }
        }
        if (!node)
            return node;

        update(node);
        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->lc) >= 0)
            return rotateRight(node);
        if (balance > 1 && balanceFactor(node->lc) < 0)
        {
            node->lc = rotateLeft(node->lc);
            return rotateRight(node);
        }
        if (balance < -1 && balanceFactor(node->rc) <= 0)
            return rotateLeft(node);
        if (balance < -1 && balanceFactor(node->rc) > 0)
        {
            node->rc = rotateRight(node->rc);
            return rotateLeft(node);
        }
        return node;
    }
    w_node *findKth(w_node *node, int k, int x)
    {
        if (!node)
            return nullptr;
        int rightSize = size(node->rc);
        if (k <= rightSize)
            return findKth(node->rc, k, x);
        else if (k == rightSize + 1)
            return node;
        else
            return findKth(node->lc, k - rightSize - 1, x);
    }

public:
    wTree() {}
    void insert(w_node data) { root = insert(root, new w_node(data)); }
    void remove(w_node data)
    {
        x_set = false;
        root = deleteNode(root, new w_node(data));
    }
    int getx() { return x; }
    w_node *findKth(int k, int x) { return findKth(root, k, x); }
};

class w_absTree
{
private:
    w_abs_node *root = nullptr;
    ll delta; // modify时的delta
    ll sum = 0;
    int height(w_abs_node *node) { return node ? node->height : 0; }
    int size(w_abs_node *node) { return node ? node->size : 0; }
    int balanceFactor(w_abs_node *node) { return node ? height(node->lc) - height(node->rc) : 0; }
    void update(w_abs_node *node)
    {
        if (node)
        {
            node->sum = (ll)(node->w) * (ll)(node->x) + (node->lc ? node->lc->sum : 0) + (node->rc ? node->rc->sum : 0);
            node->height = 1 + max(height(node->lc), height(node->rc));
            node->size = 1 + size(node->lc) + size(node->rc);
        }
    }
    w_abs_node *rotateRight(w_abs_node *y)
    {
        w_abs_node *x = y->lc;
        w_abs_node *T2 = x->rc;
        x->rc = y;
        y->lc = T2;
        update(y);
        update(x);
        return x;
    }
    w_abs_node *rotateLeft(w_abs_node *x)
    {
        w_abs_node *y = x->rc;
        w_abs_node *T2 = y->lc;
        y->lc = x;
        x->rc = T2;
        update(x);
        update(y);
        return y;
    }
    w_abs_node *insert(w_abs_node *node, w_abs_node *data)
    {
        if (!node)
        {
            data->sum = (ll)(data->w) * (ll)(data->x);
            return data;
        }
        if (abs(data->w) < abs(node->w))
            node->lc = insert(node->lc, data);
        else if (abs(data->w) > abs(node->w))
            node->rc = insert(node->rc, data);
        else
            return node;

        update(node);
        int balance = balanceFactor(node);

        if (balance > 1 && abs(data->w) < abs(node->lc->w))
            return rotateRight(node);
        if (balance < -1 && abs(data->w) > abs(node->rc->w))
            return rotateLeft(node);
        if (balance > 1 && abs(data->w) > abs(node->lc->w))
        {
            node->lc = rotateLeft(node->lc);
            return rotateRight(node);
        }
        if (balance < -1 && abs(data->w) < abs(node->rc->w))
        {
            node->rc = rotateRight(node->rc);
            return rotateLeft(node);
        }
        return node;
    }
    w_abs_node *minValueNode(w_abs_node *node)
    {
        w_abs_node *current = node;
        while (current->lc != nullptr)
            current = current->lc;
        return current;
    }
    w_abs_node *deleteNode(w_abs_node *node, w_abs_node *data)
    {
        if (!node)
            return node;
        if (abs(data->w) < abs(node->w))
            node->lc = deleteNode(node->lc, data);
        else if (abs(data->w) > abs(node->w))
            node->rc = deleteNode(node->rc, data);
        else
        {
            if ((node->lc == nullptr) || (node->rc == nullptr))
            {
                w_abs_node *temp = node->lc ? node->lc : node->rc;
                if (!temp)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                free(temp);
            }
            else
            {
                w_abs_node *temp = minValueNode(node->rc);
                node->w = temp->w;
                node->x = temp->x;
                node->rc = deleteNode(node->rc, temp);
            }
        }
        if (!node)
            return node;

        update(node);
        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->lc) >= 0)
            return rotateRight(node);
        if (balance > 1 && balanceFactor(node->lc) < 0)
        {
            node->lc = rotateLeft(node->lc);
            return rotateRight(node);
        }
        if (balance < -1 && balanceFactor(node->rc) <= 0)
            return rotateLeft(node);
        if (balance < -1 && balanceFactor(node->rc) > 0)
        {
            node->rc = rotateRight(node->rc);
            return rotateLeft(node);
        }
        return node;
    }
    void findKth(w_abs_node *node, int k)
    {
        if (!node)
            return;
        int rightSize = size(node->rc);
        if (k <= rightSize)
            findKth(node->rc, k);
        else if (k == rightSize + 1)
            sum += node->sum - ((node->lc) ? node->lc->sum : 0);
        else
        {
            sum += node->sum - ((node->lc) ? node->lc->sum : 0);
            findKth(node->lc, k - rightSize - 1);
        }
    }
    void modify(w_abs_node *node, int w, int x)
    {
        if (!node)
            return;
        if (abs(w) < abs(node->w))
            modify(node->lc, w, x);
        else if (abs(w) > abs(node->w))
            modify(node->rc, w, x);
        else
            node->x = x;
        node->sum += delta;
    }

public:
    w_absTree() {}
    void insert(w_abs_node data) { root = insert(root, new w_abs_node(data)); }
    void remove(w_abs_node data) { root = deleteNode(root, new w_abs_node(data)); }
    ll findKth(int k)
    {
        sum = 0;
        findKth(root, k);
        return sum;
    }
    void modifyByWeight(int dataw, int new_x, int old_x)
    {
        this->delta = (ll)(new_x - old_x) * (ll)dataw;
        modify(root, dataw, new_x);
    }
};

wTree w_tree;
w_absTree w_abs_tree;
int *wlist = nullptr;

void init(int n, const int *w, const int *x)
{
    ::wlist = new int[n + 1];
    for (int i = 1; i <= n; ++i)
    {
        ::wlist[i] = w[i];
        w_tree.insert({w[i], x[i]});
        w_abs_tree.insert({w[i], x[i]});
    }
}

void modify_weight(int i, int new_w)
{
    int old_w = wlist[i];
    w_tree.remove({old_w});
    w_abs_tree.remove({old_w});
    wlist[i] = new_w;
    int x = w_tree.getx();
    w_tree.insert({new_w, x});
    w_abs_tree.insert({new_w, x});
}

void modify_value(int k, int new_x)
{
    w_node *data = w_tree.findKth(k, new_x);
    w_abs_tree.modifyByWeight(data->w, new_x, data->x);
    data->x = new_x;
}

long long calculate(int k) { return w_abs_tree.findKth(k); }