#include <cstdio>
#include <climits>
#define ll long long

const long long MOD = 100000000000001219LL;

ll mul(ll a, ll b) { return (__int128_t(a) * b) % MOD; }

long long powpow(long long base, long long index)
{
    long long res = 1;
    while (index)
    {
        if (index & 1)
            res = mul(res, base);
        base = mul(base, base);
        index >>= 1;
    }
    return res % MOD;
}

int main()
{
    long long base = 100493;
    printf("%lld\n", powpow(base, MOD - 2));
    return 0;
}