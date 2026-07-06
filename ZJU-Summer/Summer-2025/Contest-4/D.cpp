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

const int N = 100005;

vector<int> adj[N];
int ans[N], n;

bool ban[N];
int sz[N], maxs[N], tot_sz, rt;
void getrt(int u, int fa) {
    sz[u] = 1, maxs[u] = 0;
    for (auto v : adj[u]) {
        if (v == fa || ban[v]) continue;
        getrt(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[maxs[u]]) maxs[u] = v;
    }
    if (max(sz[maxs[u]], tot_sz - sz[u]) * 2 <= tot_sz) rt = u;
}

namespace TREE {
const int M = 18 * N;
int ch[M][2], sz[M], val[M], rt, tot;
void ins(int &u, int bit, int num) {
    if (!u) u = ++tot, ch[u][0] = ch[u][1] = 0, sz[u] = val[u] = 0;
    sz[u]++, val[u] ^= num >> bit;
    if (bit == 18) return;
    int which = num >> bit & 1;
    ins(ch[u][which], bit + 1, num);
}
void add1(int u, int bit) {
    if (!u) return;
    if (bit == 18) return;
    swap(ch[u][0], ch[u][1]);
    add1(ch[u][0], bit + 1);
    val[u] = ((val[ch[u][0]] ^ val[ch[u][1]]) << 1) | (sz[ch[u][1]] & 1 ? 1 : 0);
}
}

vector<int> depth[N];
void dfs1(int u, int fa, int dep) {
    depth[dep].push_back(u);
    TREE::ins(TREE::rt, 0, dep);
    for (auto v : adj[u]) {
        if (v == fa || ban[v]) continue;
        dfs1(v, u, dep + 1);
    }
}

void dfz(int u) {
    ban[u] = 1;
    // printf("dfz %d\n", u);

    // 剔除这部分贡献
    for (auto v : adj[u]) {
        if (ban[v]) continue;
        TREE::rt = TREE::tot = 0;
        dfs1(v, u, 1);
        for (int dep = 1; ; dep++) {
            if (depth[dep].empty()) break;
            TREE::add1(TREE::rt, 0);
            for (auto x : depth[dep]) {
                // printf("del x = %d, val = %d\n", x, TREE::val[TREE::rt]);
                ans[x] ^= TREE::val[TREE::rt];
            }
            depth[dep].clear();
        }
    }

    // 增加这部分贡献
    TREE::rt = TREE::tot = 0;
    depth[0].push_back(u);
    TREE::ins(TREE::rt, 0, 0);
    for (auto v : adj[u]) {
        if (ban[v]) continue;
        dfs1(v, u, 1);
    }
    for (int dep = 0; ; dep++) {
        if (depth[dep].empty()) break;
        if (dep) TREE::add1(TREE::rt, 0);
        for (auto x : depth[dep]) {
            // printf("ins x = %d, val = %d\n", x, TREE::val[TREE::rt]);
            ans[x] ^= TREE::val[TREE::rt];
        }
        depth[dep].clear();
    }
    
    int tmp_tot_sz = tot_sz;
    for (auto v : adj[u]) {
        if (ban[v]) continue;
        tot_sz = sz[v] < sz[u] ? sz[v] : tmp_tot_sz - sz[u];
        getrt(v, u), dfz(rt);
    }
}

int main() {
    n = IO::read();
    for (int i = 1; i < n; i++) {
        int u = IO::read(), v = IO::read();
        adj[u].push_back(v), adj[v].push_back(u);
    }
    tot_sz = n, getrt(1, 0), dfz(rt);
    for (int i = 1; i <= n; i++) {
        IO::print(ans[i], '\n');
    }
    return 0;
}