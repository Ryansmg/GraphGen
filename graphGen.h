// Update: 2025-11-11

#ifndef GRAPHGEN_H_
#define GRAPHGEN_H_

#include "testlib.h"
#include <bits/stdc++.h>

namespace graphGen {

#ifdef chk
#error "chk must not be defined before graphGen.h"
#endif

#define chk(b, msg) chk_(b, msg, __LINE__)

    // nodes : 1 ~ V
    class Graph {
    protected:
        int v = 0;
        std::vector<std::pair<int, int>> edges;
        std::vector<int> group;
        std::vector<int> rank;

        static void chk_(bool b, const std::string& msg, int line) {
            if(!b) {
                std::cerr << "Check failed at line " << line << ":\n";
                std::cerr << msg << '\n';
                exit(1);
            }
        }

        template <typename T1, typename T2> static bool contains(const T1& arr, const T2& val) {
            return arr.find(val) != arr.end();
        }

        /**
         * Time complexity: approximately O(1)
         */
        void merge_group(int a, int b) {
            chk(1 <= a && a <= v && 1 <= b && b <= v, "not a valid node");
            a = find_group(a); b = find_group(b);
            if(rank[a] > rank[b]) std::swap(a, b);
            if(a != b) {
                group[a] = b;
                rank[b] += rank[a] == rank[b];
            }
        }
    public:
        /**
         * @brief creates a placeholder object
         */
        Graph() = default;

        /**
         * @brief creates a new graph consisting of V nodes and zero edges
         * @param V the number of nodes
         */
        explicit Graph(int V) : v(V), group(V+1), rank(V+1, 0) {
            for(int i = 1; i <= V; i++) group[i] = i;
        }

        /**
         * @brief adds the edge a -> b. <br>
         * the added edge can be interpreted as an undirected edge later.
         *
         * Time complexity: approx. O(1)
         * @param a the starting vertex
         * @param b the ending vertex
         * @pre 1 <= a <= V
         * @pre 1 <= b <= V
         */
        void add_edge(int a, int b) {
            chk(1 <= a && a <= v && 1 <= b && b <= v, "not a valid node");
            edges.emplace_back(a, b);
            merge_group(a, b);
        }

        /**
         * @brief checks whether the edge a -> b exists. <br>
         * (does not check edge b -> a)
         *
         * Time complexity: O(E)
         *
         * @return if the edge a -> b exists.
         * @param a the starting vertex
         * @param b the ending vertex
         * @pre 1 <= a <= N
         * @pre 1 <= b <= N
         */
        [[nodiscard]] bool has_edge(int a, int b) const {
            chk(1 <= a && a <= v && 1 <= b && b <= v, "not a valid node");
            auto p = std::pair(a, b);
            for(const auto& e : edges) if(e == p) return true;
            return false;
        }

        /**
         * @brief checks whether the undirected edge a -- b exists.
         *
         * Time complexity: O(E)
         *
         * @returns if add_edge(a, b) or add_edge(b, a) is called at least once.
         * @param a vertex number
         * @param b vertex number
         * @pre 1 <= a <= V
         * @pre 1 <= b <= V
         */
        [[nodiscard]] bool has_edge_undir(int a, int b) const {
            chk(1 <= a && a <= v && 1 <= b && b <= v, "not a valid node");
            return has_edge(a, b) || has_edge(b, a);
        }

        /**
         * @brief the node count of this graph.
         *
         * Time complexity: O(1)
         * @return the node count
         */
        [[nodiscard]] int node_count() const { return v; }

        /**
         * @brief the node count of this graph.
         *
         * Time complexity: O(1)
         * @return the node count
         */
        [[nodiscard]] int V() const { return v; }

        /**
         * @brief the edge count of this graph.
         *
         * Time complexity: O(1)
         * @return the edge count
         */
        [[nodiscard]] int edge_count() const { return static_cast<int>(edges.size()); }

        /**
         * @brief the edge count of this graph.
         *
         * Time complexity: O(1)
         * @return the edge count
         */
        [[nodiscard]] int E() const { return static_cast<int>(edges.size()); }

        /**
         * @brief the std::vector of added edges.
         *
         * Time complexity: O(1)
         * @return const std::vector<std::pair<int, int>>& containing the edges
         */
        [[nodiscard]] const std::vector<std::pair<int, int>>& edge_list() const { return edges; }

        /**
         * @brief the copy of the list containing added edges
         *
         * Time complexity: O(E)
         * @return the copy of edge_list()
         */
        [[nodiscard]] std::vector<std::pair<int, int>> edge_list_copy() const { return edges; }

        /**
         * @brief finds the group number of a vertex <br>
         * 'group' is unique numbers for each connected component
         *
         * Time complexity: approximately O(1)
         * @param a the target vertex
         * @return the group number of vertex a
         */
        int find_group(const int a) {
            chk(1 <= a && a <= v, "not a valid node");
            if(group[a] == a) return a;
            return group[a] = find_group(group[a]);
        }

