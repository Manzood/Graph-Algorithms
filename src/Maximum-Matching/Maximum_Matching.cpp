#include "bits/stdc++.h"
using namespace std;
#define debug(x) cout << #x << " = " << x << endl;
#define int long long

// NOTES:
// Input a bipartite graph
// Start with an empty matching
// Find augmenting paths: If there is a new path to be found, add it to the matching
// The moment you have a start and end vertex of a path that does not belong to the current matching, we've found a new path and should add it *immediately*



bool dfs (int node, vector <vector <int>>& adj, vector <bool>& visited, vector <int>& matching) {
    if (visited[node])
        return false;
    visited[node] = true;
    for (int to : adj[node]) {
        if (matching[to] == -1 || dfs(matching[to], adj, visited, matching)) {
            matching[to] = node;
            return true;
        }
    }
    return false;
}

void maximum_matching (vector <vector <int>>& adj, vector <int>& label) {
    int n = adj.size();
    vector <int> matching(n, -1);
    for (int i = 0; i < n; i++) {
        if (label[i] == 0) {
            vector <bool> visited(n, false);
            dfs (i, adj, visited, matching);
        }
    }

    // debug ("printing entire array");
    // for (int i = 0; i < n; i++) {
        // printf("%lld %lld\n", i + 1, matching[i] + 1);
    // }
    // printf("Printing Matching\n");
    for (int i = 0; i < n; i++) {
        if (label[i] == 1) {
            // debug (i);
            if (matching[i] != -1) {
                printf("%lld %lld\n", matching[i] + 1, i + 1);
            }
        }
    }
}

void label_bipartite_graph (int node, int dist, vector <vector <int>>& adj, vector <bool>& visited, vector <int>& label) {
    visited[node] = true;
    label[node] = dist % 2;

    for (auto u: adj[node]) {
        if (!visited[u]) {
            label_bipartite_graph (u, dist + 1, adj, visited, label);
        }
    }
}

void remove_edges(vector<int>& p, vector<vector<int>>& adj) {
    for (int i = 1; i < (int) p.size(); i++) {
        int a = p[i-1];
        int b = p[i];
        int ind = -1;
        for (int j = 0; j < (int) adj[a].size(); j++) {
            if (adj[a][j] == b) {
                ind = j;
            }
        }
        adj[a].erase (adj[a].begin() + ind);
        ind = -1;
        for (int j = 0; j < (int) adj[b].size(); j++) {
            if (adj[b][j] == a) {
                ind = j;
            }
        }
        adj[b].erase (adj[b].begin() + ind);
    }
}

int32_t main () {
    int n, k;
    scanf("%lld%lld", &n, &k);
    vector <vector <int>> adj(n);

    for (int i = 0; i < k; i++) {
        int a, b;
        scanf("%lld%lld", &a, &b);
        a--; b--;
        adj[a].push_back (b);
        adj[b].push_back (a);
    }

    vector <int> label(n, -1);
    vector <bool> visited(n, false);
    for (int i = 0; i < n; i++) {
        if (!visited[i]) label_bipartite_graph(i, 0, adj, visited, label);
    }
    // for (int i = 0; i < n; i++) {
        // printf("%lld %lld\n", i + 1, label[i]);
    // }
    // printf("\n");
    maximum_matching (adj, label);
}

