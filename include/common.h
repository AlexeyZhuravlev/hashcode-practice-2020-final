#pragma once

#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <unordered_map>

using namespace std;

#define mp make_pair
#define pb push_back
#define fi first
#define se second
#define li long long
#define pii pair<int, int>
#define vi vector<int>

#define forn(i, n) for (int i = 0; i < (int)n; i++)
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define all(x) (x).begin(), (x).end()

struct Point {
    int x, y;
};

struct Task {
    int s, p;
    vector<Point> points;
};

struct Arm {
    int x, y, z, k;
    vector<int> tasks;
    vector<char> instr;
};

struct Context {
    using TSolution = int;
    TSolution Solution;
    int w, h, r, m, t, l;
    vector<Point> mount_points;
    vector<Task> tasks;

    // answer    
    vector<Arm> arms;

    void Input() {
        cin >> w >> h >> r >> m >> t >> l;
        mount_points.resize(m);
        forn(i, m) {
            cin >> mount_points[i].x >> mount_points[i].y;
        }
        tasks.resize(t);
        forn(i, t) {
            Task& task = tasks[i];
            cin >> task.s >> task.p;
            task.points.resize(task.p);
            forn(j, task.p) {
                cin >> task.points[j].x >> task.points[j].y;
            }
        }
    }

    void Output() {
        cout << arms.size() << endl;
        for (const Arm& arm: arms) {
            cout << arm.x << " " << arm.y << " " << arm.z << " " << arm.k << endl;
            for (int task: arm.tasks) {
                cout << task << " ";
            }
            cout << endl;
            for (char i: arm.instr) {
                cout << i << " ";
            }
            cout << endl;
        }
    }

    uint64_t GetScore() {
        return 0;
    }
    
};
