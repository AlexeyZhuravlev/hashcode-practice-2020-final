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
    int s, points_num;
    vector<Point> points;
};

struct Arm {
    int x, y, tasks_num, instr_num;
    vector<int> tasks;
    vector<char> instr;
};

struct Context {
    using TSolution = int;
    TSolution Solution;
    int width, height, arms_num, mount_points_num, tasks_num, steps;
    vector<Point> mount_points;
    vector<Task> tasks;

    // answer    
    vector<Arm> arms;

    void Input() {
        cin >> width >> height >> arms_num >> mount_points_num >> tasks_num >> steps;
        mount_points.resize(mount_points_num);
        forn(i, mount_points_num) {
            cin >> mount_points[i].x >> mount_points[i].y;
        }
        tasks.resize(tasks_num);
        forn(i, tasks_num) {
            Task& task = tasks[i];
            cin >> task.s >> task.points_num;
            task.points.resize(task.points_num);
            forn(j, task.points_num) {
                cin >> task.points[j].x >> task.points[j].y;
            }
        }
    }

    void Output() {
        cout << arms.size() << endl;
        for (const Arm& arm: arms) {
            cout << arm.x << " " << arm.y << " " << arm.tasks_num << " " << arm.instr_num << endl;
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
