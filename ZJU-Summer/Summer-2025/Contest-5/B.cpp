#include <bits/stdc++.h>
using namespace std;

int n;

int main() {
    ios::sync_with_stdio(false), cin.tie(0);

    cin >> n;
    vector<pair<int, int>> a;
    for (int i = 0; i < 2 * n; i++) {
        int x;
        cin >> x;
        a.push_back({x, (i < n ? 0 : 1)});
    }
    sort(a.begin(), a.end());

    long long ans = 3e18, offset = a[2 * n - 1].first, cur = 0;
    for (int i = 0; i < 2 * n; i++) {
        int pos = a[i].first, type = a[i].second;
        if (type == 0) {
            cur++;
        } else {
            cur--;
        }
        if (cur > 0) {
            ans = min(ans, offset + (a[2 * n - 1].first - pos));
            offset += 2ll * (a[i + 1].first - pos);
        }
    }
    ans = min(ans, offset);
    cout << ans << endl;
    return 0;
}
