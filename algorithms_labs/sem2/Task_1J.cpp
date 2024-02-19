#include <cmath>
#include <iostream>

using namespace std;

#define ll long long

struct Node {
     ll sum, pref, suff, ans;
};

Node make_data(ll value) {
    Node res;

    res.sum = value;
    if (value > 0) {
        res.pref = res.suff = res.ans = value;
    } else {
        res.pref = res.suff = res.ans = 0;
    }

    return res;
}

Node operation(Node l, Node r) {
    Node res;

    res.sum = l.sum + r.sum;
    res.pref = max(l.pref, l.sum + r.pref);
    res.suff = max(r.suff, l.suff + r.sum);
    res.ans = max(max(r.ans, l.ans), l.suff + r.pref);

    return res;
}

void build_tree(Node tree[], int node, int lx, int rx, int a[]) {
    if (rx - lx == 1) {
        tree[node] = make_data(a[lx]);
        return;
    }
    int m = (lx + rx) / 2;
    build_tree(tree, node * 2 + 1, lx, m, a);
    build_tree(tree, node * 2 + 2, m, rx, a);

    tree[node] = operation(tree[2 * node + 1], tree[2 * node + 2]);
}

void set(Node tree[], int index, int value, int node, int lx, int rx) {
    if (rx - lx == 1) {
        tree[node] = make_data(value);
        return;
    }
    int m = (lx + rx) / 2;
    if (index < m) {
        set(tree, index, value, 2 * node + 1, lx, m);
    } else {
        set(tree, index, value, 2 * node + 2, m, rx);
    }
    tree[node] = operation(tree[2 * node + 1], tree[2 * node + 2]);
}

Node query(Node tree[], int ql, int qr, int node, int l, int r) {
    if (ql >= r || qr <= l) {
        return make_data(0);
    }

    if (l >= ql && r <= qr) {
        return tree[node];
    }

    int m = (l + r) / 2;
    Node node1 = query(tree, ql, qr, 2 * node + 1, l, m);
    Node node2 = query(tree, ql, qr, 2 * node + 2, m, r);

    return operation(node1, node2);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, k;
    cin >> n >> k;
    int new_n = 1 << (32 - __builtin_clz(n - 1));

    int a[new_n];
    Node tree[2 * new_n];

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for (int i = n; i < new_n; ++i) {
        a[i] = 0;
    }

    build_tree(tree, 0, 0, new_n, a);

    for (int i = 0; i < k; ++i) {
        string request;
        cin >> request;
        if (request == "get") {
            int l, r;
            cin >> l >> r;
            Node res = query(tree, l - 1, r, 0, 0, new_n);
            cout << res.ans << "\n";
        } else if (request == "change") {
            int i, x;
            cin >> i >> x;
            set(tree, i - 1, x, 0, 0, new_n);
        }
    }

    return 0;
}