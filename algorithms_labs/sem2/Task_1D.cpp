#include <cmath>
#include <iostream>

using namespace std;

struct Node {
    int sum;
};

Node make_data(int value) {
    Node res;

    if (value == 0) {
        res.sum = 1;
    } else {
        res.sum = 0;
    }

    return res;
}

Node operation(Node l, Node r) {
    Node res;

    res.sum = l.sum + r.sum;

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
        return make_data(-1);
    }

    if (l >= ql && r <= qr) {
        return tree[node];
    }

    int m = (l + r) / 2;
    Node node1 = query(tree, ql, qr, 2 * node + 1, l, m);
    Node node2 = query(tree, ql, qr, 2 * node + 2, m, r);

    return operation(node1, node2);
}

int find_kth(Node tree[], int node, int l, int r, int k) {
    cerr << l << " " << r  << " " << k << "\n";
    if (tree[node].sum < k) {
        return -1;
    }

    if (r - l == 1) {
        return l + 1;
    }

    int m = (l + r) / 2;

    if (tree[node * 2 + 1].sum >= k) {
        return find_kth(tree, node * 2 + 1, l, m, k);
    } else {
        return find_kth(tree, node * 2 + 2, m, r, (k - tree[node * 2 + 1].sum));
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    int new_n = 1 << (32 - __builtin_clz(n - 1));

    int a[new_n];
    Node tree[2 * new_n];

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for (int i = n; i < new_n; ++i) {
        a[i] = -1;
    }

    build_tree(tree, 0, 0, new_n, a);

    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        string request;
        cin >> request;
        if (request == "s") {
            int l, r, k;
            cin >> l >> r >> k;
            cout << find_kth(tree, 0, --l, r + 2, k) << "\n";
        } else if (request == "u") {
            int i, x;
            cin >> i >> x;
            set(tree, i - 1, x, 0, 0, new_n);
        }
    }

    return 0;
}