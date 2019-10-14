#include <iostream>
#include <vector>

using namespace std;

int INF = 1000000000;

struct Node {
    int left, right, add = -1;
    bool seq;
    Node * child_left, * child_right;
};

Node * build(int left, int right, const vector<int> & a) {
    Node * res = new Node;
    res->left = left;
    res->right = right;
    res->add = -1;
    if (left == right) {
        res->child_left = res->child_right = nullptr;
        res->seq = true;
    } else {
        int mid = (left + right) / 2;
        res->child_left = build(left, mid, a);
        res->child_right = build(mid + 1, right, a);
        if (res->child_left->seq && res->child_right->seq &&
            a[res->child_left->right] <= a[res->child_right->left]) {
            res->seq = true;
        } else {
            res->seq = false;
        }
    }
    return res;
}

void push(Node * root, vector<int> & a) {
    if (root->add != -1) {
        root->child_left->add = root->child_right->add = root->add;
        root->child_left->seq = root->child_right->seq = true;
        a[root->child_left->left] = a[root->child_left->right] = root->add;
        a[root->child_right->left] = a[root->child_right->right] = root->add;
        root->add = -1;
    }
}

void update(Node * root, int l, int r, int val, vector<int> & a) {
    if (l > root->right || r < root->left)
        return;
    if (root->left == l && root->right == r) {
        root->add = val;
        root->seq = true;
        a[root->right] = val;
        a[root->left] = val;
    } else {
        push(root, a);
        int mid = (root->left + root->right) / 2;
        update(root->child_left, l, min(mid, r), val, a);
        update(root->child_right, max(mid + 1, l), r, val, a);
        if (root->child_left->seq && root->child_right->seq &&
            a[root->child_left->right] <= a[root->child_right->left]) {
            root->seq = true;
        } else {
            root->seq = false;
        }
    }
}

bool query(Node * root, int l, int r, vector<int> & a) {
    if (l >= root->left && r <= root->right && root->seq)
        return true;
    if (l > r)
        return true;
    if (l == root->left && r == root->right)
        return root->seq;
    else {
        int mid = (root->left + root->right) / 2;
        auto res1 = query(root->child_left, l, min(mid, r), a);
        auto res2 = query(root->child_right, max(mid + 1, l), r, a);
        auto res3 = true;
        if (l <= min(mid, r) && r >= max(mid + 1, l))
            res3 = (a[root->child_left->right] <= a[root->child_right->left]);
        return res1 && res2 && res3;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i != n; ++i)
        cin >> v[i];
    Node * root = build(0, n - 1, v);
    int operation, l, r, x;
    for (int i = 0; i != m; ++i) {
        cin >> operation >> l >> r;
        if (operation == 1) {
            cin >> x;
            update(root, l - 1, r - 1, x, v);
        } else {
            if (query(root, l - 1, r - 1, v))
                cout << "Yes" << '\n';
            else
                cout << "No" << '\n';
        }
    }
}
