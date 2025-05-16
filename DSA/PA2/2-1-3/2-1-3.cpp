#include <stdio.h>
#define Rank int

class deque
{
private:
    int *data = nullptr;
    Rank front = 0;
    Rank rear = 0;

public:
    deque() {}
    ~deque() { delete[] data; }
    void set_size(int n) { data = new int[n + 2]; }
    void push_rear(int x) { data[rear++] = x; }
    int pop_front() { return data[front++]; }
    // void push_front(int x){data[--front] = x;}
    int pop_rear() { return data[--rear]; }
    int get_front() { return data[front]; }
    int get_back() { return data[rear - 1]; }
    bool empty() { return front == rear; }
};

class queap
{
private:
    deque p;
    deque dq;

public:
    queap(int n)
    {
        p.set_size(n);
        dq.set_size(n);
    }
    ~queap() {}
    void push(int x)
    {
        p.push_rear(x);
        //单调队列
        while (dq.empty() == false && dq.get_back() < x)
            dq.pop_rear();
        //单调队列
        dq.push_rear(x);
    }
    int pop()
    {
        int tmp = p.get_front();
        if (dq.get_front() == p.pop_front())
            dq.pop_front();
        return tmp;
    }
    int getmax()
    {
        return dq.get_front();
    }
};

int main()
{
    int stock_num;
    scanf("%d", &stock_num);
    queap pq(stock_num);
    int current_hold_stock = 0;
    long long sum = 0;
    for (int i = 0; i < 2 * stock_num; i++)
    {
        int days;
        scanf("%d", &days);
        sum += ((long long)current_hold_stock) * ((long long)days);
        if (getchar() == '\n')
        {
            if (pq.pop() == current_hold_stock)
            {
                current_hold_stock = pq.getmax();
            }
        }
        else
        {
            int x;
            scanf("%d", &x);
            pq.push(x);
            if (x > current_hold_stock)
            {
                current_hold_stock = x;
            }
        }
    }
    printf("%lld\n", sum);
    return 0;
}