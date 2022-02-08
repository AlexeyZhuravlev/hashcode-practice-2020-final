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
#include <random>

using namespace std;

struct MySolver : public Context {
    vector<vector<char> > mountPointsOccupied;
    vector<char> tasks_used;
    map<char, char> rev;
    double coeff = 1.0;

    void Solve() {

        TSolution& arms = Solution;
        arms.clear();

        rev['U'] = 'D';
        rev['D'] = 'U';
        rev['L'] = 'R';
        rev['R'] = 'L';

        tasks_used.assign(tasks_num, 0);
        mountPointsOccupied.assign(height, vector<char>(width, 0));
        forn (i, mount_points_num) {
            mountPointsOccupied[mount_points[i].y][mount_points[i].x] = 1;
        }

        vector<pair<double, int> > prioritized_tasks;
        forn(i, tasks_num) {
            double score = evaluate_task(tasks[i]);
            prioritized_tasks.push_back(make_pair(score, i));
        }
        sort(prioritized_tasks.rbegin(), prioritized_tasks.rend());

        int mount_point = 0;
        while (arms.size() < arms_num) {
            Arm arm;
            arm.mount_point = mount_points[mount_point];
            forn(j, tasks_num) {
                int task_id = prioritized_tasks[j].second;
                if (!tasks_used[task_id] && add_task_to_arm(task_id, arm)) {
                    tasks_used[task_id] = 1;
                }
            }
            if (arm.tasks.size() > 0) {
                arms.push_back(arm);
                Point current = arm.mount_point;
                forn(i, arm.instr.size()) {
                    switch (arm.instr[i])
                    {
                        case 'L':
                            current.x -= 1;
                            break;
                        case 'R':
                            current.x += 1;
                            break;
                        case 'U':
                            current.y += 1;
                            break;
                        case 'D':
                            current.y -= 1;
                            break;
                    }
                    mountPointsOccupied[current.y][current.x] = 1;
                }
            }
            mount_point += 1;
            if (mount_point >= mount_points_num) {
                break;
            }
        }
    }

    double evaluate_task(const Task& task) {
        double sum_inner_length = 0;
        for (int i = 1; i < task.points_num; ++i) {
            sum_inner_length += distance(task.points[i], task.points[i - 1]);
        }
        return double(task.s) - coeff * sum_inner_length;
    }

    int distance(const Point& a, const Point& b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }

    bool add_task_to_arm(int task_id, Arm& arm) {
        const Task& task = tasks[task_id];

        int old_size = arm.instr.size();

        int current_size = arm.instr.size();

        forn (i, task.points_num) {
            if (!get_point_sequence(arm.mount_point, task.points[i], arm.instr)) {
                arm.instr.resize(old_size);
                return false;
            }
            int new_size = arm.instr.size();
            for (int j = new_size - 1; j >= current_size; --j) {
                arm.instr.push_back(rev[arm.instr[j]]);
            }
            current_size = arm.instr.size();
        }

        if (arm.instr.size() >= steps) {
            arm.instr.resize(old_size);
            return false;
        } else {
            arm.tasks.push_back(task_id);
            return true;
        }
    }

    bool get_point_sequence(const Point& a, const Point& b, vector<char>& sequence) {
        // TODO: add A* (...)
        Point current = a;
        while ((current.x != b.x) || (current.y != b.y)) {
            if ((current.x < b.x) and !mountPointsOccupied[current.y][current.x + 1]) {
                sequence.push_back('R');
                current.x += 1;
            } else if ((current.x > b.x) and !mountPointsOccupied[current.y][current.x - 1]) {
                sequence.push_back('L');
                current.x -= 1;
            } else if ((current.y < b.y) and !mountPointsOccupied[current.y + 1][current.x]) {
                sequence.push_back('U');
                current.y += 1;
            } else if ((current.y > b.y) and !mountPointsOccupied[current.y - 1][current.x]) {
                sequence.push_back('D');
                current.y -= 1;
            } else {
                return false;
            }
        }
        return true;
    }

    void RandomSearch(int global_steps, int local_steps) {
        mt19937 gen(42);
        uniform_int_distribution<int> distrib(0, mount_points_num - 1);

        cerr << "Global optimizations" << endl;

        uint64_t maxScore = 0;
        double bestCoeff;
        TSolution bestSolution;
        vector<Point> best_order;
        uniform_real_distribution<double> dist(0, 2.0);

        for (int i = 0; i < global_steps; ++i) {
            shuffle(mount_points.begin(), mount_points.end(), gen);
            coeff = dist(gen);
            Solve();
            uint64_t score = GetScore();
            if (score > maxScore) {
                bestSolution = Solution;
                maxScore = score;
                best_order = mount_points;
                bestCoeff = coeff;
                cerr << "Iteration " << i << " score " << maxScore << endl;
            }
        }

        cerr << "Optimal alpha: " << bestCoeff << endl;

        cerr << "Local optimizations" << endl;

        coeff = bestCoeff;
        mount_points = best_order;
        for (int i = 0; i < local_steps; ++i) {
            int first = distrib(gen);
            int second = distrib(gen);
            swap(mount_points[first], mount_points[second]);
            Solve();
            uint64_t score = GetScore();
            if (score > maxScore) {
                bestSolution = Solution;
                maxScore = score;
                cerr << "Iteration " << i << " score " << maxScore << endl;
            } else {
                swap(mount_points[first], mount_points[second]);
            }
        }

        Solution = bestSolution;
    }
};

int main(int argc, char** argv) {
    MySolver solver;

    solver.Input();

    auto start = std::chrono::system_clock::now();
    cerr << "Started solving..." << endl;
    solver.RandomSearch(atoi(argv[1]), atoi(argv[2]));
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
