#include <cmath>
#include <iostream>

using namespace std;

struct Node {
    int element, count;
};

Node make_data(int value) {
    Node res;
    res.element = value;
    res.count = 1;
    return res;
}

Node operation(Node l, Node r) {
    if (l.element > r.element) {
        return l;
    } else if (l.element < r.element) {
        return r;
    } else if (l.element == r.element) {
        Node res;
        res.element = l.element;
        res.count = l.count + r.count;
        return res;
    }
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

Node rmq(Node tree[], int ql, int qr, int node, int l, int r) {
    if (ql >= r || qr <= l) {
        return make_data(0);
    }

    if (l >= ql && r <= qr) {
        return tree[node];
    }

    int m = (l + r) / 2;
    Node node1 = rmq(tree, ql, qr, 2 * node + 1, l, m);
    Node node2 = rmq(tree, ql, qr, 2 * node + 2, m, r);

    return operation(node1, node2);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    int new_n = 1 << (32 - __builtin_clz(n - 1));

    int a[new_n + 1];
    Node tree[2 * new_n];

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    build_tree(tree, 0, 0, new_n, a);
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        int l, r;
        cin >> l >> r;
        Node result = rmq(tree, l - 1, r, 0, 0, new_n);
        cout << result.element << " " << result.count << "\n";
    }

    return 0;
}