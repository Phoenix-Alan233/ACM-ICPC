#include <bits/stdc++.h>
using namespace std;

void sc() {
    long long n, k;
    scanf("%lld%lld", &n, &k);
    if (k == 1) {
        puts("0");
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

    printf("%lld\n", ans);
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