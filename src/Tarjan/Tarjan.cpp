#include "bits/stdc++.h"

#ifdef local
    #include "custom/debugger.h"
#else
    #define debug(x) 42;
#endif

using namespace std;
#define int long long

int Time = 0;
int dfs (int node, int p, vector <vector <int>>& adj, vector <int>& visited, vector <int>& first_visit, vector <int>& is_articulation_point, vector <int>& low) {
    int children = 0;

    low[node] = first_visit[node] = ++Time;
    for (int& v : adj[node]) {
        if (v == p) continue;
        if (!first_visit[v]) {
            children++;
            dfs(v, node, adj, visited, first_visit, is_articulation_point, low);
            if (first_visit[node] <= low[v])
                is_articulation_point[node] = true;
            low[node] = min(low[node], low[v]);
        } else
            low[node] = min(low[node], first_visit[v]);
    }
    return children;
}

void get_articulation_points (vector <vector <int>>& adj) {
    int n = adj.size();
    vector <int> is_articulation_point(n);
    vector <int> visited (n, false);
    vector <int> first_visit (n);
    vector <int> low(n);
    Time = 0;

    for (int i = 0; i < n; i++) {
        if (!first_visit[i]) {
            is_articulation_point[i] = dfs (0, -1, adj, visited, first_visit, is_articulation_point, low) > 1;
        }
    }

    printf("Printing articulation points\n");
    for (int i = 0; i < n; i++) {
        if (is_articulation_point[i]) {
            printf("%lld ", i);
        }
    }
    printf("\n");
}

int32_t main () {
    int n, k;
    // get number of vertices
    scanf("%lld%lld", &n, &k);

    vector <vector <int>> adj(n);

    for (int i = 0; i < k; i++) {
        int a, b;
        scanf("%lld%lld", &a, &b);
        // a--; b--;            // do this only if there is no zero based indexing
        adj[a].push_back (b);
        adj[b].push_back (a);
    }

    get_articulation_points(adj);
}

