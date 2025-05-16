#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>
struct point
{
    int x;
    int y;
    bool chain; // 0 is up, 1 is down
};
bool print(const point &p, const point &q, const point &r)
{
    printf("%d %d %d %d %d %d\n", p.x, p.y, q.x, q.y, r.x, r.y);
    return true;
}
bool cmp(const point &a, const point &b)
{
    return a.x < b.x;
}
int main()
{
    int n;
    scanf("%d", &n);
    std::vector<point> v;
    for (int i = 0; i < n; i++)
    {
        point p;
        scanf("%d %d", &p.x, &p.y);
        v.push_back(p); // p is v[i]
        if (i > 0)
            v[i].chain = (v[i].x < v[i - 1].x);
    }
    v[0].chain = (v[0].x < v[n - 1].x);
    std::sort(v.begin(), v.end(), cmp);
    std::stack<point> s;
    s.push(v[0]);
    s.push(v[1]);
    point tmp, reserve1, tmp2, tmp3;
    for (int i = 2; i < n; i++)
    {
        if (v[i].chain != s.top().chain) // 情况1
        {
            tmp = v[i];
            tmp2 = reserve1 = s.top();
            s.pop();
            tmp3 = s.top();
            s.pop();
            while (print(tmp3, tmp2, tmp) && !s.empty())
            {
                tmp2 = tmp3;
                tmp3 = s.top();
                s.pop();
            }
            s.push(reserve1);
            s.push(tmp);
        }
        else // 情况2
        {
            while (s.size() > 1)
            {
                tmp = s.top();
                s.pop();
                bool flag = (long long)(v[i].y - tmp.y) * (long long)(tmp.x - s.top().x) > (long long)(tmp.y - s.top().y) * (long long)(v[i].x - tmp.x);
                if (v[i].chain == flag) // 2.a
                {
                    s.push(tmp);
                    break;
                }
                else // 2.b
                {
                    print(tmp, s.top(), v[i]);
                }
            }
            s.push(v[i]);
        }
    }
    return 0;
}