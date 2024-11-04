#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define ll int
#define matrix vector<vector<bool>>
#define vc vector<ll>
#define vcb vector<bool>

bool has_edge(const matrix &mx, ll x, ll y) {
    return mx[x][y];
}

matrix read_adjacency_matrix(const ll n) {
    matrix mx(n, vcb(n, false));

    for (ll i = 1; i < n; ++i) {
        string line;
        cin >> line;

        for (ll j = 0; j < i; ++j) {
            (line[j] == '1' ? mx[i][j] : mx[j][i]) = true;
        }
    }

    return mx;
}

void dfs(const matrix &mx, vcb &is_visited, vc &cycle, ll now) {
    is_visited[now] = true;
    for (ll i = 0; i < is_visited.size(); ++i) {
        if (!is_visited[i] && has_edge(mx, now, i)) {
            dfs(mx, is_visited, cycle, i);
        }
    }
    cycle.push_back(now);
}

void print_cycle(const vc &cycle) {
    for (ll i = cycle.size() - 1; i >= 0; i--) {
        cout << cycle[i] + 1 << ' ';
    }
    cout << '\n';
}

int main() {
    ll n;
    cin >> n;
    matrix mx = read_adjacency_matrix(n);

    for (ll i = 0; i < n; i++) {
        vcb is_visited(n, false);
        vc cycle;
        dfs(mx, is_visited, cycle, i);

        if (cycle.size() == n && has_edge(mx, cycle[0], cycle.back())) {
            print_cycle(cycle);
            return 0;
        }
    }
}
