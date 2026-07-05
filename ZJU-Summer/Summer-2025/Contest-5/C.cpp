#pragma GCC optimize(2, 3, "Ofast")
#include <bits/stdc++.h>
using namespace std;

namespace IO {
    const int SIZE = 1 << 20;
    char ibuf[SIZE]; int iS, iT;
    char obuf[SIZE]; int oT;
    inline char gc() {
        if (iS == iT) iS = 0, iT = fread(ibuf, 1, SIZE, stdin);
        return iS == iT ? EOF : ibuf[iS++];
    }
    inline void pc(char c) {
        obuf[oT++] = c;
        if (oT == SIZE) fwrite(obuf, 1, SIZE, stdout), oT = 0;
    }
    inline int read() {
        int x = 0, f = 0;
        char c = gc();
        while (!isdigit(c)) f |= c == '-', c = gc();
        while (isdigit(c)) x = 10 * x + c - '0', c = gc();
        return f ? -x : x;
    }
    inline void print(int64_t x) {
        static char buf[64];
        int len = 0;
        if (x < 0) pc('-'), x = -x;
        do { buf[++len] = x % 10 + '0', x /= 10; } while (x);
        while (len) pc(buf[len--]);
    }
    inline void print(int64_t x, char ch) {
        print(x), pc(ch);
    }
    struct Flusher {
        ~Flusher() {
            if (oT) fwrite(obuf, 1, oT, stdout);
        }
    } flusher;
}

const int N = 300005;

vector<int> inz[N];
vector<int> candidate;
int a[N], ans[N], n;

void init(int n) {
    static bool vis[N];
    for (int i = 2; i <= n; i++) {
        if (!vis[i]) {
            inz[i].push_back(i);
            for (int j = i + i; j <= n; j += i) {
                inz[j].push_back(i);
                vis[j] = 1;
            }
        }
    }
}

int bucket[N];
void dfs1(vector<int> &inz, int ind, int num, int coef) {
    if (ind == inz.size()) {
        bucket[num] += coef;
        return;
    }
    dfs1(inz, ind + 1, num, coef);
    dfs1(inz, ind + 1, num * inz[ind], coef);
}

int cnt;
void dfs2(vector<int> &inz, int ind, int num, int mu) {
    if (ind == inz.size()) {
        cnt += mu * bucket[num];
        return;
    }
    dfs2(inz, ind + 1, num, mu);
    dfs2(inz, ind + 1, num * inz[ind], -mu);
}

// 查询 ask (下标集合) 的答案，且答案是 candidate[L..R] 中的某一个 
void solve(int L, int R, vector<int> ask) {
    if (ask.empty()) return;
    if (L == R) {
        for (auto ind : ask) {
            ans[ind] = candidate[L];
        }
        return;
    }
    // printf("solve [%d, %d], ask = ", L, R);
    // for (auto it : ask) printf("%d ", it);
    // printf("\n");
    int mid = (L + R) / 2;
    for (int i = mid + 1; i <= R; i++) {
        dfs1(inz[candidate[i]], 0, 1, 1);
    }
    vector<int> askL, askR;
    for (auto it : ask) {
        cnt = 0;
        dfs2(inz[a[it]], 0, 1, 1);
        if (cnt > 0) askR.push_back(it);
        else askL.push_back(it);
    }
    for (int i = mid + 1; i <= R; i++) {
        dfs1(inz[candidate[i]], 0, 1, -1);
    }
    solve(L, mid, askL);
    solve(mid + 1, R, askR);
}

int main() {
    n = IO::read();

    init(300000);
    for (int i = 0; i < n; i++) {
        a[i] = IO::read();
        candidate.push_back(a[i]);
    }
    sort(candidate.begin(), candidate.end());
    candidate.erase(unique(candidate.begin(), candidate.end()), candidate.end());
    
    vector<int> ask;
    for (int i = 0; i < n; i++) {
        ask.push_back(i);
    }
    solve(0, candidate.size() - 1, ask);

    for (int i = 0; i < n; i++) {
        IO::print(ans[i]);
        IO::pc(" \n"[i == n - 1]);
    }
    return 0;
}

/*
6
1 3 6 4 2 4

6 4 1 3 3 3
*/

/*
4
1 6 10 15

15 1 1 1
*/