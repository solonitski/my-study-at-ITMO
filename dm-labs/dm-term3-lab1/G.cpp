#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define ll int
#define vc vector<ll>
#define vcb vector<bool>
#define table vector<vc>

void dfs(ll now, const table &tb, vcb &is_visited, vc &colors, ll &max_color) {
    is_visited[now] = true;
    vcb used_colors(tb.size(), false);

    ll k = 0;
    for (ll neighbor: tb[now]) {
        if (colors[neighbor] != 0) {
            used_colors[colors[neighbor]] = true;
            max_color = max(max_color, ++k);
        }
    }

    max_color = max(max_color, k);

    for (ll color = 1; color < tb.size(); ++color) {
        if (!used_colors[color]) {
            colors[now] = color;
            max_color = max(max_color, color);
            break;
        }
    }

    for (ll i = 0; i < tb[now].size(); i++) {
        ll neighbor = tb[now][i];
        if (!is_visited[neighbor]) {
            dfs(neighbor, tb, is_visited, colors, max_color);
        }
    }
}

void input_data(ll &n, ll &m, table &tb) {
    cin >> n >> m;
    tb.resize(n + 1);
    for (ll i = 0; i < m; ++i) {
        ll a, b;
        cin >> a >> b;
        tb[a].push_back(b);
        tb[b].push_back(a);
    }
}

void output_results(ll max_color, const vc &colors, ll n) {
    ll k = max_color + static_cast<int>(max_color % 2 == 0);
    cout << k << '\n';
    for (ll i = 1; i <= n; ++i) {
        cout << colors[i] << '\n';
    }
}

int main() {
    ll n, m;
    table tb;
    input_data(n, m, tb);

    vcb is_visited(n + 1, false);
    vc colors(n + 1, 0);
    ll max_color = -1;

    dfs(1, tb, is_visited, colors, max_color);

    for (const auto &v: tb) {
        max_color = max(max_color, static_cast<int>(v.size()));
    }

    output_results(max_color, colors, n);

    return 0;
}
