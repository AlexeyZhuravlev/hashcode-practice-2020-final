#include <common.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <chrono>

using namespace std;

struct MySolver : public Context {
    vector<vector<char> > mountPointsOccupied;
    vector<char> mountPointsUsed;
    map<char, char> rev;

    int currentSteps;

    void Solve() {
        rev['U'] = 'D';
        rev['D'] = 'U';
        rev['L'] = 'R';
        rev['R'] = 'L';

        currentSteps = 0;
        mountPointsUsed.resize(mount_points_num, 0);
        mountPointsOccupied.resize(height, vector<char>(width, 0));
        forn (i, mount_points_num) {
            mountPointsOccupied[mount_points[i].y][mount_points[i].x] = 1;
        }

        vector<pair<double, int> > prioritized_tasks;
        forn(i, tasks_num) {
            double score = evaluate_task(tasks[i]);
            prioritized_tasks.push_back(make_pair(score, i));
        }
        sort(prioritized_tasks.rbegin(), prioritized_tasks.rend());
        forn(i, tasks_num) {
            int task_id = prioritized_tasks[i].second;
            if (!add_task(task_id)) {
                continue;
            }
        }
    }

    double evaluate_task(const Task& task) {
        double sum_inner_length = 0;
        for (int i = 1; i < task.points_num; ++i) {
            sum_inner_length += distance(task.points[i], task.points[i - 1]);
        }

        return double(task.s) / (sum_inner_length + 1);
    }

    int distance(const Point& a, const Point& b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }

    bool add_task(int task_id) {
        const Task& task = tasks[task_id];
        if (arms.size() < arms_num) {
            int minIdx = -1;
            int minDist = 5000;
            forn(i, mount_points_num) {
                if (mountPointsUsed[i]) {
                    continue;
                }
                int dist = distance(task.points[0], mount_points[i]);
                if (dist < minDist) {
                    minDist = dist;
                    minIdx = i;
                }
            }
            Arm arm;
            arm.mount_point = mount_points[minIdx];
            arm.tasks.push_back(task_id);
            arm.instr.resize(currentSteps, 'W');
            int new_steps = get_sequence(mount_points[minIdx], task, arm.instr);
            if (currentSteps + new_steps > steps) {
                return false;
            } else {
                add_waits(new_steps, -1);
                arms.push_back(arm);
                currentSteps += new_steps;
                mountPointsUsed[minIdx] = 1;
                return true;
            }
        } else {
            int minIdx = -1;
            int minDist = 5000;
            forn(i, arms.size()) {
                int dist = distance(task.points[0], arms[i].mount_point);
                if (dist < minDist) {
                    minDist = dist;
                    minIdx = i;
                }
            }
            Arm& arm = arms[minIdx];
            arm.tasks.push_back(task_id);
            int old_size = arm.instr.size();
            int new_steps = get_sequence(arm.mount_point, task, arm.instr);
            if (currentSteps + new_steps > steps) {
                arm.instr.resize(old_size);
                arm.tasks.pop_back();
                return false;
            } else {
                add_waits(new_steps, minIdx);
                currentSteps += new_steps;
                return true;
            }
        }
    }

    void add_waits(int wait_length, int ignore_idx) {
        forn (i, arms.size()) {
            if (i == ignore_idx) {
                continue;
            }
            arms[i].instr.resize(arms[i].instr.size() + wait_length, 'W');
        }
    }

    int get_sequence(const Point& point, const Task& task, vector<char>& sequence) {
        int start_size = sequence.size();
        Point current = point;
        vector<vector<char> > currentOccupied = mountPointsOccupied;
        forn(i, task.points_num) {
            get_point_sequence(current, task.points[i], sequence, currentOccupied);
            current = task.points[i];
        }
        int new_size = sequence.size();
        for (int i = new_size - 1; i >= start_size; --i) {
            sequence.push_back(rev[sequence[i]]);
        }
        return sequence.size() - start_size;
    }

    void get_point_sequence(const Point& a, const Point& b, vector<char>& sequence, vector<vector<char> >& currentOccupied) {
        // TODO: add A*
        Point current = a;
        while ((current.x != b.x) or (current.y != b.y)) {
            currentOccupied[current.y][current.x] = 1;
            if ((current.x < b.x) and !currentOccupied[current.y][current.x + 1]) {
                sequence.push_back('R');
                current.x += 1;
            } else if ((current.x > b.x) and !currentOccupied[current.y][current.x - 1]) {
                sequence.push_back('L');
                current.x -= 1;
            } else if ((current.y < b.y) and !currentOccupied[current.y + 1][current.x]) {
                sequence.push_back('U');
                current.y += 1;
            } else if ((current.y > b.y) and !currentOccupied[current.y - 1][current.x]) {
                sequence.push_back('D');
                current.y -= 1;
            } else {
                sequence.resize(sequence.size() + 10000, 'R');
                return;
            }
        }
    }
};

int main() {
    MySolver solver;

    solver.Input();

    auto start = std::chrono::system_clock::now();
    cerr << "Started solving..." << endl;
    solver.Solve();
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
