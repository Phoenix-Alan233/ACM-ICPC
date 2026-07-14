#include <bits/stdc++.h>
using namespace std;

int n;

void sc() {
    scanf("%d", &n);

    vector<int> S(n);
    iota(S.begin(), S.end(), 1);

    vector<int> result;

    while (!S.empty()) {
        int l = 0, r = S.size();
        while (l + 1 < r) {
            int mid = l + r >> 1;

            // [l, mid)
            vector<int> L = vector<int>(S.begin() + l, S.begin() + mid);
            // [mid, r)
            vector<int> R = vector<int>(S.begin() + mid, S.begin() + r);
            printf("? %d ", L.size());
            for (auto it : L) printf("%d ", it);
            printf("%d ", R.size());
            for (auto it : R) printf("%d ", it);
            puts(""), fflush(stdout);

            int deg_out; scanf("%d", &deg_out);
            int total_deg_out = L.size() * R.size();
            if (2 * deg_out >= total_deg_out) r = mid;
            else l = mid;
        }

        int p = S[l];
        result.push_back(p);

        vector<int> newS;
        for (auto it : S) {
            if (it != p) {
                printf("? 1 %d 1 %d\n", p, it);
                fflush(stdout);

                int _; scanf("%d", &_);
                if (_ == 0) newS.push_back(it);
            }
        }
        S.swap(newS);
    }

    printf("! %d ", result.size());
    for (auto it : result) printf("%d ", it);
    puts(""), fflush(stdout);
}

int main() {
    int T; scanf("%d", &T);
    while (T--) sc();
    return 0;
}