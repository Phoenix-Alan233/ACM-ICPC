#include <bits/stdc++.h>
using namespace std;

namespace IO {
    const int SIZE = 1 << 20;
    char ibuf[SIZE]; int iS, iT;
    char obuf[SIZE]; int oT;
    inline char gc() {
        if (iS == iT) iS = 0, iT = fread(ibuf, 1, SIZE, stdin);
        return iS == iT ? EOF : ibuf[iS++];
    }
    inline void pc(char c) {
        obuf[oT++] = c;
        if (oT == SIZE) fwrite(obuf, 1, SIZE, stdout), oT = 0;
    }
    inline int read() {
        int x = 0, f = 0;
        char c = gc();
        while (!isdigit(c)) f |= c == '-', c = gc();
        while (isdigit(c)) x = 10 * x + c - '0', c = gc();
        return f ? -x : x;
    }
    inline void print(__int128 x) {
        static char buf[128];
        int len = 0;
        if (x < 0) pc('-'), x = -x;
        do { buf[++len] = x % 10 + '0', x /= 10; } while (x);
        while (len) pc(buf[len--]);
    }
    inline void print(__int128 x, char ch) {
        print(x), pc(ch);
    }
    struct Flusher {
        ~Flusher() {
            if (oT) fwrite(obuf, 1, oT, stdout);
        }
    } flusher;
}

#define ll long long

__int128 ans = 0;
ll value[2000005], n, K, R, X, C, cur, take;

int main() {
    n = IO::read(), K = IO::read(), R = IO::read(), X = IO::read(), C = IO::read();
    for (int i = 1; i <= n; i++) {
        ll x = IO::read();
        cur += x;
        // 位置 i 带来的收益
        value[i] = (n - i + 1) * C;
        assert(take <= cur);
        if (take < cur) {
            take++;
            value[i] += (n - i + 1) * R;
        }
        ans += X - cur * R;
    }
    // sort(value + 1, value + n + 1, greater<ll> ());
    nth_element(value + 1, value + K, value + n + 1, greater<ll> ());
    for (int i = 1; i <= K; i++) {
        ans += value[i];
    }
    IO::print(ans, '\n');
    return 0;
}