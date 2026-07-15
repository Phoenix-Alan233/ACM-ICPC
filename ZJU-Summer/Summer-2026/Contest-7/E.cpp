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

#define ll long long

const int N = 1000005;
const ll inf = 6e18;

// 0: 左侧大炮, 1: 右侧大炮, 2: 新建大炮
ll dp[N][3];
int m[N], e[N];
int M, E, C, L;

struct Fenwick {
    ll c[N];
    int n;
    void init(int _n) {
        n = _n;
        for (int i = 1; i <= n; i++) c[i] = inf;
    }
    void ins(int x, ll dlt) {
        ++x;
        while (x > 0) {
            c[x] = min(c[x], dlt);
            x ^= x & -x;
        }
    }
    ll qry(int x) {
        ++x;
        ll ret = inf;
        while (x <= n) {
            ret = min(ret, c[x]);
            x += x & -x;
        }
        return ret;
    }
} tr0;

int main() {
    M = IO::read(), E = IO::read(), C = IO::read(), L = IO::read();

    for (int i = 1; i <= M; i++) {
        m[i] = IO::read();
        assert(m[i] >= m[i - 1]);
    }
    for (int i = 1; i <= E; i++) {
        e[i] = IO::read();
        assert(e[i] >= e[i - 1]);
    }

    // 清除一些与机器位置重合的怪
    {
        int ptr = 1, j = 0;
        for (int i = 1; i <= E; i++) {
            while (ptr <= M && m[ptr] < e[i]) ++ptr;
            if (ptr <= M && m[ptr] == e[i]) ;
            else e[++j] = e[i];
        }
        E = j;
    }

    dp[0][0] = dp[0][1] = inf;
    for (int i = 1; i <= E; i++) {
        dp[i][0] = dp[i][1] = dp[i][2] = inf;
    }
    tr0.init(E + 1);
    tr0.ins(0, -e[1]);

    for (int i = 1; i <= E; i++) {
        int nxt = lower_bound(m + 1, m + M + 1, e[i]) - m;
        int pre = nxt - 1;
        assert(m[pre] <= e[i]);
        // 用右侧大炮
        if (nxt != M + 1 && (i == E || e[i + 1] > m[nxt]) && m[nxt] - L <= e[i]) {
            int pos = lower_bound(e + 1, e + E + 1, max(m[pre], m[nxt] - L)) - e;
            if (pos <= i) {
                // 用 m[nxt] 攻击 [j, i]
                dp[i][1] = tr0.qry(pos - 1) + m[nxt];
                // for (int j = i; j >= pos; j--) {
                //     dp[i][1] = min(dp[i][1], min(dp[j - 1][0], dp[j - 1][2]) + m[nxt] - e[j]);
                // }
                if (e[pos - 1] <= m[pre]) {
                    dp[i][1] = min(dp[i][1], dp[pos - 1][1] + m[nxt] - e[pos]);
                }
            }
        }
        // 用左侧大炮
        if (pre != 0 && e[i] - m[pre] <= L) {
            int pos = lower_bound(e + 1, e + E + 1, m[pre]) - e;
            // 贪心选择 [pos, i]
            dp[i][0] = min(dp[i][0], min(dp[pos - 1][0], dp[pos - 1][2]) + e[i] - m[pre]);
            if (e[pos - 1] <= m[pre - 1])
                dp[i][0] = min(dp[i][0], dp[pos - 1][1] + e[i] - m[pre]);
        }
        // 用新建大炮
        int pos = lower_bound(e + 1, e + E + 1, max(m[pre], e[i] - L)) - e;
        if (pos <= i) {
            dp[i][2] = tr0.qry(pos - 1) + e[i] + C;
            // for (int j = i; j >= pos; j--) {
            //     dp[i][2] = min(dp[i][2], min(dp[j - 1][0], dp[j - 1][2]) + e[i] - e[j] + C);
            // }
            if (pre && e[pos - 1] <= m[pre]) {
                dp[i][2] = min(dp[i][2], dp[pos - 1][1] + e[i] - e[pos] + C);
            }
        }

        tr0.ins(i, min(dp[i][0], dp[i][2]) - e[i + 1]);
        // printf("dp[%d] = {%lld, %lld, %lld}\n", i, dp[i][0], dp[i][1], dp[i][2]);
    }
    printf("%lld\n", min({dp[E][0], dp[E][1], dp[E][2]}));
    return 0;
}