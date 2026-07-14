#include <bits/stdc++.h>
using namespace std;

const int N = 100005;

vector<int> adj[N];
int dad[N], n, m;

// col[u]: 1 为黑, 2 为白
int col[N];

void GetTree() {
    queue<int> q;
    col[1] = 1, dad[1] = 0;
    for (auto it : adj[1]) {
        col[it] = 2, dad[it] = 1;
        q.push(it);
    } 

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto v : adj[u]) {
            if (col[v]) continue;
            col[v] = 1, dad[v] = u;
            for (auto to : adj[v]) {
                if (col[to]) {
                    assert(col[to] == 2);
                    continue;
                }
                col[to] = 2, dad[to] = v;
                q.push(to);
            }
        }
    }
}

vector<int> G[N], cyc;
bool HasOddCycle;
int vis[N], Gdad[N];
void dfs(int u, int col) {
    vis[u] = col;
    for (auto v : G[u]) {
        if (!~vis[v]) {
            Gdad[v] = u;
            dfs(v, col ^ 1);
        } else if (vis[v] == col && !HasOddCycle) {
            HasOddCycle = true;
            for (int i = u; i != v; i = Gdad[i]) cyc.push_back(i);
            cyc.push_back(v);
        }
    }
}

int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    GetTree();
    // for (int i = 1; i <= n; i++) {
    //     printf("col[%d] = %d\n", i, col[i]);
    // }

    for (int i = 1; i <= n; i++) {
        for (auto j : adj[i]) {
            if (dad[j] != i && dad[i] != j) {
                G[i].push_back(j);
                G[j].push_back(i);
            }
        }
    }
    
    memset(vis, -1, sizeof(vis));

    for (int i = 1; i <= n; i++) {
        if (!~vis[i]) {
            dfs(i, 0);
        }
    }
    
    if (HasOddCycle) {
        printf("2\n");
        printf("%d ", cyc.size());
        for (auto it : cyc) printf("%d ", it);
        return 0;
    }
    
    printf("1\n");
    for (int i = 1; i <= n; i++) {
        if (col[i] == 1) ;
        else if (!vis[i]) col[i] = 2;
        else col[i] = 3;
        printf("%d ", col[i]);
    }
    puts("");
    return 0;
}