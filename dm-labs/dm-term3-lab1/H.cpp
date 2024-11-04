#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define ll int
#define matrix vector<vector<bool>>
#define vc vector<ll>
#define vcb vector<bool>

bool has_self_loops(const matrix &mx, ll n) {
    for (ll i = 1; i <= n; ++i) {
        if (mx[i][i]) {
            return true;
        }
    }
    return false;
}

bool is_edge_less(const matrix &mx, ll n) {
    for (ll i = 1; i <= n; ++i) {
        for (ll j = i + 1; j <= n; ++j) {
            if (mx[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool get_edge(const matrix &mx, ll n, ll &u, ll &v) {
    for (ll i = 1; i <= n; ++i) {
        for (ll j = i + 1; j <= n; ++j) {
            if (mx[i][j]) {
                u = i;
                v = j;
                return true;
            }
        }
    }
    return false;
}

matrix delete_edge(const matrix &mx, ll u, ll v) {
    matrix mxDelete = mx;
    mxDelete[u][v] = mxDelete[v][u] = false;
    return mxDelete;
}

matrix contract_edge(const matrix &mx, ll n, ll u, ll v, ll &newN) {
    newN = n - 1;
    matrix mxContract = mx;

    for (ll i = 1; i <= n; ++i) {
        if (i != u && mx[v][i]) {
            mxContract[u][i] = mxContract[i][u] = true;
        }
    }

    for (ll i = 1; i <= n; ++i) {
        mxContract[v][i] = mxContract[i][v] = false;
    }

    mxContract[u][u] = false;

    matrix mxResult(newN + 1, vcb(newN + 1, false));
    ll idx_i = 1;
    for (ll i = 1; i <= n; ++i) {
        if (i == v) continue;
        ll idx_j = 1;
        for (ll j = 1; j <= n; ++j) {
            if (j == v) continue;
            mxResult[idx_i][idx_j] = mxContract[i][j];
            ++idx_j;
        }
        ++idx_i;
    }

    return mxResult;
}

vc chromatic_polynomial(const matrix &mx, ll n) {
    if (has_self_loops(mx, n)) {
        vc zeroPoly(n + 1, 0);
        return zeroPoly;
    }

    if (is_edge_less(mx, n)) {
        vc t_n(n + 1, 0);
        t_n[n] = 1;
        return t_n;
    }

    ll u = -1, v = -1;
    get_edge(mx, n, u, v);

    matrix mxWithDeleteEdge = delete_edge(mx, u, v);
    vc polyDelete = chromatic_polynomial(mxWithDeleteEdge, n);

    ll newN;
    matrix adjContract = contract_edge(mx, n, u, v, newN);
    vc polyContract = chromatic_polynomial(adjContract, newN);

    vc result(max(polyDelete.size(), polyContract.size()), 0);
    for (size_t i = 0; i < polyDelete.size(); ++i)
        result[i] += polyDelete[i];
    for (size_t i = 0; i < polyContract.size(); ++i)
        result[i] -= polyContract[i];

    return result;
}

void print_polynomial(const vc &poly, ll degree) {
    cout << degree << endl;
    for (ll i = static_cast<ll>(poly.size()) - 1; i >= 0; --i) {
        cout << poly[i] << ' ';
    }
    cout << '\n';
}

int main() {
    ll n, m;
    cin >> n >> m;

    matrix adj(n + 1, vcb(n + 1, false));
    for (ll i = 0; i < m; ++i) {
        ll u, v;
        cin >> u >> v;
        adj[u][v] = adj[v][u] = true;
    }

    vc ans = chromatic_polynomial(adj, n);
    print_polynomial(ans, n);

    return 0;
}
