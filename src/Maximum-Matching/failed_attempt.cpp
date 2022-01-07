#include "bits/stdc++.h"
using namespace std;
#define debug(x) cout << #x << " = " << x << endl;
#define int long long

// NOTES:
// Input a bipartite graph
// add a source and sink
// connect all edges from one part of the graph to source, the other to sink
// apply ford_fulkerson for max flow
// since incoming for each node = outgoing, this means it will only accept one edge at destinations

bool dfs (int node, int sink, vector <vector <int>>& adj, vector <int>& p, vector <bool>& visited) {
    bool found_path = false;
    visited[node] = true;
    p.push_back (node);

    if (node == sink) {
        return true;
    }
    for (auto u: adj[node]) {
        if (!visited[u]) {
            found_path |= dfs (u, sink, adj, p, visited);
        }
    }
    if (!found_path) {
        p.pop_back();
    }

    return found_path;
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

void remove_edges (vector <int>& p, vector <vector <int>>& adj) {
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

int ford_fulkerson (vector <vector <int>>& adj) {
    // keep doing dfs while removing edges
    // if no more paths exist to destination, you're done
    // first add a source and sink
    // have to break the graph down into its parts

    // one way to do this would be to arbitrarily pick a node and then put all even-distanced vertices in its vector, and odd-distanced vertices in the other one

    int flow = 0;
    int n = adj.size();
    vector <bool> visited(n, false);
    visited.assign (n, false);
    vector <int> p;
    int source = n + 1, sink = n + 2;

    while (dfs(source, sink, adj, p, visited)) {
        // add flow
        flow++;
        // remove this path
        remove_edges(p, adj);
        p.clear();
        visited.assign(n, false);
    }

    return flow;
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

    vector <int> label(n);
    vector <bool> visited(n, false);
    label[0] = 0;

    label_bipartite_graph(0, 0, adj, visited, label);
    adj.resize (n + 2);
    for (int i = 0; i < n; i++) {
        if (label[i] == 0) {
            adj[n].push_back (i);
            adj[i].push_back (n);
        } else {
            adj[n+1].push_back (i);
            adj[i].push_back (n + 1);
        }
    }
    // added edges to source and sink

    // call ford-fulkerson
    int answer = ford_fulkerson(adj);
    printf("%lld\n", answer);
}
