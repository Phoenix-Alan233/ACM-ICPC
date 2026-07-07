#include <bits/stdc++.h>
using namespace std;

#define ll long long

const ll LIM = 1ll << 31;

ll lcm(ll a, ll b) { return a / __gcd(a, b) * b; }

// reach[k][cost] 表示花费 cost 的代价, 能得到的最大 lcm (即模数)
// 查询答案, 就是找到最小的 cost, 满足 reach[k][cost] + k - 1 >= n
int K, limcost;
ll reach[2005][101];
void dfs(vector<int> &candidate, int pos, ll mod, int cost) {
    if (reach[K][cost] < mod) reach[K][cost] = mod;
    if (cost >= limcost) return;
    if (mod >= LIM) {
        limcost = min(limcost, cost);
        return;
    }
    if (reach[K][cost] > mod && reach[K][cost] % mod == 0) return;

    if (pos == candidate.size()) return;
    // not take
    dfs(candidate, pos + 1, mod, cost);
    // take
    ll new_mod = lcm(mod, candidate[pos]);
    if (new_mod != mod)
        dfs(candidate, pos + 1, new_mod, cost + K % candidate[pos]);
}

void sc() {
    ll n, k;
    scanf("%lld%lld", &n, &k);
    n = n - k + 1;
    if (k <= 2000) {
        int pos = lower_bound(reach[k], reach[k] + 101, n) - reach[k];
        if (pos == 101) pos = -1;
        printf("%d\n", pos); 
    } else { // 答案 <= 3
        if (n <= k) puts("0");
        else if (n <= k * (k - 1)) puts("1");
        else puts("3");
    }
}

int main() {
    for (int k = 1; k <= 2000; k++) {
        vector<int> candidate;
        for (int t = k; t > k / 2 && t >= k - 25; t--) candidate.push_back(t);
        K = k, limcost = 100;
        dfs(candidate, 0, 1, 0);
        for (int j = 1; j <= 100; j++) {
            reach[k][j] = max(reach[k][j], reach[k][j - 1]);
        }
    }
    cerr << "time used = " << clock() / 1. / CLOCKS_PER_SEC << endl;

    int T; scanf("%d", &T);
    while (T--) sc();
    return 0;
}