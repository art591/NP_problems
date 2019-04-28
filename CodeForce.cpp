#include <iostream>
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
#include<set>
#include <unordered_map>


using namespace std;

vector<int> parent;
vector<int> s;


void make_set (int v) {
    parent[v] = v;
    s[v] = 1;
}

int find_set (int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set (parent[v]);
}

void union_sets (int a, int b) {
    a = find_set (a);
    b = find_set (b);
    if (a != b) {
        if (s[a] < s[b])
            swap (a, b);
        parent[b] = a;
        s[a] += s[b];
    }
}

void FindAns(int k, int s, set<pair<int, int>>& ans, vector<vector<int>>& dp,
        vector<pair<int, int>>& res) {
    if (dp[k][s] == 0)
        return;
    if (dp[k][s] == dp[k - 1][s]) {
        FindAns(k - 1, s, ans, dp, res);
    } else {
        FindAns(k - 1, s - res[k - 1].first, ans, dp, res);
        ans.insert({res[k - 1].second, k - 1});
    }
}

int main() {
    int numOfflats, cabels;
    cin >> numOfflats >> cabels;
    parent.resize(numOfflats);
    s.resize(numOfflats);
    for (size_t i = 0; i != numOfflats; ++i) {
        make_set(i);
    }
    vector<tuple<int, int, int, int>> flats(numOfflats);
    unordered_map<int, int> m;
    int f1, f2, l;
    vector<pair<int, int>> flatstosort(numOfflats);
    for (size_t i = 0; i != cabels; ++i) {
        cin >> f1 >> f2 >> l;
        flats.push_back({l, f1 - 1, f2 - 1,  i});
    }
    int p5, q5, p6, q6;
    cin >> p5 >> q5 >> p6 >> q6;
    int minp = min(p5, p6), otherp = max(p5, p6);
    int minq, otherq, c1, c2;
    if (p5 == minp) {
        c1 = 5;
        c2 = 6;
        minq = q5;
        otherq = q6;
    } else {
        c2 = 5;
        c1 = 6;
        minq = q6;
        otherq = q5;
    }
    vector<pair<int, int>> res(numOfflats - 1);
    sort(flats.begin(), flats.end());
    int count = 0;
    for (size_t i = 0; i != flats.size(); ++i) {
        int a = get<1>(flats[i]), b = get<2>(flats[i]);
        int l = get<0>(flats[i]);
        if (find_set(a) != find_set(b)) {
            res[count] = {l, get<3>(flats[i])};
            union_sets(a, b);
            ++count;
        }
    }
    int summ = 0;
    if (!res.empty()) {
        if (res.back() == pair{0, 0}) {
            cout << "Impossible";
            return 0;
        }
    } else if (numOfflats > 1) {
        cout << "Impossible";
        return 0;
    }
    vector<vector<int>> dp(res.size() + 1, vector<int>(minq + 1));
    for (size_t i = 1; i <= res.size(); ++i) {
        for (size_t j = 1; j <= minq; ++j) {
            int s = res[i - 1].first;
            if (j >= s) {
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - s] + s);
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }
    set<pair<int, int>> ans;
    set<int> ans1;
    FindAns(res.size(), minq, ans, dp, res);
    int w2 = 0;
    for (size_t i = 0; i != res.size(); ++i) {
        int s = res[i].first;
        if (ans.find({res[i].second, i}) == ans.end()) {
            summ += otherp  * s;
            w2 += s;
            if (w2 > otherq) {
                cout << "Impossible";
                return 0;
            }
            ans1.insert(res[i].second);
        } else {
            summ += minp * s;
        }
    }
    cout << summ << '\n';
    for (auto el : ans) {
        cout << el.first + 1 << ' ' << c1 << '\n';
    }
    for (auto el : ans1) {
        cout << el + 1 << ' ' << c2 << '\n';
    }
}