        /**
         * @brief finds the group numbers of all nodes
         *
         * Time complexity: O(V)
         * @return a std::vector representing the group of nodes 1..V <br>
         * the value of node_group()[0] is undefined.
         */
        const std::vector<int>& node_group() {
            for(int i = 1; i <= v; i++) group[i] = find_group(i);
            return group;
        }

        /**
         * @brief shuffles the order of edges
         *
         * Time complexity: O(E)
         * @return the reference of this graph
         */
        Graph& shuffle_edge_list() { shuffle(edges.begin(), edges.end()); return *this; }

        /**
         * @brief shuffles the order of edges and the order of nodes in the edges
         *
         * Time complexity: O(E)
         * @return the reference of this graph
         */
        Graph& shuffle_edge_list_undir() {
            for(auto &[a, b] : edges) if(rnd.next(2)) std::swap(a, b);
            return shuffle_edge_list();
        }

        /**
         * @brief makes the adjacency list of the directed graph <br>
         * edge u->v => adj[u].push_back(v)
         *
         * Time complexity: O(E)
         * @return the adjacency list of this graph
         */
        [[nodiscard]] std::vector<std::vector<int>> adjacency_list() const {
            std::vector<std::vector<int>> adj(v+1);
            for(const auto& [u1, u2] : edges)
                adj[u1].push_back(u2);
            return adj;
        }

        /**
         * @brief makes the adjacency list of the undirected graph <br>
         * edge u<->v => adj[u].push(v), adj[v].push(u)
         *
         * Time complexity: O(E)
         * @return the adjacency list of this graph
         */
        [[nodiscard]] std::vector<std::vector<int>> adjacency_list_undir() const {
            std::vector<std::vector<int>> adj(v+1);
            for(const auto& [u1, u2] : edges)
                adj[u1].push_back(u2), adj[u2].push_back(u1);
            return adj;
        }

        /**
         * @brief calculates the connected components of the graph
         *
         * Time complexity: O(V log V)
         * @return the std::vector of connected components
         */
        std::vector<std::vector<int>> connected_components() {
            std::map<int, std::vector<int>> ret1;
            for(int i = 1; i <= v; i++) ret1[find_group(i)].push_back(i);
            std::vector<std::vector<int>> ret2;
            for(const auto& [a, b] : ret1) ret2.emplace_back(b);
            return ret2;
        }

        /**
         * @brief checks whether this directed graph has duplicate edges. <br>
         * edge a->b and edge b->a are treated as different edges.
         *
         * Time complexity: O(E log E)
         *
         * @return whether this graph has duplicate edges
         */
        [[nodiscard]] bool has_duplicate_edges() const {
            std::set<std::pair<int, int>> s;
            for(auto [a, b] : edges) {
                if(contains(s, std::make_pair(a, b))) return true;
                s.emplace(a, b);
            }
            return false;
        }

        /**
         * @brief checks whether this undirected graph has duplicate edges. <br>
         * edge a<->b and edge b<->a are treated as the same edge.
         *
         * Time complexity: O(E log E)
         * @return whether this graph has duplicated edges
         */
        [[nodiscard]] bool has_duplicate_edges_undir() const {
            std::set<std::pair<int, int>> s;
            for(auto [a, b] : edges) {
                if(a > b) std::swap(a, b);
                if(contains(s, std::make_pair(a, b))) return true;
                s.emplace(a, b);
            }
            return false;
        }

        /**
         * @brief returns the copy of this graph with shuffled node numbers
         *
         * Time complexity: O(V + E)
         * @param arr arr[prev_node_number] = next_node_number
         * @return the shuffled graph
         */
        [[nodiscard]] Graph nodes_shuffled(const std::vector<int>& arr) const {
            chk(arr.size() == v + 1, "size of arr at nodes_shuffled(arr) must be n+1");
            std::vector<bool> check(v + 1, false);
            for(int i = 1; i <= v; i++) check[arr[i]] = true;
            for(int i = 1; i <= v; i++) chk(arr[i], "arr should be a bijective function.");

            Graph ret(v);
            for(const auto& [u, u2] : edges)
                ret.add_edge(arr[u], arr[u2]);
            return ret;
        }


        /**
         * @brief returns the copy of this graph with shuffled node numbers
         *
         * Time complexity: O(V + E)
         * @return the shuffled graph
         */
        [[nodiscard]] Graph nodes_shuffled() const {
            std::vector<int> arr(v);
            for(int i = 0; i < v; i++) arr[i] = i+1;
            shuffle(arr.begin(), arr.end());
            arr.insert(arr.begin(), 0);
            return nodes_shuffled(arr);
        }

