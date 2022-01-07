#include "bits/stdc++.h"

#ifdef local
    #include "custom/debugger.h"
#else
    #define debug(x) 42;
#endif

using namespace std;
#define int long long

void simple_dfs (int node, vector <vector <int>>& adj, vector <bool>& visited) {
    if (visited[node]) return;
    visited[node] = true;
    for (auto u: adj[node]) {
        simple_dfs(u, adj, visited);
    }
}

int check_Euler (vector <vector <int>>& adj) {
    // check connectivity
    int n = adj.size();
    vector <bool> visited(n, false);
    simple_dfs(0, adj, visited);
    bool connected = true;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) connected = false;
    }

    if (!connected) return 0;

    int odd_cnt = 0;

    for (int i = 0; i < n; i++) {
        if ((int) adj[i].size() % 2 == 1) {
            odd_cnt++;
        }
    }

    if (odd_cnt > 2) return 0;

    return odd_cnt ? 1 : 2;

}

void remove_edge(int u, int v, vector <vector <int>>& adj) {
    // removes an edge between node u and node v
    int pos = -1;
    for (int i = 0; i < (int) adj[u].size(); i++) {
        if (adj[u][i] == v) {
            pos = i;
        }
    }

    adj[u].erase(adj[u].begin() + pos);

    pos = -1;
    for (int i = 0; i < (int) adj[v].size(); i++) {
        if (adj[v][i] == u) {
            pos = i;
        }
    }

    adj[v].erase(adj[v].begin() + pos);
}

int dfsCount (int node, vector <vector <int>>& adj, vector <bool>& visited) {
    int retval = 0;
    if (visited[node]) return retval;
    visited[node] = true;

    retval = 1;
    for (auto u: adj[node]) {
        retval += dfsCount(u, adj, visited);
    }

    return retval;
}

void add_edge (int u, int v, vector <vector <int>>& adj) {
    adj[u].push_back (v);
    adj[v].push_back (u);
}

bool valid_edge (int u, int v, vector <vector <int>>& adj, vector <vector <bool>>& visited) {
    bool valid = false;
    int cnt = 0;

    for (auto x: adj[u]) {
        if (!visited[u][x]) {
            cnt++;
        }
    }

    if (cnt == 1) return true;

    vector <bool> visited2((int) adj.size(), false);
    // check if it's a bridge

    int val = dfsCount (u, adj, visited2);
    remove_edge (u, v, adj);
    visited2.assign ((int) adj.size(), false);

    if (dfsCount (u, adj, visited2) == val) {
        // not a bridge, and therefore valid
        valid = true;
    }

    // add the edge back
    add_edge (u, v, adj);

    return valid;
}

void fleury_dfs (int u, int v, vector <vector <int>>& adj, vector <vector <bool>>& visited, vector <int>& p) {
    visited[u][v] = true;
    visited[v][u] = true;
    p.push_back (v);
    // debug (v);
    // check all other unvisited nodes
    for (auto x: adj[v]) {
        if (!visited[v][x]) {
            if (valid_edge(v, x, adj, visited)) {
                // debug (v, x);
                // debug (v);
                // debug (x);
                // visit this
                fleury_dfs (v, x, adj, visited, p);
            }
        }
    }
    // return the path
}

vector <int> fleury (vector <vector <int>>& adj) {
    vector <int> p;
    int n = adj.size();

    // check whether the graph has 2 or 0 odd vertices
    int cnt_odd = 0;
    int start = 0;
    for (int i = 0; i < n; i++) {
        if (adj[i].size() % 2 == 1) {
            cnt_odd++;
            start = i;
        }
    }

    if (cnt_odd != 2 && cnt_odd != 0) {
        printf("NO EULER CIRCUIT EXISTS\n");
        return p;
    }

    // find the first edge you want to visit
    int sec = -1;
    vector <vector <bool>> visited(n, vector <bool> (n, false));

    for (auto x: adj[start]) {
        if (valid_edge(start, x, adj, visited)) {
            sec = x;
            break;
        }
    }

    // follow edges one at a time. If there's a choice between a bridge and a non-bridge, choose the bridge
    p.push_back (start);
    fleury_dfs(start, sec, adj, visited, p);
    // debug (p.size());

    return p;
}

vector <int> chinese_postman (vector <vector <int>>& adj) {
    vector<int> p;
    if (check_Euler(adj) != 0) {
        // using fleury's algorithm to return an eulerian circuit
        p = fleury(adj);
    }
    return p;
}

int32_t main () {
    int n, k;
    // get number of vertices
    scanf("%lld%lld", &n, &k);

    vector <vector <int>> adj(n);

    for (int i = 0; i < k; i++) {
        int a, b;
        scanf("%lld%lld", &a, &b);
        adj[a].push_back (b);
        adj[b].push_back (a);
    }

    vector<int> p = chinese_postman(adj);

    if ((int)p.size() > 0) {
        printf("Printing path:\n");
        for (int i = 0; i < (int) p.size(); i++) {
            printf("%lld ", p[i]);
        }
        printf("\n");
    }
}

