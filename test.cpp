//#include "bits/stdc++.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>

#include "list.h"
#define puba push_back
#define ff first
#define ss second
#define bend(_x) begin(_x), end(_x)
#define szof(_x) ((int) (_x).size())
#define TASK_NAME ""

using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
const int INF = 1e9 + 7;
const ll INFL = 1e18 + 123;
//const double PI = atan2(0, -1);
//mt19937 tw(960172);
//ll rnd(ll x, ll y) { static uniform_int_distribution<ll> d; return d(tw) % (y - x + 1) + x; }

using namespace budin;

int main() {
    //freopen(TASK_NAME ".in", "r", stdin);
    //freopen(TASK_NAME ".out", "w", stdout);
    //cerr << fixed << setprecision(15);
    //cout << fixed << setprecision(15);
    

    list<int> l;
    for (int i = 0; i < 5; ++i) {
        cerr << i << endl;
        l.push_back(i);
    }
    l.erase(++l.begin());
    l.insert(--l.end(), 5);
    for (auto num : l) {
        cerr << num << " ";
    }
    cerr << endl;

    auto it = l.begin();
    l.push_front(-1);
    while (it != l.end()) {
        cerr << *it << " ";
        ++it;
    }
    cerr << endl;

    cerr << "here" << endl;
    list<int> l2 = l;
    for (int num : l2) {
        cerr << num << " ";
    }
    cerr << endl;

    l2.rbegin();
    l2.rend();

    {
        //list<int>::iterator it = l2.rbegin();
    }

    l2.erase(++(++l2.begin()));
    swap(l, l2);
    for (int num : l) {
        cerr << num << " ";
    }
    cerr << endl;

    for (int num : l2) {
        cerr << num << " ";
    }
    cerr << endl;


    int c = 0;
    while (true) {
        c++;
        if (c % 100 == 0) {
            cerr << c << endl;
        }
        list<int> l1, l2;

        for (int i = 0; i < 1000; ++i) {
            l1.push_back(i);
            l2.push_back(i);
        }
        
        for (int i = 0; i < 10; ++i) {
            auto it = l1.begin();
            int skip = rand() % 10;
            for (int j = 0; j < skip; ++j) {
                ++it;
            }
            l1.erase(it);
        }

        for (int i = 0; i < 10; ++i) {
            auto it = l1.end();
            int skip = rand() % 10;
            for (int j = 0; j < skip; ++j) {
                --it;
            }
            l1.insert(it, rand());
        }
        auto it1 = l1.begin();
        for (int i = 0; i < 100; ++i) {
            ++it1;
        }
        auto it2 = it1;
        for (int i = 0; i < 100; ++i) {
            ++it2;
        }

        l2.splice(++l2.begin(), l1, it1, it2);
    }
    
    
    
    //#ifdef LOCAL
    //    cerr << "time: " << clock() << endl;
    //#endif
    return 0;
}