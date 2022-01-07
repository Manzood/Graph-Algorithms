#include "bits/stdc++.h"
using namespace std;
#ifdef local
    #include "custom/debugger.h"
#else
    #define debug(x) 42;
#endif
#define int long long

// using a disjoint set union to implement boruvka's algorithm

class dsu {
  public:
    vector <int> p;
    vector <int> size;
    int n;
    dsu (int _n) : n(_n) {
        p.resize(n);
        iota(p.begin(), p.end(), 0);
        size.resize(n, 1);
    }
    int parent(int x) {
        // path compression takes place here
        if (x == p[x]) return x;
        else return p[x] = parent(p[x]);
    }
    inline bool unite (int x, int y) {
        x = parent(x);
        y = parent(y);
        if (x != y) {
            if (size[x] > size[y])
                swap(x, y);
            p[x] = y;
            size[x] += size[y];
            return true;
        }
        return false;
    }
};


vector <int> visited;
void dfs (int node, vector <vector <int>>& adj, vector <vector <int>>& weights, dsu& d, vector <pair <int, pair <int, int>>>& temp) {
    if (visited[node]) return;
    visited[node] = true;

    for (auto x: adj[node]) {
        if (d.parent(x) == d.parent(node)) {
            dfs (x, adj, weights, d, temp);
        } else {
            temp.push_back ({weights[node][x], {node, x}});
        }
    }
};


// time complexity: O(n^3)
int boruvka (vector <vector <int>>& adj, vector <vector <int>>& weights) {
    int mst = 0;
    // initialise all vertices as individual components

    int n = adj.size();
    dsu d(n);

    visited.resize (n, false);
    vector <pair <int,int>> mst_edges;

    // combine components
    // find all the edges in the current component

    for (int i = 0; i < n - 1; i++) {
        visited.assign(n, false);
        vector <pair <int, pair <int, int>>> temp;
        int cur = d.parent(0);
        // debug (cur);
        // if (cur == 1) {
            // debug (d.parent(0));
            // debug (d.parent(1));
        // }

        dfs(cur, adj, weights, d, temp);

        // take the one with the least weight
        sort (temp.begin(), temp.end());
        // adding it to the current component
        int new_node = temp[0].second.second;

        // debug (temp[0].first);
        // debug (temp[0].second.first, temp[0].second.second);

        mst_edges.push_back(temp[0].second);
        d.unite (cur, new_node);
        mst += temp[0].first; // adding current weight
    }

    printf("Printing mst edges:\n");
    for (int i = 0; i < (int) mst_edges.size(); i++) {
        printf("%lld %lld\n", mst_edges[i].first, mst_edges[i].second);
    }
    printf("\n");

    return mst;
}


void delete_edge (vector <vector <int>>& adj, int a, int b) {
    int ind = -1;
    for (int i = 0; i < (int) adj[a].size(); i++) {
        if (adj[a][i] == b) {
            ind = i;
            break;
        }
    }
    adj[a].erase (adj[a].begin() + ind);
    ind = -1;
    for (int i = 0; i < (int) adj[b].size(); i++) {
        if (adj[b][i] == a) {
            ind = i;
            break;
        }
    }
    adj[b].erase (adj[b].begin() + ind);
}


void simple_dfs (int node, vector <vector <int>>& adj) {
    if (visited[node]) return;
    visited[node] = true;
    for (auto x: adj[node]) {
        simple_dfs (x, adj);
    }
}

vector <vector <int>> reverse_delete (vector <vector <int>> adj, vector <vector <int>> weights) {
    vector <vector <int>> copied_graph = adj;
    int n = copied_graph.size();
    vector <vector <int>> edge_weights;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (weights[i][j] > 0) {
                edge_weights.push_back ({weights[i][j], i, j});
            }
        }
    }

    sort (edge_weights.begin(), edge_weights.end());
    reverse (edge_weights.begin(), edge_weights.end());

    for (int i = 0; i < (int) edge_weights.size(); i++) {
        // check if deleting this edge will disconnect the graph
        // delete edge
        // if it's disconnected, reconnect it
        int a = edge_weights[i][1];
        int b = edge_weights[i][2];

        delete_edge (copied_graph, a, b);

        // check if graph is disconnnected
        visited.resize (n, false);
        visited.assign (n, false);
        simple_dfs (0, copied_graph);
        bool disconnected = false;

        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                disconnected = true;
            }
        }

        if (disconnected) {
            copied_graph[a].push_back (b);
            copied_graph[b].push_back (a);
        }
    }

    return copied_graph;
}

int32_t main () {
    // input is a connected graph with weights (undirected)

    int n, m;
    scanf("%lld%lld", &n, &m);
    vector <vector <int>> adj(n);
    vector <vector <int>> weight(n, vector <int> (n, 0));

    for (int i = 0; i < m; i++) {
        int a, b, w;
        scanf("%lld%lld%lld", &a, &b, &w);
        // a--; b--;
        adj[a].push_back (b);
        adj[b].push_back (a);
        weight[a][b] = w;
        weight[b][a] = w;
    }

    bool testing_boruvka = false;

    if (testing_boruvka) {
        int mst = boruvka (adj, weight);
        printf("Sum of weights of edges of MST: %lld\n", mst);
    } else {
        // reverse-delete algorithm
        vector <vector <int>> mst = reverse_delete(adj, weight);
        printf("Printing MST:\n");
        int sum_of_mst = 0;
        set <pair <int, int>> used;
        for (int i = 0; i < n; i++) {
            for (auto x: mst[i]) {
                if (!used.count({x, i})) {
                    printf("%lld %lld\n", i, x);
                    used.insert({i, x});
                    sum_of_mst += weight[i][x];
                }
            }
        }
        printf("Sum of weights of edges in MST: %lld\n", sum_of_mst);
    }
}
