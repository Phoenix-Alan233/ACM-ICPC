#include <bits/stdc++.h>
using namespace std;

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        int g = __gcd(n, m);
        cout << min(g, n / g) << endl;
    }
    return 0;
}