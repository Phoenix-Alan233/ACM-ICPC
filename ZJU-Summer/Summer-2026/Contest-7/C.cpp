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
    inline long long read() {
        long long x = 0, f = 0;
        char c = gc();
        while (!isdigit(c)) f |= c == '-', c = gc();
        while (isdigit(c)) x = 10 * x + c - '0', c = gc();
        return f ? -x : x;
    }
    inline void print(int64_t x) {
        static char buf[64];
        int len = 0;
        if (x < 0) pc('-'), x = -x;
        do { buf[++len] = x % 10 + '0', x /= 10; } while (x);
        while (len) pc(buf[len--]);
    }
    inline void print(int64_t x, char ch) {
        print(x), pc(ch);
    }
    struct Flusher {
        ~Flusher() {
            if (oT) fwrite(obuf, 1, oT, stdout);
        }
    } flusher;
}

void sc() {
    long long n = IO::read(), k = IO::read();
    if (k == 1) {
        IO::print(0, '\n');
        return;
    }
    vector<long long> powers;
    {
        long long p = k;
        while (p < 2 * n) {
            powers.push_back(p);
            if ((__int128)p * k >= 2 * n) break;
            p *= k;
        }
        reverse(powers.begin(), powers.end());
    }

    long long ans = 0, last = n + 1;

    for (auto p : powers) {
        long long R = min(last - 1, p - 1);
        long long L = p - R;
        R = (L + R - 1) / 2;
        if (L <= R) {
            // del [L, R]
            ans += R - L + 1;
            // printf("L = %lld, R = %lld\n", L, R);
            last = L;
        }
    }

    IO::print(ans, '\n');
}

int main() {
    int T; scanf("%d", &T);
    while (T--) sc();
    return 0;
}

// int main() {
//     for (int n = 1; n <= 16; n++) {
//         for (int k = 3; k <= 3; k++) {
//             set<int> S;
//             for (int p = k; p <= n + n; p *= k) S.insert(p);
//             int ban = 1e9, tt;
//             for (int zip = 0; zip < 1 << n; zip++) {
//                 int del = 0;
//                 vector<int> orz;
//                 for (int i = 0; i < n; i++) {
//                     if (zip >> i & 1) {
//                         orz.push_back(i + 1);
//                     }
//                     else {
//                         del++;
//                     }
//                 }
//                 bool ok = 1;
//                 for (auto x : orz)
//                     for (auto y : orz)
//                         if (x != y && S.count(x + y))
//                             ok = 0;
//                 if (ok && del <= ban) {
//                     ban = del;
//                     tt = zip;
//                 }
//             }
//             printf("n = %d, k = %d, ban = %d, tt = %d\n", n, k, ban, tt);
//         }
//     }
// }