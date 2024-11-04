#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <set>

using namespace std;

#define ll int
#define matrix vector<vector<ll>>
#define vc vector<ll>

void delete_edge(matrix &mx, ll x, ll y) {
    mx[x].erase(std::find(mx[x].begin(), mx[x].end(), y));
    mx[y].erase(std::find(mx[y].begin(), mx[y].end(), x));
}

ll find_parent(const matrix &mx, ll x) {
    if (!mx[x].empty()) {
        return mx[x][0];
    }
    return x;
}

vector<pair<ll, ll> > build_tree_from_prufer(const vc &pruferCode, ll n) {
    vc degs(n + 1, 1);
    for (ll node: pruferCode) {
        degs[node]++;
    }

    set<ll> leaves;
    for (ll i = 1; i <= n; i++) {
        if (degs[i] == 1) {
            leaves.insert(i);
        }
    }

    vector<pair<ll, ll> > edges;
    for (ll node: pruferCode) {
        ll leaf = *leaves.begin();
        leaves.erase(leaves.begin());

        edges.emplace_back(leaf, node);
        degs[node]--;
        if (degs[node] == 1) {
            leaves.insert(node);
        }
    }

    auto it = leaves.begin();
    ll lastLeaf1 = *it, lastLeaf2 = *(++it);
    edges.emplace_back(lastLeaf1, lastLeaf2);

    return edges;
}


int main() {
    ll n;
    cin >> n;
    vc pruferCode(n - 2);
    for (ll i = 0; i < n - 2; i++) {
        cin >> pruferCode[i];
    }

    auto edges = build_tree_from_prufer(pruferCode, n);
    for (auto & edge : edges) {
        cout << edge.first << " " << edge.second << '\n';
    }

    return 0;
}
