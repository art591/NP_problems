#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <stack>
#include <math.h>
#include <random>
#include <time.h>
#include<chrono>
#include <limits>



using namespace std;

const long long INF = numeric_limits<long long>::max();
/*
 План:
 1)Минимальное остовное дерево - алгоритм Крускала
 2) Удваиваем ребра
 3) Находим Эйлеров цикл
 4) Выписываем вершины без повторений
 */
long double find_dist(long double x1, long double y1, long double x2, long double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//  Для Крускала
vector<long long> parent;
vector<long long> s;


void make_set (long long v) {
    parent[v] = v;
    s[v] = 1;
}

long long find_set (long long v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set (parent[v]);
}

void union_sets (long long a, long long b) {
    a = find_set (a);
    b = find_set (b);
    if (a != b) {
        if (s[a] < s[b])
            swap (a, b);
        parent[b] = a;
        s[a] += s[b];
    }
}

pair<long double, vector<long long>> two_approximate_algo(long long n, vector<pair<long double, long double>> & nodes) {
    pair<long double, long double> v1, v2;
    vector<tuple<long double, long long, long long>> E(n * ( n - 1) / 2);
    long long count = 0;
    for (long long i = 0; i < n; ++i){
        for (long long j = i + 1; j < n; ++j) {
            v1 = nodes[i];
            v2 = nodes[j];
            long double dist = find_dist(v1.first, v1.second, v2.first, v2.second);

            E[count] = tuple<long double, long long, long long>{dist, i, j};
            count++;
        }
    }

    // Крускал
    parent.resize(n);
    s.resize(n);
    for (long long i = 0; i < n; ++i)
        make_set(i);
    vector<unordered_multiset<long double>> T(n);
    sort(E.begin(), E.end());
    for (size_t i = 0; i != E.size(); ++i) {
        long long a = get<1>(E[i]), b = get<2>(E[i]);
        long double l = get<0>(E[i]);
        if (find_set(a) != find_set(b)) {
            T[a].insert(b);
            T[a].insert(b);
            T[b].insert(a);
            T[b].insert(a);
            union_sets(a, b);
        }
    }
    // Эйлеров цикл

    stack<long long> st;
    vector<long long> ans;
    st.push(0);
    unordered_set<long long> repeat;
    while (!st.empty()) {
        long long v = st.top();
        if (T[v].empty()) {
            st.pop();
            if (repeat.find(v) == repeat.end()) {
                ans.push_back(v);
                repeat.insert(v);
            }
        } else {
            long long u = *(T[v].begin());
            T[v].erase(T[v].begin());
            auto it = T[u].begin();
            for (; it != T[u].end(); ++it) {
                if (*it == v)
                    break;
            }
            T[u].erase(it);
            st.push(u);
        }
    }
    ans.push_back(0);
    long double sum = 0;
    for (size_t i = 0; i + 1 < ans.size(); ++i) {
        sum += find_dist(nodes[ans[i]].first, nodes[ans[i]].second, nodes[ans[i + 1]].first, nodes[ans[i + 1]].second);
    }
    return {sum, ans};
}

pair<long double, vector<long long>> greedy_algo(long long n, vector<pair<long double, long double>> & nodes) {

    unordered_set<long long> s;
    for (long long i = 1; i < n; ++i) {
        s.insert(i);
    }

    long double sum = 0;
    long long v = 0;
    vector<long long> ans;
    ans.push_back(v);
    for (long long i = 0; i + 1 < n; ++i) {
        long long min_dist = INF;
        long long min_v = -1;
        for (auto el : s) {
            long long d = find_dist(nodes[v].first, nodes[v].second, nodes[el].first, nodes[el].second);
            if (d <= min_dist) {
                min_dist = d;
                min_v = el;
            }
        }
        v = min_v;
        ans.push_back(v);
        s.erase(min_v);

    }
    ans.push_back(0);
    for (size_t i = 0; i + 1 < ans.size(); ++i) {
        sum += find_dist(nodes[ans[i]].first, nodes[ans[i]].second, nodes[ans[i + 1]].first, nodes[ans[i + 1]].second);
    }
    return {sum, ans};
}

void permutations(vector<long long> & str, long long i, long long & n, long double sum, long double & minnum, vector<long long> & ans, vector<vector<long double>> & a)
{
    // base condition
    if (i == n - 1)
    {
        if (sum + a[str[i]][str[i - 1]] + a[str[i]][0] <= minnum) {

            ans = str;
            minnum = sum + a[str[i]][str[i - 1]] + a[str[i]][0];

        }
        return;
    }
    // process each character of the remaining string
    for (long long j = i; j < n; j++)
    {
        // swap character at index i with current character
        swap(str[i], str[j]);        // STL swap() used

        // recurse for string [i+1, n-1]
        if (sum + a[str[i]][str[i - 1]] <= minnum)
            permutations(str, i + 1, n, sum + a[str[i]][str[i - 1]], minnum, ans, a);


        // backtrack (restore the string to its original state)
        swap(str[i], str[j]);
    }
}
vector<long long> brabr(long long n, vector<pair<long double, long double>> & nodes, long double border) {
    vector<vector<long double>> a(n, vector<long double>(n));
    pair<long double, long double> v1, v2;
    for (long long i = 0; i < n; ++i){
        for (long long j = i; j < n; ++j) {
            v1 = nodes[i];
            v2 = nodes[j];
            long double dist = find_dist(v1.first, v1.second, v2.first, v2.second);
            a[i][j] = dist;
            a[j][i] = dist;
        }
    }
    vector<long long> nodes2(n);
    for (size_t i = 0; i < n; ++i) {
        nodes2[i] = i;
    }


    long long start = 0;
    long long res = INF;
    vector<long long> ans;
    long double minnum = border;
    long double sum = 0;
    permutations(nodes2, 1, n, sum, minnum, ans, a);
    return ans;

}


void otjig(vector<long long> & ans, vector<vector<long double>> & a, long double curr, std::chrono::steady_clock::time_point start) {
    int i1, j1;

    /* initialize random seed: */
    srand(0);
    int count = 1;
    while (true) {
        if(std::chrono::steady_clock::now() - start > std::chrono::seconds(9))
            break;
        i1 = rand() % (a.size() - 1) + 1;
        j1 = rand() % (a.size() - 1) + 1;
        int x = min(i1, j1), y = max(i1, j1);
        reverse(ans.begin() + x, ans.begin() + y);
        long double sum = 0;
        for (size_t i = 0; i + 1 < ans.size(); ++i) {
            sum += a[ans[i]][ans[i + 1]];
        }
 
        if (sum >= curr) {
            reverse(ans.begin() + x, ans.begin() + y);
        } else {
            curr = sum;
        }
        ++count;
    }
    return;
}
int main() {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    long long n;
    long long BORDER = 0;
    int smth;
    cin >> n;
    vector<pair<long double, long double>> nodes(n);
    long long x, y;
    for (long long i = 0; i < n; ++i) {
        cin >> x >> y;
        nodes[i] = pair<long double, long double>{x, y};
    }
    if (n == 0)
        return 0;
    if (n > 1) {
        long long B_2 = 30;
        pair<long double, vector<long long>> r1 = two_approximate_algo(n, nodes);
        pair<long double, vector<long long>> r2 = greedy_algo(n, nodes);
        if (n > BORDER) { // НЕ ЗАБЫТЬ ВЕРНУТЬ на >
            long double border;
            vector<long long> ans;
            if (r1.first <= r2.first) {
                border = r1.first;
                ans = r1.second;
            } else {
                border = r2.first;
                ans = r2.second;
            }
            vector<vector<long double>> a(n, vector<long double>(n));
            pair<long double, long double> v1, v2;
            for (long long i = 0; i < n; ++i){
                for (long long j = i; j < n; ++j) {
                    v1 = nodes[i];
                    v2 = nodes[j];
                    long double dist = find_dist(v1.first, v1.second, v2.first, v2.second);
                    a[i][j] = dist;
                    a[j][i] = dist;
                }
            }

            if (n <= B_2) {
                long double curr = border;
                for (int i = 1; i + 1 < n; ++i) {
                    for (int j = 1; j + 1 < n; ++j) {
                        int x = min(i, j), y = max(i, j);
                        reverse(ans.begin() + x, ans.begin() + y);
                        long double sum = 0;
                        for (size_t k = 0; k + 1 < ans.size(); ++k) {
                            sum += a[ans[k]][ans[k + 1]];
                        }
                        if (sum >= curr) {
                            reverse(ans.begin() + x, ans.begin() + y);
                        } else {
                            curr = sum;
                        }


                    }
                }
            } else {
                otjig(ans, a, border, start);
            }
            for (auto el : ans) {
                cout << el + 1 << ' ';
            }
        } else {
            long double border;
            if (r1.first <= r2.first) {
                border = r1.first;
            } else {
                border = r2.first;
            }
            vector<long long> ans = brabr(n, nodes, border);
            for (auto el : ans) {
                cout << el + 1 << ' ';
            }
            cout << 1;
        }

    } else {
        cout << 1;
    }

};
