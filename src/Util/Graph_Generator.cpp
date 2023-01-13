#include "bits/stdc++.h"

#ifdef local
#include "custom/debugger.h"
#else
#define debug(x) 42;
#endif

using namespace std;
#define int long long

// author: Manzood Naqvi
// NOTE: This is a work in progress. It currently does not work as well as I
// want it to, and I have not tested it at all

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

class GraphGenerator {
    int n, m;
    bool weighted;
    vector<vector<int>> graph;
    vector<vector<int>> weights;

   public:
    GraphGenerator(int _n, int _m, bool _weighted)
        : n(_n), m(_m), weighted(_weighted) {
        graph.resize(n);
        weights.resize(n, vector<int>(n, 0));
    }

    void generateUnweightedGraph() {
        // make random edges, add them
        // TODO: consider the possibility of self loops and cycles
        set<pair<int, int>> edges;
        for (int i = 0; i < m; i++) {
            pair<int, int> edge;
            do {
                int sourceNode = uniform_int_distribution<int>(0, n - 1)(rng);
                int destNode = uniform_int_distribution<int>(0, n - 1)(rng);
                edge = {sourceNode, destNode};
            } while (edges.count(edge));
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }
    }

    void generateWeights(vector<vector<int>>& adj, int range) {
        for (int i = 0; i < n; i++) {
            for (auto x : adj[i]) {
                weights[i][x] = uniform_int_distribution<int>(0, range)(rng);
            }
        }
    }

    void test_PrintGraph(vector<vector<int>>& adj) {
        for (int i = 0; i < (int)adj.size(); i++) {
            printf("%lld: ", i + 1);
            for (auto x : adj[i]) {
                printf("%lld ", x + 1);
            }
            printf("\n");
        }
    }

    // ideally, this function should be able to take in graphs of different
    // kinds and print them using a single interface
    void printGraph() {
        printf("%lld %lld %lld\n", n, m, (int)weighted);
        set<pair<int, int>> edges;
        for (int i = 0; i < n; i++) {
            for (auto x : graph[i]) {
                int temp = i;
                if (temp > x) swap(temp, x);
                edges.insert({temp, x});
            }
        }
        vector<pair<int, int>> e;
        for (auto x : edges) {
            e.push_back(x);
        }
        shuffle(e.begin(), e.end(), rng);
        for (auto x : e) {
            printf("%lld %lld\n", x.first + 1, x.second + 1);
        }
    }

    void generateBipartiteGraph() {
        // create two sets of nodes, and start to make edges between them
        vector<int> temp(n);
        iota(temp.begin(), temp.end(), 0);
        shuffle(temp.begin(), temp.end(), rng);
        int temp_index = uniform_int_distribution<int>(1, n - 2)(rng);
        vector<int> set1, set2;
        for (int i = 0; i <= temp_index; i++) {
            set1.push_back(temp[i]);
        }
        for (int i = temp_index + 1; i < n; i++) {
            set2.push_back(temp[i]);
        }
        // make m edges between set 1 and set 2
        set<pair<int, int>> edges;
        for (int i = 0; i < m; i++) {
            pair<int, int> edge;
            do {
                int temp1 =
                    uniform_int_distribution<int>(0, (int)set1.size() - 1)(rng);
                int temp2 =
                    uniform_int_distribution<int>(0, (int)set2.size() - 1)(rng);
                temp1 = set1[temp1];
                temp2 = set2[temp2];
                edge = {temp1, temp2};
            } while (edges.count(edge));
            graph[edge.first].push_back(edge.second);
            graph[edge.second].push_back(edge.first);
        }
    }
};

int32_t main(int32_t argc, char* argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    bool is_weighted = atoi(argv[3]);
    // vector <vector <int>> graph = generate_graph (n, m);
    // printgraph (graph);
    GraphGenerator Gen(n, m, is_weighted);
    Gen.generateUnweightedGraph();
    Gen.printGraph();
}
