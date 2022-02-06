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

map<char, int> dx = {{'R', 1}, {'L', -1}, {'U', 0}, {'D', 0}, {'W', 0}};
map<char, int> dy = {{'R', 0}, {'L', 0}, {'U', 1}, {'D', -1}, {'W', 0}};

struct Point {
    int x, y;
    void apply(char c) {
        x += dx[c];
        y += dy[c];
    }
};

bool operator == (const Point& first, const Point& second) {
    return first.x == second.x && first.y == second.y;
}

bool operator < (const Point& first, const Point& second) {
    if (first.x != second.x) {
        return first.x < second.x;
    }
    return first.y < second.y;
}

struct Task {
    int s, points_num;
    vector<Point> points;
};

struct Arm {
    Point mount_point;
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
            cout << arm.mount_point.x << " " << arm.mount_point.y <<
                 " " << arm.tasks.size() << " " << arm.instr.size() << endl;
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

    bool valid(const Point& p) {
        return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
    }


    uint64_t GetScore() {
        set<Point> set_mount_points(all(mount_points));
        assert(arms.size() > 0 && (int)arms.size() <= arms_num && "arm number should be <= R");
        set<int> assigned_tasks;
        for (const Arm& arm: arms) {
            assert(valid(arm.mount_point));
            assert(arm.tasks.size() > 0 && (int)arm.tasks.size() <= tasks_num && "number of tasks should be > 0 and <= T");
            assert(arm.instr.size() > 0 && (int)arm.instr.size() <= steps && "number of instructions should be > 0 and <= L");
            assert(set_mount_points.count(arm.mount_point) == 1 && "mount point not found");
            for (int task: arm.tasks) {
                assert(assigned_tasks.count(task) == 0 && "can't assign tasks more than once");
                assigned_tasks.insert(task);
            }
        }
        vector<vector<Point>> nodes(arms.size());
        forn(i, arms.size()) {
            nodes[i].pb(arms[i].mount_point);
        }
        vector<pii> task_state(arms.size());
        for (int t = 0; t < steps; ++t) {
            forn(i, arms.size()) {
                Arm& arm = arms[i];
                // skip if we wait (W)
                if ((int)arm.instr.size() <= t || arm.instr[t] == 'W') {
                    continue;
                }
                Point new_node = nodes[i].back();
                new_node.apply(arm.instr[t]);
                assert(valid(new_node) && "moving outside the field");
                cout << new_node.x << " " << new_node.y << endl;
                assert((set_mount_points.count(new_node) == 0 || new_node == arm.mount_point) && "can't visit other mount points");

                while (task_state[i].fi < (int)arm.tasks.size()) {
                    int task_id = arm.tasks[task_state[i].fi];
                    if (new_node == tasks[task_id].points[task_state[i].se]) {
                        ++task_state[i].se;
                        if (task_state[i].se == tasks[task_id].points_num) {
                            task_state[i] = {task_state[i].fi + 1, 0};
                        }
                    } else {
                        break;
                    }
                }

                if (nodes[i].size() == 1) {
                    nodes[i].pb(new_node);
                    continue;
                }
                Point last_but_one = nodes[i][nodes[i].size() - 2];
                // cout << "last but one " << last_but_one.x << " " << last_but_one.y << endl;
                if (last_but_one == new_node) {
                    nodes[i].pop_back();
                } else {
                    nodes[i].pb(new_node);
                }
            }
            vector<vi> field(width, vi(height));
            forn(i, arms.size()) {
                for (auto node: nodes[i]) {
                    // cout << "node " << node.x << " " << node.y << endl;
                    assert(field[node.x][node.y] == 0 && "cell is already occupied");
                    field[node.x][node.y] = i + 1; 
                }
            }
            cout << "after step " << t << endl;
            forn(i, width) {
                forn(j, height) {
                    cout << field[i][j];
                }
                cout << endl;
            }
        } 
        forn(i, arms.size()) {
            pii expected = {arms[i].tasks.size(), 0};
            assert((task_state[i] == expected) && "not all tasks are done");
        }
        li total_score = 0;
        forn(i, arms.size()) {
            for (int t: arms[i].tasks) {
                total_score += tasks[t].s;
            }
        }
        cerr << "total score " << total_score << endl;
        return total_score;
    }
    
};
