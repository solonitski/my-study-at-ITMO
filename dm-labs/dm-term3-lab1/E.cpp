#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include <cstdint>

using namespace std;

#define ll int
#define matrix vector<vector<ll>>
#define vc vector<ll>

void delete_edge(matrix& mx, ll x, ll y) {
    mx[x].erase(std::find(mx[x].begin(), mx[x].end(), y));
    mx[y].erase(std::find(mx[y].begin(), mx[y].end(), x));
}

ll find_parent(matrix& mx, ll x) {
    if (!mx[x].empty()) {
        return mx[x][0];
    }
    return x;
}

int main() {
    ll n;
    cin >> n;
    matrix tree(n + 1);

    for (size_t i = 0; i < n - 1; ++i) {
        ll a, b;
        cin >> a >> b;
        tree[a].push_back(b);
        tree[b].push_back(a);
    }

    priority_queue<ll, vc, greater<ll>> leaves;
    for (size_t i = 1; i <= n; ++i) {
        if (tree[i].size() == 1) {
            leaves.push(i);
        }
    }

    for (size_t i = 0; i < n - 2; ++i) {
        if (leaves.empty()) break;
        ll item = leaves.top();
        leaves.pop();
        ll parent = find_parent(tree, item);

        delete_edge(tree, item, parent);
        if (tree[parent].size() == 1) {
            leaves.push(parent);
        }

        cout << parent << " ";
    }

    return 0;
}
