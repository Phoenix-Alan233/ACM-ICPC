#include <bits/stdc++.h>
using namespace std;

const int mod = 998244353;

void add(int &x, int y) {
    if ((x += y) >= mod) x -= mod;
}

// 对于每行, 记录是否出现过 0 (0/1), 是否出现过 1 (0/1), 要求之后都 0或1 吗 (0/1/2)
int f[1005][12 * 12 * 12];
int n, m;

void n3_solve() {
    static bool good[8];
    for (int zip = 0; zip < 8; zip++) {
        int k1 = zip & 1, k2 = (zip >> 1) & 1, k3 = (zip >> 2) & 1;
        if (k1 != k2 || k2 != k3) {
            good[zip] = 1;
        }
    }
    // 第一列
    for (int zip = 0; zip < 8; zip++) {
        int k = zip >> 1 & 1;
        if (k == 1) {
            f[1][2 + 2 * 4]++;
        } else {
            f[1][1 + 2 * 4]++;
        }
    }
    // 第 2~(m-1) 列
    for (int i = 2; i <= m - 1; i++) {
        for (int j = 0; j < 12; j++) {
            for (int zip = 0; zip < 8; zip++) {
                int k = zip >> 1 & 1, demand = j >> 2;
                if (demand != 2 && k != demand) continue;
                if (good[zip]) {
                    int new_j = j | (1 << k);
                    add(f[i][new_j], f[i - 1][j]);
                } else {
                    if (!(j >> k & 1)) {
                        int new_j = j | (1 << k);
                        add(f[i][new_j], f[i - 1][j]);
                    } else {
                        int new_j = (j & 3) | (1 << k) | ((k ^ 1) << 2);
                        add(f[i][new_j], f[i - 1][j]);
                    }
                }
            }
        }
    }
    // 第 m 列
    int ans = 0;
    for (int j = 0; j < 12; j++) {
        for (int zip = 0; zip < 8; zip++) {
            int k = zip >> 1 & 1, demand = j >> 2;
            if (demand != 2 && k != demand) continue;
            add(ans, f[m - 1][j]);
        }
    }
    int total = 1;
    for (int i = 1; i <= n * m; i++) {
        total = 2 * total % mod;
    }
    ans = (total + mod - ans) % mod;
    cout << ans << endl;
}

int main() {
    cin >> n >> m;
    if (n > m) swap(n, m);

    if (n <= 2) {
        cout << 0 << endl;
        return 0;
    }

    if (n == 3) {
        n3_solve();
    }
}