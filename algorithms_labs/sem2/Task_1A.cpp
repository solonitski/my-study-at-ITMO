#include <iostream>
#include <cmath>

using namespace std;

int NEUTRAL = -1000000001;

int operation(int first_index, int second_index, int a[]) {
    return a[first_index] > a[second_index] ? first_index : second_index;
}

void build_tree(int tree[], int node, int lx, int rx, int indexes[], int a[]) {
    if (rx - lx == 1) {
        tree[node] = indexes[lx];
        return;
    }
    int m = (lx + rx) / 2;
    build_tree(tree, node * 2 + 1, lx, m, indexes, a);
    build_tree(tree, node * 2 + 2, m, rx, indexes, a);

    tree[node] = operation(tree[2 * node + 1], tree[2 * node + 2], a);
}

void set(int tree[], int index, int value, int node, int lx, int rx, int a[]) {
    if (rx - lx <= 1) {
        tree[node] = value;
        return;
    }
    int m = (lx + rx) / 2;
    if (index < m) {
        set(tree, index, value, 2 * node + 1, lx, m, a);
    } else {
        set(tree, index, value, 2 * node + 2, m, rx, a);
    }
    tree[node] = operation(tree[2 * node + 1], tree[2 * node + 2], a);
}

int rmq(int tree[], int ql, int qr, int node, int l, int r, int a[]) {
    if (ql >= r || qr <= l) {
        return NEUTRAL;
    }

    if (l >= ql && r <= qr) {
        return tree[node];
    }

    int m = (l + r) / 2;
    int node1 = rmq(tree, ql, qr, 2 * node + 1, l, m, a);
    int node2 = rmq(tree, ql, qr, 2 * node + 2, m, r, a);

    return operation(node1, node2, a);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n; cin >> n;
    int new_n = 1 << (32 - __builtin_clz(n - 1));

    int a[new_n + 1], indexes[new_n + 1], tree[2 * new_n];
    a[new_n] = -1000000001;
    NEUTRAL = new_n;

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for (int i = 0; i <= new_n; ++i) {
        indexes[i] = i;
    }

    build_tree(tree, 0, 0, new_n, indexes, a);
    int k; cin >> k;
    for (int i = 0; i < k; ++i) {
        int l, r;
        cin >> l >> r;
        int index = rmq(tree, l - 1, r, 0, 0, new_n, a);
        cout << a[index] << " " << (index + 1) << "\n";
    }
    return 0;
}