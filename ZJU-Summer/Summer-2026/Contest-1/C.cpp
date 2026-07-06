#include <bits/stdc++.h>
using namespace std;

const int N = 500005;

int p[N], n;

const int mod1 = 998244353;
const int mod2 = 1e9 + 7;

struct mint {
    int x, y;
    mint(int _x = 0, int _y = 0): x(_x), y(_y) {}
};

bool operator==(const mint &a, const mint &b) {
    return a.x == b.x && a.y == b.y;
}

mint operator+(const mint &a, const mint &b) {
    return mint((a.x + b.x) % mod1, (a.y + b.y) % mod2);
}

mint operator*(const mint &a, const mint &b) {
    return mint(1ll * a.x * b.x % mod1, 1ll * a.y * b.y % mod2);
}

void operator+=(mint &a, const mint &b) {
    a = a + b;
}

struct SegNode {
    int maxx;
    mint maxx_ways;
    SegNode() { maxx = 0, maxx_ways = mint(0, 0); }
    SegNode(int _maxx, mint _maxx_ways): maxx(_maxx), maxx_ways(_maxx_ways) {} 
} tr[N << 2];

SegNode merge(const SegNode &lhs, const SegNode &rhs) {
    if (lhs.maxx > rhs.maxx) {
        return lhs;
    } else if (lhs.maxx < rhs.maxx) {
        return rhs;
    } else {
        return SegNode(lhs.maxx, lhs.maxx_ways + rhs.maxx_ways);
    }
}

struct SegmentTree {
    #define ls (u << 1)
    #define rs (u << 1 | 1)

    void build(int u, int l, int r) {
        tr[u].maxx = 0, tr[u].maxx_ways = mint(r - l + 1, r - l + 1);
        if (l == r) return;
        int mid = l + r >> 1;
        build(ls, l, mid), build(rs, mid + 1, r);
    }

    void update(int u, int l, int r, int pos, SegNode value) {
        if (l == r) {
            tr[u] = value;
            return;
        }
        int mid = l + r >> 1;
        if (pos <= mid) update(ls, l, mid, pos, value);
        else update(rs, mid + 1, r, pos, value);
        tr[u] = merge(tr[ls], tr[rs]);
    }

    SegNode query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[u];
        int mid = l + r >> 1;
        if (qr <= mid) return query(ls, l, mid, ql, qr);
        else if (ql > mid) return query(rs, mid + 1, r, ql, qr);
        else return merge(query(ls, l, mid, ql, qr),
                          query(rs, mid + 1, r, ql, qr));
    }
} smt;

// pf[i] 表示 p[1..i] LIS, 以及 LIS 方案数
// sf[i] 表示 p[i..n] LIS, ...
// pg[i] 表示 p[1..i] LDS, ...
// sg[i] 表示 p[i..n] LDS, ...
SegNode pf[N], sf[N], pg[N], sg[N];

void sc() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> p[i];
    }

    smt.build(1, 1, n);
    for (int i = 1; i <= n; i++) {
        pf[i] = smt.query(1, 1, n, 1, p[i]);
        if (pf[i].maxx == 0) {
            pf[i] = SegNode(1, mint(1, 1));
        } else {
            pf[i].maxx++;
        }
        smt.update(1, 1, n, p[i], pf[i]);
    }
    smt.build(1, 1, n);
    for (int i = 1; i <= n; i++) {
        pg[i] = smt.query(1, 1, n, p[i], n);
        if (pg[i].maxx == 0) {
            pg[i] = SegNode(1, mint(1, 1));
        } else {
            pg[i].maxx++;
        }
        smt.update(1, 1, n, p[i], pg[i]);
    }
    smt.build(1, 1, n);
    for (int i = n; i >= 1; i--) {
        sf[i] = smt.query(1, 1, n, p[i], n);
        if (sf[i].maxx == 0) {
            sf[i] = SegNode(1, mint(1, 1));
        } else {
            sf[i].maxx++;
        }
        smt.update(1, 1, n, p[i], sf[i]);
    }
    smt.build(1, 1, n);
    for (int i = n; i >= 1; i--) {
        sg[i] = smt.query(1, 1, n, 1, p[i]);
        if (sg[i].maxx == 0) {
            sg[i] = SegNode(1, mint(1, 1));
        } else {
            sg[i].maxx++;
        }
        smt.update(1, 1, n, p[i], sg[i]);
    }

    int LIS = 0, LDS = 0;
    mint total_LIS(0, 0), total_LDS(0, 0);
    mint total_ways(0, 0), intersect_ways(0, 0);

    for (int i = 1; i <= n; i++) {
        if (pf[i].maxx > LIS)
            LIS = pf[i].maxx;
        if (pg[i].maxx > LDS)
            LDS = pg[i].maxx;
    }
    for (int i = 1; i <= n; i++) {
        if (pf[i].maxx == LIS)
            total_LIS += pf[i].maxx_ways;
        if (pg[i].maxx == LDS)
            total_LDS += pg[i].maxx_ways;
    }
    total_ways = total_LIS * total_LDS;

    // printf("total_ways = (%d, %d)\n", total_ways.x, total_ways.y);

    for (int i = 1; i <= n; i++) {
        if (pf[i].maxx + sf[i].maxx - 1 == LIS &&
            pg[i].maxx + sg[i].maxx - 1 == LDS) {
            intersect_ways += pf[i].maxx_ways * sf[i].maxx_ways * 
                              pg[i].maxx_ways * sg[i].maxx_ways;
        } 
    }

    // printf("intersect_ways = (%d, %d)\n", intersect_ways.x, intersect_ways.y);

    if (total_ways == intersect_ways) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
    }
}

int main() {
    ios::sync_with_stdio(false), cin.tie(0);
    int T; cin >> T;
    while (T--) sc();
    return 0;
}