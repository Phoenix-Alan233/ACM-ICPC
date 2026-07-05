#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;

int a[N], n;

void sc() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    if (n == 2 || n == 3) {
        cout << 1 << endl;
    } else if (n % 2 == 0) {
        cout << n - 3 << endl;
    } else {
        n--;
        long long ans = 0;
        for (int p = 3; p <= n - 3; p++) {
            ans += (n - 1 - p);
        }
        // p=2
        if (n >= 6) {
            ans += (n - 4) / 2 + 2;
        }
        // p=1
        if (n >= 4) {
            ans += 2;
        }
        cout << ans << endl;
    }
}

int main() {
    ios::sync_with_stdio(false), cin.tie(0);
    int T; cin >> T;
    while (T--) sc();
    return 0;
}