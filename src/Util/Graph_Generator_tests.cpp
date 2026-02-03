#include <exception>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#define GRAPH_GENERATOR_NO_MAIN
#include "Graph_Generator.cpp"
#undef int

using std::string;
using std::vector;

struct TestFailure : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

static void require(bool condition, const string& message) {
    if (!condition) throw TestFailure(message);
}

using Adj = vector<vector<long long>>;

static std::set<std::pair<long long, long long>> to_unique_edges(
    const Adj& adj) {
    std::set<std::pair<long long, long long>> edges;
    for (long long u = 0; u < (long long)adj.size(); u++) {
        for (long long v : adj[u]) {
            if (u < v) edges.insert({u, v});
        }
    }
    return edges;
}

static void check_undirected_graph(const Adj& adj, long long n, long long m,
                                   const string& label) {
    require((long long)adj.size() == n,
            label + ": adjacency size mismatch");

    vector<std::unordered_set<long long>> adj_sets(n);
    for (long long u = 0; u < n; u++) {
        for (long long v : adj[u]) {
            require(v >= 0 && v < n, label + ": vertex out of range");
            require(v != u, label + ": self loop detected");
            require(adj_sets[u].insert(v).second,
                    label + ": duplicate neighbor detected");
        }
    }

    for (long long u = 0; u < n; u++) {
        for (long long v : adj_sets[u]) {
            require(adj_sets[v].count(u) == 1,
                    label + ": missing reverse edge");
        }
    }

    auto edges = to_unique_edges(adj);
    require((long long)edges.size() == m,
            label + ": unique edge count mismatch");
}

static void check_bipartite(const Adj& adj, long long n,
                            const string& label) {
    vector<int> color(n, -1);
    for (long long i = 0; i < n; i++) {
        if (color[i] != -1) continue;
        std::queue<long long> q;
        color[i] = 0;
        q.push(i);
        while (!q.empty()) {
            long long u = q.front();
            q.pop();
            for (long long v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else {
                    require(color[v] != color[u],
                            label + ": not bipartite");
                }
            }
        }
    }
}

static void check_connected(const Adj& adj, long long n,
                            const string& label) {
    vector<int> seen(n, 0);
    std::queue<long long> q;
    seen[0] = 1;
    q.push(0);
    long long count = 1;
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (long long v : adj[u]) {
            if (!seen[v]) {
                seen[v] = 1;
                q.push(v);
                count++;
            }
        }
    }
    require(count == n, label + ": graph is disconnected");
}

static void test_unweighted_graph_invariants() {
    const long long n = 6;
    const long long m = 14;

    rng.seed(0);
    GraphGenerator gen(n, m, false);
    gen.generateUnweightedGraph();

    check_undirected_graph(gen.adjacency(), n, m, "unweighted seed=0");
}

static void test_bipartite_graph_invariants() {
    const long long n = 10;
    const long long m = 20;

    rng.seed(0);
    GraphGenerator gen(n, m, false);
    gen.generateBipartiteGraph();

    check_undirected_graph(gen.adjacency(), n, m, "bipartite seed=0");
    check_bipartite(gen.adjacency(), n, "bipartite seed=0");
}

static void test_tree_graph_invariants() {
    const long long n = 8;
    const long long m = n - 1;

    rng.seed(0);
    GraphGenerator gen(n, m, false);
    gen.generateTree();

    check_undirected_graph(gen.adjacency(), n, m, "tree seed=0");
    check_connected(gen.adjacency(), n, "tree seed=0");
}

int main() {
    try {
        test_unweighted_graph_invariants();
        test_bipartite_graph_invariants();
        test_tree_graph_invariants();
    } catch (const TestFailure& e) {
        std::cerr << "TEST FAILED: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "TEST ERROR: " << e.what() << "\n";
        return 1;
    }

    std::cout << "ALL TESTS PASSED\n";
    return 0;
}
