#include <iostream>
#include <vector>

using namespace std;

#define ll int
#define matrix vector<string>
#define vc vector<ll>

bool question(ll Xi, ll Yi) {
    cout << "1 " << Xi << " " << Yi << '\n';
    cout.flush();

    string answer;
    cin >> answer;

    return answer == "YES";
}

vc merge(const vc &left, const vc &right) {
    vc merged;
    size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (question(left[i], right[j])) {
            merged.push_back(left[i]);
            i++;
        } else {
            merged.push_back(right[j]);
            j++;
        }
    }
    while (i < left.size()) {
        merged.push_back(left[i]);
        i++;
    }
    while (j < right.size()) {
        merged.push_back(right[j]);
        j++;
    }
    return merged;
}

vc mergeSort(const vc &arr) {
    if (arr.size() <= 1) {
        return arr;
    }
    size_t m = arr.size() / 2;
    vc left(arr.begin(), arr.begin() + m);
    vc right(arr.begin() + m, arr.end());
    return merge(mergeSort(left), mergeSort(right));
}

int main() {
    ll n;
    cin >> n;

    vc lamps(n);
    for (ll i = 0; i < n; ++i) {
        lamps[i] = i + 1;
    }

    vc sorted_lamps = mergeSort(lamps);
    cout << "0 ";
    for (ll i = 0; i < n; ++i) {
        cout << sorted_lamps[i] << ' ';
    }
    cout.flush();

    return 0;
}