        /**
         * @brief shuffles the numbers of nodes
         *
         * Time complexity: O(V + E)
         * @param arr arr[prev_node_number] = next_node_number
         * @return this graph
         */
        Graph& shuffle_nodes(const std::vector<int>& arr) {
            *this = this->nodes_shuffled(arr);
            return *this;
        }

        /**
         * @brief shuffles the numbers of nodes
         *
         * Time complexity: O(V + E)
         * @return this graph
         */
        Graph& shuffle_nodes() {
            *this = this->nodes_shuffled();
            return *this;
        }

        [[nodiscard]] Graph edges_shuffled() const { Graph g = *this; g.shuffle_edge_list(); return g; }
        [[nodiscard]] Graph edges_shuffled_undir() const { Graph g = *this; g.shuffle_edge_list_undir(); return g; }

        Graph& shuffle_all() { return shuffle_nodes(), shuffle_edge_list(); }
        Graph& shuffle_all_undir() { return shuffle_nodes(), shuffle_edge_list_undir(); }
        [[nodiscard]] Graph all_shuffled() const { Graph g = *this; g.shuffle_all(); return g; }
        [[nodiscard]] Graph all_shuffled_undir() const { Graph g = *this; g.shuffle_all_undir(); return g; }

        bool is_tree() { return static_cast<long long>(edges.size()) == v - 1 &&
            static_cast<long long>(connected_components().size()) == 1; }

        /**
         * default elongation value. <br>
         * used in Graph::tree, Graph::halin_graph, etc.
         */
        static constexpr int DEFAULT_ELONGATION = -2147483648;

        /**
         * @brief generates a tree
         *
         * Time Complexity: O(n)
         * @param n the node count
         * @param elongation the larger 'elongation' makes the generated tree longer. can be negative.
         * @return the generated tree
         */
        static Graph tree(const int n, const int elongation) {
            if(elongation == DEFAULT_ELONGATION) return tree(n);
            Graph ret(n);
            std::vector<int> p(n, 0);
            for(int i = 0; i <= n - 1; i++) if (i > 0) p[i] = rnd.wnext(i, elongation);
            std::vector<int> perm(n, 0); for(int i = 0; i <= n - 1; i++) perm[i] = i;
            shuffle(perm.begin() + 1, perm.end());
            for(int i = 1; i <= n - 1; i++) ret.add_edge(perm[i] + 1, perm[p[i]] + 1);
            return ret;
        }

        /**
         * @brief generates a tree by making (n-1) edges randomly without cycles
         *
         * Time Complexity: O(n)
         * @param n the node count
         * @return the generated tree
         */
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

