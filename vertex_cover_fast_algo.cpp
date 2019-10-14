#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <set>

using namespace std;

bool found = false;
void brute_force(int index, vector<int> & nodes, vector<set<int>> & gr, vector<bool> & cover, int k) {
    if (found) {
        return;
    }
    if (k == 0) {
        bool if_cover = true;
        for (int i = 0; i < gr.size(); ++i) {
            for (auto el : gr[i]) {
                if (cover[i] == 0 && cover[el] == 0) {
                    if_cover = false;
                    break;
                }
            }
        }
        if (if_cover) {
            found = true;
        }
        return;
    }
    int copy = index;
    while (copy < gr.size() && cover[nodes[copy]] == 1) {
        copy++;
    }
    if (copy >= gr.size())
        return;
    int node = nodes[copy];
    if (copy >= gr.size()) {
        return;
    } else {
            vector<int> in_cover;
            in_cover.reserve(gr[node].size());
            for (auto el : gr[node]) {
                if (cover[el] == 0) {
                    in_cover.push_back(el);
                }
            }
            if (in_cover.size() <= k) {
                for (auto el : in_cover) {
                    cover[el] = 1;
                }
                brute_force(index + 1, nodes, gr, cover, k - in_cover.size());
                if (!found) {
                    for (auto el : in_cover) {
                        cover[el] = 0;
                    }
                }
            }
            if (in_cover.size() > 1 && !found) {
                cover[node] = 1;
                brute_force(index + 1, nodes, gr, cover, k - 1);
                if (!found) {
                    cover[node] = 0;
                }
            }
    }
}

vector<set<int>> gr;

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    int v1, v2;
    gr.resize(n);
    vector<int> nodes(n);
    for (size_t i = 0; i < m; ++i) {
        if (i < n)
            nodes[i] = i;
        cin >> v1 >> v2;
        gr[v1 - 1].insert(v2 - 1);
        gr[v2 - 1].insert(v1 - 1);
    }
    vector<bool> cover(n, false);

    int counter = 0;
    for (int i = 0; i < n; ++i) {
        if (gr[i].size() > k) {
            cover[i] = 1;
            for (int j = 0; j < n; ++j) {
                gr[j].erase(i);
            }
            gr[i].clear();
            counter++;
        }
    }
    k -= counter;
    for (int i = 0; i < n; ++i) {
        if (gr[i].size() == 1) {
            int v = 0;
            for (auto el : gr[i]) {
                cover[el] = 1;
                v = el;
            }
            for (int j = 0; j < n; ++j) {
                gr[j].erase(i);
                gr[j].erase(v);
            }
            gr[i].clear();
            gr[v].clear();
            k--;
        }
    }
    sort(nodes.begin(), nodes.end(),
         [](int n1, int n2) {
             return (gr[n1].size() > gr[n2].size());
         });
    int vx = 0;
    brute_force(vx, nodes, gr, cover, k);
    if (found)
        cout << "Yes" << '\n';
    else
        cout << "No" << '\n';
    if (found)
        for (int i = 0; i < n; ++i) {
            if (cover[i] == 1) {
                cout << i + 1 << ' ';
            }
        }
    cout << '\n';
};

