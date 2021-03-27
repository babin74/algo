#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <random>
#include <cassert>

using namespace std;

namespace persique {
    const int C = 3e7;
    struct Node {
        int a, b, x, h;
    } nd[C];
    static map<int, int> tabx;
    static map<pair<int, int>, int> tab2;
    static int nd_cnt = 0, vc = 1;

    int new_node(int a, int b) {
        nd[nd_cnt].a = a;
        nd[nd_cnt].b = b;
        int& h = tab2[{nd[a].h, nd[b].h}];
        if (h == 0) h = vc++;
        nd[nd_cnt].h = h;
        return nd_cnt++;
    }

    int new_node(int x) {
        int& h = tabx[x];
        if (h == 0) h = vc++;
        nd[nd_cnt].h = h;
        nd[nd_cnt].x = x;
        return nd_cnt++;
    }

    std::vector<int> to_vec(int i, int n) {
        if (n == 0) {
            return {  };  
        } else if (n == 1) {
            return { nd[i].x };
        } else {
            auto u = to_vec(nd[i].a, n-n/2), w = to_vec(nd[i].b, n/2);
            std::vector<int> res(n);
            for (int i = 0; i < n; ++i) {
                res[i] = i % 2 == 0 ? u[i/2] : w[i/2];
            }

            return res;
        }
    }

    int push_front(int v, int n, int x) {
        assert(v >= 0 || n == 0);
        if (n == 0) {
            return new_node(x);
        } else if (n == 1) {
            return new_node(new_node(x), v);
        } else {
            return new_node(push_front(nd[v].b, n/2, x), nd[v].a);
        }
    }

    int push_back(int v, int n, int x) {
        if (n == 0) {
            return new_node(x);
        } else if (n == 1) {
            return new_node(v, new_node(x));
        } else {
            if (n % 2 == 0) {
                return new_node(push_back(nd[v].a, n/2, x), nd[v].b);
            } else {
                return new_node(nd[v].a, push_back(nd[v].b, n/2, x));
            }
        }
    }

    int pop_front(int v, int n) {
        if (n <= 1) return -1;
        if (n == 2) {
            return nd[v].b;
        } else {
            return new_node(nd[v].b, pop_front(nd[v].a, n-n/2));
        }
    }

    int pop_back(int v, int n) {
        if (n <= 1) return -1;
        if (n == 2) {
            return nd[v].a;
        } else {
            if (n % 2 == 0) {
                return new_node(nd[v].a, pop_back(nd[v].b, n/2));
            } else {
                return new_node(pop_back(nd[v].a, n-n/2), nd[v].b);
            }
        }
    }
}

vector<int> to_vec(deque<int> d) {
    vector<int> v;
    while (!d.empty()) v.push_back(d.front()), d.pop_front();
    return v;
}

int main() {
    vector<int> v { 1, 2, 3, 4, 5, 6, 7 };
    int m = v.size();
    int h = -1;
    const int Q = 15;
    mt19937 rng(1337228);
    for (int t = Q; t--; ) {
        int j = -1, n = 0;
        int i = rng() % m;
        int l = i, r = i;
        while (l != 0 || r != m) {
            int g = rng() & 1;
            if (g == 0 && l != 0) {
                j = persique::push_front(j, n++, v[--l]);
            } else if (g == 1 && r != m) {
                j = persique::push_back(j, n++, v[r++]);
            }
        }

        cout << j << endl;
        if (h < 0) h = persique::nd[j].h;
        if (h != persique::nd[j].h) {
            cout << "FAIL" << endl;
        }
    }
}