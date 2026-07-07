#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;
const int mod = 998244353;

vector<int> inz[N];
vector<int> base[N];
int n, vis[N], ans = 1;

void init(int n) {
    static bool vis[N];
    static int cnt = 0;
    for (int i = 2; i <= n; i++) {
        if (!vis[i]) {
            for (int j = i; j <= n; j += i) {
                if (j > i) vis[j] = 1;
                int num = j, ff = 0;
                while (num % i == 0) ff ^= 1, num /= i;
                if (ff) {
                    inz[j].push_back(i);
                    cnt++;
                }
            }
        }
    }
    cerr << "cnt = " << cnt << endl;
}

vector<int> XOR(vector<int> a, vector<int> b) {
    vector<int> c;
    int i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] == b[j]) i++, j++;
        else if (a[i] < b[j]) c.push_back(a[i]), i++;
        else c.push_back(b[j]), j++;
    }
    while (i < a.size()) {
        c.push_back(a[i]), i++;
    }
    while (j < b.size()) {
        c.push_back(b[j]), j++;
    }
    return c;
}

int main() {
    init(1e6);
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        bool ok = 0;
        vector<int> cur = inz[x];
        while (!cur.empty()) {
            if (vis[x]) break;
            int t = cur.back();
            if (!base[t].empty()) cur = XOR(cur, base[t]);
            else {
                base[t].swap(cur);
                ok = 1;
                break;
            }
        }
        vis[x] = 1;
        if (!ok) {
            ans = 2 * ans % mod;
        }
        printf("%d%c", ans, " \n"[i == n]);
    }
    return 0;
}