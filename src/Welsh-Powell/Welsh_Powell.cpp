#include "bits/stdc++.h"
using namespace std;
#define debug(x) cout << #x << " = " << x << endl;
#define int long long

vector <int> welsh_powell (int n, vector <vector <int>>& adj) {
    vector <int> colors(n, -1);
    vector <int> degree(n, 0);
    vector <pair <int, int>> vertices(n);
    vector <bool> coloring_finished(n, false);

    // sort vertices in the order of their priority, which is degree
    for (int i = 0; i < n; i++) {
        degree[i] = adj[i].size();
        vertices[i].first = degree[i];
        vertices[i].second = i;
    }

    sort (vertices.begin(), vertices.end());
    reverse (vertices.begin(), vertices.end());

    // colour largest unpicked vertex with current colour
    int color = 1;
    int completed = 0;

    while (completed < n) {
        set <int> current;
        for (int i = 0; i < n; i++) {
            int node = vertices[i].second;
            if (!coloring_finished[node]) {
                bool valid = true;
                for (auto u: adj[node]) {
                    // check if any of the connections have the same color
                    if (current.count(u)) {
                        valid = false;
                    }
                }

                if (valid) {
                    colors[node] = color;
                    completed++;
                    coloring_finished[node] = true;
                    current.insert (node);
                }
            }
        }
        color++;
    }

    return colors;
}

int32_t main () {
    int n, k;
    // get number of vertices
    scanf("%lld%lld", &n, &k);

    vector <vector <int>> adj(n);

    for (int i = 0; i < k; i++) {
        int a, b;
        scanf("%lld%lld", &a, &b);
        a--; // do this only if there is no zero based indexing
        b--;
        adj[a].push_back (b);
        adj[b].push_back (a);
    }

    vector <int> colors = welsh_powell(n, adj);

    for (int i = 0; i < n; i++) {
        printf("%lld ", colors[i]);
    }

    printf("\n");
}
