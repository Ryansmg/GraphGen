// Update: 2025-06-04

#ifndef GRAPHGEN_H_
#define GRAPHGEN_H_

#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// nodes : 1 ~ N
class Graph {
protected:
    int n = 0;
    vector<pair<int, int>> edges;
    map<pair<int, int>, int> edge_cnt;
    vector<int> group;

    #define chk(b, msg) chk_(b, msg, __LINE__)

    static void chk_(bool b, const string& msg, int line) {
        if(!b) {
            cerr << "Check failed at line " << line << ":\n";
            cerr << msg << '\n';
            exit(1);
        }
    }

    template <typename T1, typename T2> static bool contains(const T1& arr, const T2& val) {
        return arr.find(val) != arr.end();
    }

    void merge_group(int a, int b) {
        chk(1 <= a && a <= n && 1 <= b && b <= n, "not a valid node");
        a = find_group(a); b = find_group(b);
        if(a != b) group[a] = b;
    }
public:
    int find_group(const int a) {
        chk(1 <= a && a <= n, "not a valid node");
        if(group[a] == a) return a;
        return group[a] = find_group(group[a]);
    }
    explicit Graph(int N) : n(N), group(N+1) {
        for(int i = 1; i <= N; i++) group[i] = i;
    }
    [[nodiscard]] int node_count() const { return n; }
    [[nodiscard]] int edge_count() const { return static_cast<int>(edges.size()); }
    Graph() = default;

    void add_edge(int a, int b) {
        chk(1 <= a && a <= n && 1 <= b && b <= n, "not a valid node");
        edges.emplace_back(a, b);
        edge_cnt[make_pair(a, b)]++;
        merge_group(a, b);
    }
    [[nodiscard]] bool has_edge(int a, int b) const {
        chk(1 <= a && a <= n && 1 <= b && b <= n, "not a valid node");
        return contains(edge_cnt, make_pair(a, b));
    }
    [[nodiscard]] bool has_edge_undir(int a, int b) const {
        chk(1 <= a && a <= n && 1 <= b && b <= n, "not a valid node");
        return has_edge(a, b) || has_edge(b, a);
    }
    [[nodiscard]] const vector<pair<int, int>>& edge_list() const { return edges; }
    [[nodiscard]] vector<pair<int, int>> edge_list_copy() const { return edges; }
    Graph& shuffle_edge_list() { shuffle(edges.begin(), edges.end()); return *this; }
    Graph& shuffle_edge_list_undir() {
        for(auto &[a, b] : edges) if(rnd.next(2)) swap(a, b);
        return shuffle_edge_list();
    }
    /// the value of node_group()[0] is undefined.
    const vector<int>& node_group() {
        for(int i = 1; i <= n; i++) group[i] = find_group(i);
        return group;
    }
    [[nodiscard]] vector<vector<int>> adjacent_list() const {
        vector<vector<int>> adj(n+1);
        for(const auto& [u, v] : edges)
            adj[u].push_back(v);
        return adj;
    }
    [[nodiscard]] vector<vector<int>> adjacent_list_undir() const {
        vector<vector<int>> adj(n+1);
        for(const auto& [u, v] : edges)
            adj[u].push_back(v), adj[v].push_back(u);
        return adj;
    }
    vector<vector<int>> connected_components() {
        map<int, vector<int>> ret1;
        for(int i = 1; i <= n; i++) ret1[find_group(i)].push_back(i);
        vector<vector<int>> ret2;
        for(const auto& [a, b] : ret1) ret2.emplace_back(b);
        return ret2;
    }
    [[nodiscard]] bool has_duplicate_edges() const {
        for(const auto& [a, b] : edge_cnt) if(b > 1) return true;
        return false;
    }
    [[nodiscard]] bool has_duplicate_edges_undir() const {
        set<pair<int, int>> s;
        for(auto [a, b] : edges) {
            if(a > b) swap(a, b);
            if(contains(s, make_pair(a, b))) return true;
            s.emplace(a, b);
        }
        return false;
    }

    /// arr[예전 노드 번호] = 새 노드 번호
    [[nodiscard]] Graph nodes_shuffled(const vector<int>& arr) const {
        chk(arr.size() == n + 1, "size of arr at nodes_shuffled(arr) must be n+1");
        vector<bool> check(n + 1, false);
        for(int i = 1; i <= n; i++) check[arr[i]] = true;
        for(int i = 1; i <= n; i++) chk(arr[i], "arr should be a bijective function.");

        Graph ret(n);
        for(const auto& [u, v] : edges)
            ret.add_edge(arr[u], arr[v]);
        return ret;
    }

