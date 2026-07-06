#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
const int M = 30 * N;
const int mod = 998244353;

void add(int &x, int y) {
    if ((x += y) >= mod) x -= mod;
}

int a[N], pre[N], n, ans;

int ch[M][2], sz[M], tot, rt;
void ins(int &u, int bit, int num) {
    if (!u) u = ++tot, ch[u][0] = ch[u][1] = sz[u] = 0;
    sz[u]++;
    if (bit == -1) return;
    int which = num >> bit & 1;
    ins(ch[u][which], bit - 1, num); 
}

void qry(int u, int bit, int num) {
    if (!u) return;
    if (bit == -1) return;
    int which = num >> bit & 1;
    add(ans, 1ll * sz[ch[u][which ^ 1]] * (1ll << bit) % mod);
    qry(ch[u][which], bit - 1, num);
}

int main() {
    ios::sync_with_stdio(false), cin.tie(0);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pre[i] = pre[i - 1] ^ a[i];
        add(ans, a[i] % mod); // [i, i]
    }
    
    for (int i = 1; i <= n; i++) {
        if (i >= 2) ins(rt, 29, pre[i - 2]);
        qry(rt, 29, pre[i]);
    }

    cout << ans << endl;
    return 0;
}

// https://www.codechef.com/problems/SEGCOMPR?tab=statement