        /**
         * @brief generates a halin graph.
         *
         * Time Complexity: O(n)
         * @param n the node count
         * @param elongation the larger 'elongation' makes the base tree longer. can be negative. not required.
         * @return the generated halin graph.
         */
        static Graph halin_graph(const int n, const int elongation = DEFAULT_ELONGATION) {
            chk(n >= 4, "a halin graph must have at least four nodes");

            Graph tr = tree(n, elongation);
            std::vector<std::vector<int>> adj(n + 1);
            for(const auto& [u, v] : tr.edge_list()) adj[u].push_back(v), adj[v].push_back(u);
            int root = 0;
            for(int i = 1; i <= n; i++) if(adj[i].size() > 2U) { root = i; break; }

            // 그래프가 일자인 경우 리프 노드를 잡으면 됨
            for(int i = 1; i <= n && !root; i++) if(adj[i].size() == 1) { root = i; break; }

            std::vector<std::vector<int>> child(n + 1);

            // 자식 노드 계산
            std::function<void(int, int)> f = [&](int cur, int par) {
                for(int i : adj[cur]) {
                    if(i == par) continue;
                    child[cur].push_back(i);
                    f(i, cur);
                }
            };
            f(root, -1);

            // degree가 2인 노드 제거
            std::function<void(int, int)> f2 = [&](int cur, int par) {
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

            std::vector<int> leaves;

            // 리프 노드 구하기
            std::function<void(int)> f3 = [&](int i) {
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

        static Graph tree_without_degree_of_2(const int n, const int elongation = DEFAULT_ELONGATION) {
            chk(n >= 4, "tree_without_degree_of_2 must have at least four nodes");
            Graph tr = tree(n, elongation);
            std::vector<std::vector<int>> adj(n + 1);
            for(const auto& [u, v] : tr.edge_list()) adj[u].push_back(v), adj[v].push_back(u);
            int root = 0;
            for(int i = 1; i <= n; i++) if(adj[i].size() > 2U) { root = i; break; }
            for(int i = 1; i <= n && !root; i++) if(adj[i].size() == 1) { root = i; break; }
            std::vector<std::vector<int>> child(n + 1);
            std::function<void(int, int)> f = [&](int cur, int par) {
                for(int i : adj[cur]) {
                    if(i == par) continue;
                    child[cur].push_back(i);
                    f(i, cur);
                }
            };
            f(root, -1);
            std::function<void(int, int)> f2 = [&](int cur, int par) {
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
        static std::vector<int> distribute_natural_numbers(const int sum, const int count) {
            chk(sum >= count, "count cannot be greater than sum");
            chk(count > 0, "count must be greater than zero.");

            if(sum == count) return std::vector<int>(count, 1);

            std::vector<int> bars = rnd.distinct(count - 1, 1, sum - 1);
            bars.push_back(0);
            bars.push_back(sum);
            sort(bars.begin(), bars.end());
            std::vector<int> result;
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
        /// @param elongation : 트리의 elongation
        static Graph cactus(const int n, const int tree_n, const int cycle_cnt, const int elongation = DEFAULT_ELONGATION) {
            chk(cycle_cnt <= tree_n, "cycle_cnt cannot be greater than tree_n.");
            chk(cycle_cnt <= n - tree_n, "cannot make cycles, cycle_cnt is too large.");
            chk(cycle_cnt || n == tree_n, "n must be equal to tree_n if cycle_cnt is 0.");

            if(!cycle_cnt) return tree(n, elongation);

            Graph tr = tree(tree_n, elongation);
            Graph ret = Graph(n);

            std::set<int> cycleNodes;
            while(cycleNodes.size() < cycle_cnt) cycleNodes.insert(rnd.next(tree_n) + 1);
            std::vector<int> cycleSizes = distribute_natural_numbers(n - tree_n, cycle_cnt);
            std::vector<std::vector<int>> nextNodes(tree_n + 1);
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

            return ret;
        }

        static Graph connected_graph(const int n, const int m, const int elongation = DEFAULT_ELONGATION) {
            chk(n >= 1, "a graph must have at least one node");
            chk(m >= n - 1, "edge count is too small to make a connected graph");
            chk(m <= n * (n - 1) / 2, "edge count is too large");
            Graph ret;
            if(elongation == DEFAULT_ELONGATION) ret = tree(n);
            else ret = tree(n, elongation);
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

        /// 일자 그래프를 생성
        /// 1--2 2--3 3--4 ... (n-1)--n 형태의 그래프를 의미
        /// first_node와 last_node를 설정하면 차수가 1인 노드가 각각 first_node와 last_node인 그래프를 반환
        static Graph path_graph(const int n, const int first_node = -1, const int last_node = -1) {
            std::vector<int> arr(n);
            for(int i = 0; i < n; i++) arr[i] = i+1;
            shuffle(arr.begin(), arr.end());

            if(first_node != -1) {
                chk(1 <= first_node && first_node <= n, "first_node is not a valid node");
                chk(first_node != last_node && n > 1, "first_node and last_node cannot be the same");
                for(int i = 0; i < n; i++) if(arr[i] == first_node) std::swap(arr[0], arr[i]);
            }

            if(last_node != -1) {
                chk(1 <= last_node && last_node <= n, "last_node is not a valid node");
                for(int i = 0; i < n; i++) if(arr[i] == last_node) std::swap(arr[n-1], arr[i]);
            }

            if(first_node != -1) assert(arr[0] == first_node);
            if(last_node != -1) assert(arr[n-1] == last_node);

            Graph ret(n);
            for(int i = 0; i < n-1; i++) ret.add_edge(arr[i], arr[i+1]);
            return ret;
        }

        /**
         * @brief generates a complete graph of n nodes
         *
         * Time complexity: O(n^2 log n)
         * @param n node count
         * @return the complete graph
         */
        static Graph complete_graph(const int n) {
            Graph ret(n);
            for(int i = 1; i <= n; i++) for(int j = i+1; j <= n; j++) ret.add_edge(i, j);
            return ret;
        }

        /**
         * @brief generates a star graph of n nodes
         *
         * Time complexity: O(n log n)
         * @param n node count
         * @param root the node to which all other nodes are connected
         * @return the complete graph
         */
        static Graph star_graph(const int n, const int root = 1) {
            Graph ret(n);
            for(int i = 1; i <= n; i++) if(i != root) ret.add_edge(root, i);
            return ret;
        }

        // path graph에서 리프 노드가 아닌 모든 노드에 리프 노드 하나를 이은 그래프
        static Graph skeleton_tree(const int n) {
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
            println(v, edges.size());
            for(auto& [u, u2] : edges) println(u - zero_base, u2 - zero_base);
        }

        void csacademy_style_print() const {
            for(int i = 1; i <= v; i++) println(i);
            for(const auto& [u, u2] : edges) println(u, u2);
        }

        void print_edges() const {
            for(const auto& [u, u2] : edges) println(u, u2);
        }
    };

#undef chk
}

#endif