    [[nodiscard]] Graph nodes_shuffled() const {
        vector<int> arr(n);
        for(int i = 0; i < n; i++) arr[i] = i+1;
        shuffle(arr.begin(), arr.end());
        arr.insert(arr.begin(), 0);
        return nodes_shuffled(arr);
    }

    Graph& shuffle_nodes(const vector<int>& arr) {
        *this = this->nodes_shuffled(arr);
        return *this;
    }

    Graph& shuffle_nodes() {
        *this = this->nodes_shuffled();
        return *this;
    }

    Graph& shuffle_all() { return shuffle_nodes(), shuffle_edge_list(); }
    Graph& shuffle_all_undir() { return shuffle_nodes(), shuffle_edge_list_undir(); }
    [[nodiscard]] Graph all_shuffled() const { Graph g = *this; g.shuffle_all(); return g; }
    [[nodiscard]] Graph all_shuffled_undir() const { Graph g = *this; g.shuffle_all_undir(); return g; }

    bool is_tree() { return static_cast<long long>(edges.size()) == n - 1 &&
        static_cast<long long>(connected_components().size()) == 1; }


    /// nodes : 1~n
    static Graph tree(const int n, const int linearRatio) {
        Graph ret(n);
        vector<int> p(n, 0);
        for(int i = 0; i <= n - 1; i++) if (i > 0) p[i] = rnd.wnext(i, linearRatio);
        vector<int> perm(n, 0); for(int i = 0; i <= n - 1; i++) perm[i] = i;
        shuffle(perm.begin() + 1, perm.end());
        for(int i = 1; i <= n - 1; i++) ret.add_edge(perm[i] + 1, perm[p[i]] + 1);
        assert(ret.is_tree());
        return ret;
    }

    static Graph tree(const int n) {
        Graph ret(n);
        for(int i = 0; i < n-1; i++) {
            int u, v;
            do {
                u = rnd.next(n) + 1;
                v = rnd.next(n) + 1;
            } while(ret.find_group(u) == ret.find_group(v));
            ret.add_edge(u, v);
        }
        return ret;
    }

    template <typename... Args>
    static Graph halin_graph(const int n, const Args&... args) {
        chk(n >= 4, "a halin graph must have at least four nodes");

        Graph tr = tree(n, args...);
        vector<vector<int>> adj(n + 1);
        for(const auto& [u, v] : tr.edge_list()) adj[u].push_back(v), adj[v].push_back(u);
        int root = 0;
        for(int i = 1; i <= n; i++) if(adj[i].size() > 2U) { root = i; break; }

        // 그래프가 일자인 경우 리프 노드를 잡으면 됨
        for(int i = 1; i <= n && !root; i++) if(adj[i].size() == 1) { root = i; break; }

        vector<vector<int>> child(n + 1);

        // init child
        function<void(int, int)> f = [&](int cur, int par) {
            for(int i : adj[cur]) {
                if(i == par) continue;
                child[cur].push_back(i);
                f(i, cur);
            }
        };
        f(root, -1);


        // degree가 2인 노드 제거
        function<void(int, int)> f2 = [&](int cur, int par) {
            if(child[cur].size() == 1 && par != -1) {
                child[par].push_back(child[cur][0]);
                child[cur].clear();
            } else {
                for(int i = 0; i < static_cast<int>(child[cur].size()); i++) {
                    f2(child[cur][i], cur);
                }
            }
        };
        f2(root, -1);

        vector<int> leaves;

        // 리프 노드 구하기
        function<void(int)> f3 = [&](int i) {
            if(child[i].empty()) leaves.push_back(i);
            else for(int j : child[i]) f3(j);
        };
        f3(root);

        Graph ret(n);
        for(int i = 1; i <= n; i++) for(int j : child[i]) ret.add_edge(i, j);
        int lsz = static_cast<int>(leaves.size());
        for(int i = 0; i < lsz - 1; i++) ret.add_edge(leaves[i], leaves[i + 1]);
        ret.add_edge(leaves.back(), leaves[0]);
        return ret;
    }

