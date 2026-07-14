#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int N = 100005;

vector<int> adj[N];
int n, m;

int dad[N], dep[N], jp[18][N], heavy[N], sz[N];

void dfs0(int u, int fa) {
    dad[u] = fa, dep[u] = dep[fa] + 1;
    sz[u] = 1;
    
    jp[0][u] = fa;
    for (int i = 1; i <= 17; i++) jp[i][u] = jp[i - 1][jp[i - 1][u]];

    for (auto v : adj[u]) {
        if (v == fa) continue;
        dfs0(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[heavy[u]]) heavy[u] = v;
    }
}
int bl[N], dfn[N], dtot;
void dfs1(int u, int topf) {
    bl[u] = topf, dfn[u] = ++dtot;
    if (heavy[u]) dfs1(heavy[u], topf);
    for (auto v : adj[u]) {
        if (v == dad[u] || v == heavy[u]) continue;
        dfs1(v, v);
    }
}
int jump(int u, int dis) {
    for (int i = 17; i >= 0; i--) {
        if (dis >> i & 1) u = jp[i][u];
    }
    return u;
}

struct Fenwick {
    ll c[N];
    void ins(int x, ll value) {
        while (x <= n) c[x] += value, x += x & -x;
    }
    ll qry(int x) {
        ll ans = 0;
        while (x) ans += c[x], x ^= x & -x;
        return ans;
    }
    ll qry(int l, int r) {
        return qry(r) - qry(l - 1);
    }
} tr;

vector<pair<int, int>> deal[N];
// f[u]: 子树 u 中的最大链和
// g[u]: 算上 (dad[u], u) 这条边，子树 u 中的最大链和
ll f[N], g[N];

// 计算 u->v 链上 f[x]-g[x] 的和
ll calc(int u, int v) {
    ll ans = 0;
    while (bl[u] != bl[v]) {
        if (dep[bl[u]] < dep[bl[v]]) swap(u, v);
        ans += tr.qry(dfn[bl[u]], dfn[u]);
        u = dad[bl[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    ans += tr.qry(dfn[u], dfn[v]);
    return ans;
}

void dfs2(int u, int fa) {
    for (auto v : adj[u]) {
        if (v == fa) continue;
        dfs2(v, u);
        
        g[v] = f[v];
        // u->to 这条链，价值是 w
        for (auto [to, w] : deal[v]) {
            ll tmp = calc(v, to) + f[v] + w;
            if (tmp > g[v]) g[v] = tmp;
        }

        tr.ins(dfn[v], f[v] - g[v]);
        
        f[u] += g[v];
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs0(1, 0), dfs1(1, 1);
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        if (dep[u] > dep[v]) swap(u, v);
        int sonu = jump(v, dep[v] - dep[u] - 1);
        assert(dad[sonu] == u);
        deal[sonu].push_back({v, w});
    }

    dfs2(1, 0);
    cout << f[1] << endl;
    return 0;
}