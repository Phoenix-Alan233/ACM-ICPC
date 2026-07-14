#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int N = 10005;

int a[N];
int n;

ll solve(int l, int r) {
    if (l > r) {
        return 0;
    }
    if (l == r) {
        a[l] = 1;
        return 1;
    }
    int mid = (l + r) / 2;
    ll sum = solve(l, mid - 1) + solve(mid + 1, r);
    a[mid] = sum + 1;
    sum += a[mid];
    return sum;
}

void sc() {
    cin >> n;
    solve(1, n);
    for (int i = 1; i <= n; i++)
        cout << a[i] << " \n"[i == n];
}

int main() {
    int T; cin >> T;
    while (T--) sc();
    return 0;
}