    template <typename... Args>
    static Graph tree_without_degree_of_2(const int n, const Args&... args) {
        chk(n >= 4, "tree_without_degree_of_2 must have at least four nodes");
        Graph tr = tree(n, args...);
        vector<vector<int>> adj(n + 1);
        for(const auto& [u, v] : tr.edge_list()) adj[u].push_back(v), adj[v].push_back(u);
        int root = 0;
        for(int i = 1; i <= n; i++) if(adj[i].size() > 2U) { root = i; break; }
        for(int i = 1; i <= n && !root; i++) if(adj[i].size() == 1) { root = i; break; }
        vector<vector<int>> child(n + 1);
        function<void(int, int)> f = [&](int cur, int par) {
            for(int i : adj[cur]) {
                if(i == par) continue;
                child[cur].push_back(i);
                f(i, cur);
            }
        };
        f(root, -1);
        function<void(int, int)> f2 = [&](int cur, int par) {
            if(child[cur].size() == 1 && par != -1) {
                child[par].push_back(child[cur][0]);
                child[cur].clear();
            } else {
                for(int i = 0; i < static_cast<int>(child[cur].size()); i++) {
                    f2(child[cur][i], cur);
                }
            }
        };
        f2(root, -1);
        Graph ret(n);
        for(int i = 1; i <= n; i++) for(int j : child[i]) ret.add_edge(i, j);
        return ret;
    }

    /// @returns 합이 sum이 되는 count개의 자연수
    static vector<int> distribute_natural_numbers(const int sum, const int count) {
        chk(sum >= count, "count cannot be greater than sum");
        chk(count > 0, "count must be greater than zero.");

        if(sum == count) return vector<int>(count, 1);

        vector<int> bars = rnd.distinct(count - 1, 1, sum - 1);
        bars.push_back(0);
        bars.push_back(sum);
        sort(bars.begin(), bars.end());
        vector<int> result;
        for (int i = 0; i < count; ++i) {
            int val = bars[i + 1] - bars[i];
            result.push_back(val);
        }
        shuffle(result.begin(), result.end());
        return result;
    }

    /// @param n : 총 노드 수
    /// @param tree_n : 사이클을 압축한 트리의 총 노드 수
    /// @param cycle_cnt : 사이클 수
    /// @param linearRatio : 트리의 linearRatio
    static Graph cactus(const int n, const int tree_n, const int cycle_cnt, const int linearRatio = -1) {
        chk(cycle_cnt <= tree_n, "cycle_cnt cannot be greater than tree_n.");
        chk(cycle_cnt <= n - tree_n, "cannot make cycles, cycle_cnt is too large.");
        chk(cycle_cnt || n == tree_n, "n must be equal to tree_n if cycle_cnt is 0.");

        if(!cycle_cnt) return tree(n, linearRatio);

        Graph tr = linearRatio == -1 ? tree(tree_n) : tree(tree_n, linearRatio);
        Graph ret = Graph(n);

        set<int> cycleNodes;
        while(cycleNodes.size() < cycle_cnt) cycleNodes.insert(rnd.next(tree_n) + 1);
        vector<int> cycleSizes = distribute_natural_numbers(n - tree_n, cycle_cnt);
        vector<vector<int>> nextNodes(tree_n + 1);
        int cycleSizesIdx = 0, curNodeNum = 1;
        for(int i = 1; i <= tree_n; i++) {
            nextNodes[i].push_back(curNodeNum++);
            if(contains(cycleNodes, i)) {
                int nsz = cycleSizes[cycleSizesIdx++];
                for(int j = 1; j <= nsz; j++) nextNodes[i].push_back(curNodeNum++);

                for(int j = 0; j < nsz; j++) ret.add_edge(nextNodes[i][j], nextNodes[i][j+1]);
                ret.add_edge(nextNodes[i].back(), nextNodes[i].front());
            }
        }

        for(const auto& [u, v] : tr.edge_list())
            ret.add_edge(rnd.any(nextNodes[u]), rnd.any(nextNodes[v]));

        // 사이클의 노드 번호가 항상 연속해있으므로 번호를 셔플해서 반환
        return ret.nodes_shuffled();
    }

    static Graph connected_graph(const int n, const int m, const int linearRatio = -1) {
        chk(n >= 1, "a graph must have at least one node");
        chk(m >= n - 1, "edge count is too small to make a connected graph");
        chk(m <= n * (n - 1) / 2, "edge count is too large");
        Graph ret;
        if(linearRatio == -1) ret = tree(n);
        else ret = tree(n, linearRatio);
        for(int i=n-1; i<m; i++) {
            int u, v;
            do {
                u = rnd.next(n) + 1;
                v = rnd.next(n) + 1;
            } while(u == v || ret.has_edge_undir(u, v));
            ret.add_edge(u, v);
        }
        return ret;
    }

