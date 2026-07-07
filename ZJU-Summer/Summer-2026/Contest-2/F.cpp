#include <bits/stdc++.h>
using namespace std;

const int N = 200005;
const int inf = 1e9;

vector<int> adj[N];
vector<pair<int, int>> query[N];
int a[N], answer[N], n, q;

int dfn[N], idfn[N], dad[N], sz[N], dfn_tot;
int bl[N], son[N];

void dfs1(int u) {
    sz[u] = 1;
    for (auto v : adj[u]) {
        dad[v] = u;
        dfs1(v);
        sz[u] += sz[v];
        if (sz[v] > sz[son[u]]) son[u] = v;
    }
}

void dfs2(int u, int topf) {
    dfn[u] = ++dfn_tot, idfn[dfn_tot] = u;
    bl[u] = topf;
    if (son[u]) dfs2(son[u], topf);
    for (auto v : adj[u]) {
        if (v == son[u]) continue;
        dfs2(v, v);
    }
}

// 树剖后, 记录 a[u]' 的最小值和最小值所在的位置
struct SMT1 {
    #define ls (u << 1)
    #define rs (u << 1 | 1)
    int minn[N << 2], minpos[N << 2], tag[N << 2];

    void merge(int &v, int &p, int v1, int p1, int v2, int p2) {
        if (v1 < v2) v = v1, p = p1;
        else v = v2, p = p2;
    }

    void pushup(int u) {
        merge(minn[u], minpos[u], minn[ls], minpos[ls], minn[rs], minpos[rs]);
    }

    void pushdown(int u) {
        if (tag[u]) {
            minn[ls] += tag[u], minn[rs] += tag[u];
            tag[ls] += tag[u], tag[rs] += tag[u];
            tag[u] = 0;
        }
    }

    void build(int u, int l, int r) {
        if (l == r) {
            minn[u] = a[idfn[l]];
            minpos[u] = -l;
            return;
        }
        int mid = l + r >> 1;
        build(ls, l, mid), build(rs, mid + 1, r);
        pushup(u);
    }

    void update(int u, int l, int r, int ql, int qr, int v) {
        if (ql <= l && r <= qr) {
            minn[u] += v;
            tag[u] += v;
            return;
        }
        pushdown(u);
        int mid = l + r >> 1;
        if (ql <= mid) update(ls, l, mid, ql, qr, v);
        if (mid < qr) update(rs, mid + 1, r, ql, qr, v);
        pushup(u);
    }

    pair<int, int> query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return {minn[u], minpos[u]};
        pushdown(u);
        int mid = l + r >> 1;
        pair<int, int> res = {inf, inf};
        if (ql <= mid) res = min(res, query(ls, l, mid, ql, qr));
        if (mid < qr) res = min(res, query(rs, mid + 1, r, ql, qr));
        return res;
    }
} smt1;

// 记录区间编号 min, 用于剔除子树中的最小点
struct SMT2 {
    #define ls (u << 1)
    #define rs (u << 1 | 1)
    int minn[N << 2];
    void build(int u, int l, int r) {
        minn[u] = inf;
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(ls, l, mid), build(rs, mid + 1, r);
    }
    void update(int u, int l, int r, int pos, int num) {
        if (l == r) {
            minn[u] = num;
            return;
        }
        int mid = l + r >> 1;
        if (pos <= mid) update(ls, l, mid, pos, num);
        else update(rs, mid + 1, r, pos, num);
        minn[u] = min(minn[ls], minn[rs]);
    }
    int query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return minn[u];
        int mid = l + r >> 1, res = inf;
        if (ql <= mid) res = min(res, query(ls, l, mid, ql, qr));
        if (mid < qr) res = min(res, query(rs, mid + 1, r, ql, qr));
        return res;
    }
} smt2;

// 用于记录哪些点被取了, 方便计算答案
struct Fenwick {
    int c[N];
    void add(int x, int v) {
        for (; x <= n; x += x & -x) c[x] += v;
    }
    int query(int x) {
        int res = 0;
        for (; x; x -= x & -x) res += c[x];
        return res;
    }
} tr;

pair<int, int> Q(int u) {
    pair<int, int> ans = {inf, inf};
    while (u) {
        ans = min(ans, smt1.query(1, 1, n, dfn[bl[u]], dfn[u]));
        u = dad[bl[u]];
    }
    return ans;
}
void U(int u, int delta) {
    while (u) {
        smt1.update(1, 1, n, dfn[bl[u]], dfn[u], delta);
        u = dad[bl[u]];
    }
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
    }
    for (int i = 2; i <= n; i++) {
        int fa; scanf("%d", &fa);
        adj[fa].push_back(i);
    }
    for (int i = 1; i <= q; i++) {
        int l, r; scanf("%d%d", &l, &r);
        query[r].push_back({l, i});
    }

    dfs1(1), dfs2(1, 1);
    smt1.build(1, 1, n);
    smt2.build(1, 1, n);

    for (int u = 1; u <= n; u++) {
        pair<int, int> tmp = Q(u);
        tmp.second = -tmp.second;
        // printf("u = %d, tmp = (%d, %d)\n", u, tmp.first, tmp.second);
        if (tmp.first >= 1) {
            // printf("[INFO] No need to update for node %d, tmp.first = %d\n", u, tmp.first);
            U(u, -1);
            smt2.update(1, 1, n, dfn[u], u);
            tr.add(u, 1);
        } else {
            int badnode = idfn[tmp.second];
            int take = smt2.query(1, 1, n, dfn[badnode], dfn[badnode] + sz[badnode] - 1);
            // printf("[INFO] For node %d, badnode = %d, take = %d\n", u, badnode, take);
            if (take != inf) {
                U(take, 1);
                smt2.update(1, 1, n, dfn[take], inf);
                tr.add(take, -1);

                U(u, -1);
                smt2.update(1, 1, n, dfn[u], u);
                tr.add(u, 1);
            }
        }
        for (auto [l, id] : query[u]) {
            answer[id] = tr.query(u) - tr.query(l - 1);
        }
    }

    for (int i = 1; i <= q; i++) {
        printf("%d\n", answer[i]);
    }
    return 0;
}