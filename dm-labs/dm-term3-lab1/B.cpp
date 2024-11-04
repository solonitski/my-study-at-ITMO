#include <iostream>
#include <vector>
#include <deque>
#include <string>

using namespace std;

#define ll int
#define matrix vector<string>
#define vc vector<ll>

bool has_edge(const matrix &mx, ll x, ll y) {
    if (x < y) {
        swap(x, y);
    }
    return mx[x][y] == '1';
}

void swap_sub_queue_by_t1nol(deque<ll> &queue, ll begin, ll end) {
    while (begin < end) {
        swap(queue[begin++], queue[end--]);
    }
}

//source -- https://neerc.ifmo.ru/wiki/index.php?title=%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%BD%D0%B0%D1%85%D0%BE%D0%B6%D0%B4%D0%B5%D0%BD%D0%B8%D1%8F_%D0%93%D0%B0%D0%BC%D0%B8%D0%BB%D1%8C%D1%82%D0%BE%D0%BD%D0%BE%D0%B2%D0%B0_%D1%86%D0%B8%D0%BA%D0%BB%D0%B0_%D0%B2_%D1%83%D1%81%D0%BB%D0%BE%D0%B2%D0%B8%D1%8F%D1%85_%D1%82%D0%B5%D0%BE%D1%80%D0%B5%D0%BC_%D0%94%D0%B8%D1%80%D0%B0%D0%BA%D0%B0_%D0%B8_%D0%9E%D1%80%D0%B5
vc find_hamiltonian_cycle(const matrix &mx, ll n) {
    deque<ll> queue;
    for (ll i = 0; i < n; i++) {
        queue.push_back(i);
    }

    for (ll k = 0; k < n * (n - 1); k++) {
        if (!has_edge(mx, queue[0], queue[1])) {
            ll i = 2;
            bool flag = false;
            while ((i < n - 1) && (!has_edge(mx, queue[0], queue[i])
                                   || !has_edge(mx, queue[1], queue[i + 1 < n ? i + 1 : 0]))) {
                i++;
            }
            if (i >= n - 1) {
                i = 2;
                while ((i < n) && !has_edge(mx, queue[0], queue[i])) {
                    ++i;
                }
            }
            swap_sub_queue_by_t1nol(queue, 1, i);
        }
        queue.push_back(queue.front());
        queue.pop_front();
    }

    vc result(queue.begin(), queue.end());
    return result;
}

int main() {
    ll n;
    cin >> n;

    matrix mx(n + 1);
    for (ll i = 1; i < n; i++) {
        cin >> mx[i];
    }

    vc cycle = find_hamiltonian_cycle(mx, n);

    for (ll v: cycle) {
        cout << v + 1 << ' ';
    }

    return 0;
}