    static Graph rand_graph(const int n, const int m) {
        chk(n >= 1, "a graph must have at least one node");
        chk(m <= n * (n - 1) / 2, "edge count is too large");
        Graph ret(n);
        for(int r = 0; r < m; r++) {
            int u, v;
            do {
                u = rnd.next(n) + 1;
                v = rnd.next(n) + 1;
            } while(u == v || ret.has_edge_undir(u, v));
            ret.add_edge(u, v);
        }
        return ret;
    }

    static Graph line_graph(const int n, const int first_node = -1, const int last_node = -1) {
        vector<int> arr(n);
        for(int i = 0; i < n; i++) arr[i] = i+1;
        shuffle(arr.begin(), arr.end());

        if(first_node != -1) {
            chk(1 <= first_node && first_node <= n, "first_node is not a valid node");
            chk(first_node != last_node && n > 1, "first_node and last_node cannot be the same");
            for(int i = 0; i < n; i++) if(arr[i] == first_node) swap(arr[0], arr[i]);
        }

        if(last_node != -1) {
            chk(1 <= last_node && last_node <= n, "last_node is not a valid node");
            for(int i = 0; i < n; i++) if(arr[i] == last_node) swap(arr[n-1], arr[i]);
        }

        if(first_node != -1) assert(arr[0] == first_node);
        if(last_node != -1) assert(arr[n-1] == last_node);

        Graph ret(n);
        for(int i = 0; i < n-1; i++) ret.add_edge(arr[i], arr[i+1]);
        return ret;
    }

    static Graph complete_graph(const int n) {
        Graph ret(n);
        for(int i = 1; i <= n; i++) for(int j = i+1; j <= n; j++) ret.add_edge(i, j);
        return ret;
    }

    static Graph star_graph(const int n, const int root = 1) {
        Graph ret(n);
        for(int i = 1; i <= n; i++) if(i != root) ret.add_edge(root, i);
        return ret;
    }

    // 일직선 그래프에서 리프 노드가 아닌 모든 노드에 리프 노드 하나를 이은 그래프
    static Graph skeleton_graph(const int n) {
        chk(n % 2 == 0, "node count must be even at skeleton graph");
        int line_len = n / 2 + 1;
        Graph g(n);
        for(int i = 2; i <= line_len; i++) g.add_edge(i - 1, i);
        for(int i = 2; i <= line_len - 1; i++) g.add_edge(i, line_len + i - 1);
        return g;
    }

    // 노드개수 간선개수
    // u1 v1
    // u2 v2
    // ...
    void ps_style_print(const bool zero_base = false) const {
        println(n, edges.size());
        for(auto& [u, v] : edges) println(u - zero_base, v - zero_base);
    }

    void csacademy_style_print() const {
        for(int i = 1; i <= n; i++) println(i);
        for(const auto& [u, v] : edges) println(u, v);
    }

    void print_edges() const {
        for(const auto& [u, v] : edges) println(u, v);
    }
};

class Tree : public Graph {
    Tree(const Graph& g, bool unsafe) : Graph(g) { chk(unsafe, ""); }
    Tree(Graph&& g, bool unsafe) : Graph(std::move(g)) { chk(unsafe, ""); }
public:
    void add_edge(int a, int b) = delete;

    explicit Tree(const Graph& graph) : Graph(graph) { chk(this->Graph::is_tree(), "graph must be tree"); }
    explicit Tree(Graph&& graph) : Graph(std::move(graph)) { chk(this->Graph::is_tree(), "graph must be tree"); }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    bool is_tree() { return true; } // NOLINT(*-convert-member-functions-to-static)

    static Tree tree(const int n) { return Tree(Graph::tree(n), true); }
    static Tree tree(const int n, const int linearRatio) { return Tree(Graph::tree(n, linearRatio), true); }

    template <typename... Args>
    static Tree halin_graph(int n, const Args&... args) = delete;

    static Tree cactus(int n, int tree_n, int cycle_cnt, int linearRatio = -1) = delete;

    static Tree line_graph(const int n, const int first_node = -1, const int last_node = -1) { return Tree(Graph::line_graph(n, first_node, last_node), true); }

    static Tree star_graph(const int n, const int root = 1) { return Tree(Graph::star_graph(n, root), true); }

    template <typename... Args>
    static Tree skeleton_graph(int n, const Args&... args) { return Tree(Graph::skeleton_graph(n, args...), true); }
};

#endif
