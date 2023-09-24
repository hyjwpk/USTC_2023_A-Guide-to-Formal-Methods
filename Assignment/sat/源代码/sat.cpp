#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

time_t start_time, end_time;

void unit_resolution(int x, vector<vector<int>> &clauses, vector<int> &assignment) {
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            if (clauses[i][j] == x) {
                clauses.erase(clauses.begin() + i);
                i--;
                break;
            } else if (clauses[i][j] == -x) {
                clauses[i].erase(clauses[i].begin() + j);
                break;
            }
        }
    }
    assignment[abs(x) - 1] = x;
}

void unit_resolution(int x, vector<vector<int>> &clauses, vector<int> &assignment, vector<int> &index) {
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            if (clauses[i][j] == x) {
                clauses.erase(clauses.begin() + i);
                index.erase(index.begin() + i);
                i--;
                break;
            } else if (clauses[i][j] == -x) {
                clauses[i].erase(clauses[i].begin() + j);
                break;
            }
        }
    }
    assignment[abs(x) - 1] = x;
}

// find the variable with the most occurrences
int var_selection1(int n, vector<vector<int>> clauses) {
    int x = 0;
    vector<int> count;
    for (int i = 0; i < n; i++) {
        count.push_back(0);
    }
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            count[abs(clauses[i][j]) - 1]++;
        }
    }
    int max = 0;
    for (int i = 0; i < n; i++) {
        if (count[i] > max) {
            max = count[i];
            x = i + 1;
        }
    }
    return x;
}

// find the smallest clause and return the first variable in it
int var_selection2(int n, vector<vector<int>> clauses) {
    int x = 0;
    int min = n + 1;
    for (int i = 0; i < clauses.size(); i++) {
        if (clauses[i].size() < min) {
            min = clauses[i].size();
            x = abs(clauses[i][0]);
        }
    }
    return x;
}

// find the smallest clause and return the variable with the most occurrences in it
int var_selection3(int n, vector<vector<int>> clauses) {
    int x = 0;
    int min_index = 0;
    int min = n + 1;
    for (int i = 0; i < clauses.size(); i++) {
        if (clauses[i].size() < min) {
            min = clauses[i].size();
            min_index = i;
        }
    }

    vector<int> count;
    for (int i = 0; i < n; i++) {
        count.push_back(0);
    }
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            count[abs(clauses[i][j]) - 1]++;
        }
    }
    int max = 0;
    for (int j = 0; j < clauses[min_index].size(); j++) {
        if (count[abs(clauses[min_index][j]) - 1] > max) {
            max = count[abs(clauses[min_index][j]) - 1];
            x = abs(clauses[min_index][j]);
        }
    }
    return x;
}

// VSIDS heuristic
int var_selection4(int n, vector<vector<int>> clauses, vector<pair<float, float>> &activity, vector<int> assignment) {
    int x = 0;
    float max = -1;
    for (int i = 0; i < n; i++) {
        if (assignment[i] == 0) {
            if (activity[i].first > max) {
                max = activity[i].first;
                x = i + 1;
            }
            if (activity[i].second > max) {
                max = activity[i].second;
                x = -(i + 1);
            }
        }
        activity[i].first = activity[i].first * 0.90;
        activity[i].second = activity[i].second * 0.90;
    }
    int threshold = 10;
    if ((x > 0 && activity[x - 1].first < threshold) || (x < 0 && activity[-x - 1].second < threshold)) {
        x = var_selection3(n, clauses);
    }
    return x;
}

// Jeroslow-Wang heuristic
int var_selection5(int n, vector<vector<int>> clauses) {
    int x = 0;
    vector<pair<float, float>> count;
    for (int i = 0; i < n; i++) {
        count.push_back(make_pair(0, 0));
    }
    for (int i = 0; i < clauses.size(); i++) {
        for (int j = 0; j < clauses[i].size(); j++) {
            if (clauses[i][j] > 0) {
                count[abs(clauses[i][j]) - 1].first += 1 / pow(2, clauses[i].size());
            } else {
                count[abs(clauses[i][j]) - 1].second += 1 / pow(2, clauses[i].size());
            }
        }
    }
    float max = 0;
    for (int i = 0; i < n; i++) {
        if (count[i].first + count[i].second > max) {
            max = count[i].first + count[i].second;
            if (count[i].first > count[i].second) {
                x = i + 1;
            } else {
                x = -(i + 1);
            }
        }
    }
    return x;
}

void dpll(int n, vector<vector<int>> clauses, vector<int> assignment) {
    // unit resolution
    {
        bool change = true;
        while (change) {
            change = false;
            for (int i = 0; i < clauses.size(); i++) {
                if (clauses[i].size() == 1) {
                    for (int j = 0; j < clauses.size(); j++) {
                        if (clauses[j].size() == 1 && clauses[i][0] == -clauses[j][0]) { // conflict
                            return;
                        }
                    }
                    change = true;
                    unit_resolution(clauses[i][0], clauses, assignment);
                    i = -1;
                }
            }
        }
    }

    // // pure literal elimination
    // {
    //     vector<pair<int, int>> pure;
    //     for (int i = 0; i < n; i++) {
    //         pure.push_back(make_pair(0, 0));
    //     }
    //     for (int i = 0; i < clauses.size(); i++) {
    //         for (int j = 0; j < clauses[i].size(); j++) {
    //             if (clauses[i][j] > 0) {
    //                 pure[abs(clauses[i][j]) - 1].first++;
    //             } else {
    //                 pure[abs(clauses[i][j]) - 1].second++;
    //             }
    //         }
    //     }
    //     for (int i = 0; i < n; i++) {
    //         if ((pure[i].first != 0 && pure[i].second == 0) ||
    //             (pure[i].first == 0 && pure[i].second != 0)) {
    //             unit_resolution((pure[i].first != 0) ? (i + 1) : -(i + 1), clauses, assignment);
    //         }
    //     }
    // }

    // choose a variable
    int x = 0;
    if (clauses.size() != 0) {
        x = var_selection3(n, clauses);
    } else {
        for (int i = 0; i < n; i++) {
            if (assignment[i] == 0) {
                x = i + 1;
                break;
            }
        }
    }

    // find a solution
    if (x == 0) {
        end_time = clock();
        cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
        cout << "SAT" << endl;
        for (int i = 0; i < n; i++) {
            cout << assignment[i] << " ";
        }
        cout << "0" << endl;
        exit(0);
    }

    // branch
    vector<int> assumption;
    assumption.push_back(x);
    clauses.push_back(assumption);
    dpll(n, clauses, assignment);
    clauses[clauses.size() - 1][0] = -x;
    dpll(n, clauses, assignment);
}

bool dpll_opt(int n, vector<vector<int>> clauses, vector<int> assignment) {
    // a list of variables that have been assigned and the decision flag
    vector<pair<int, bool>> M;

    vector<vector<vector<int>>> clauses_stack;
    vector<vector<int>> assignment_stack;

    while (true) {
        // unit resolution
        {
            bool change = true;
            while (change) {
                change = false;
                for (int i = 0; i < clauses.size(); i++) {
                    if (clauses[i].size() == 1) {
                        change = true;
                        bool conflict = false;
                        for (int j = 0; j < clauses.size(); j++) {
                            if (clauses[j].size() == 1 && clauses[i][0] == -clauses[j][0]) { // conflict
                                if (clauses_stack.size() == 0) {
                                    return false;
                                } else {
                                    clauses = clauses_stack[clauses_stack.size() - 1];
                                    clauses_stack.pop_back();
                                    assignment = assignment_stack[assignment_stack.size() - 1];
                                    assignment_stack.pop_back();
                                    for (int k = M.size() - 1; k >= 0; k--) {
                                        if (M[k].second == false) {
                                            M.pop_back();
                                        } else {
                                            break;
                                        }
                                    }
                                    M[M.size() - 1].first = -M[M.size() - 1].first;
                                    M[M.size() - 1].second = false;
                                    unit_resolution(M[M.size() - 1].first, clauses, assignment);
                                    conflict = true;
                                    break;
                                }
                            }
                        }
                        if (!conflict) {
                            M.push_back(make_pair(clauses[i][0], false));
                            unit_resolution(clauses[i][0], clauses, assignment);
                        }
                        break;
                    }
                }
            }
        }

        // pure literal elimination
        {
            vector<pair<int, int>> pure;
            for (int i = 0; i < n; i++) {
                pure.push_back(make_pair(0, 0));
            }
            for (int i = 0; i < clauses.size(); i++) {
                for (int j = 0; j < clauses[i].size(); j++) {
                    if (clauses[i][j] > 0) {
                        pure[abs(clauses[i][j]) - 1].first++;
                    } else {
                        pure[abs(clauses[i][j]) - 1].second++;
                    }
                }
            }
            for (int i = 0; i < n; i++) {
                if ((pure[i].first != 0 && pure[i].second == 0) ||
                    (pure[i].first == 0 && pure[i].second != 0)) {
                    M.push_back(make_pair((pure[i].first != 0) ? (i + 1) : -(i + 1), false));
                    unit_resolution((pure[i].first != 0) ? (i + 1) : -(i + 1), clauses, assignment);
                }
            }
        }

        // choose a variable
        int x = 0;
        if (clauses.size() != 0) {
            x = var_selection3(n, clauses);
        }

        // find a solution
        if (x == 0) {
            end_time = clock();
            cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
            cout << "SAT" << endl;
            for (int i = 0; i < n; i++) {
                if (assignment[i] == 0) {
                    assignment[i] = i + 1;
                }
                cout << assignment[i] << " ";
            }
            cout << "0" << endl;
            return true;
        }

        // branch
        clauses_stack.push_back(clauses);
        assignment_stack.push_back(assignment);
        M.push_back(make_pair(x, true));
        unit_resolution(x, clauses, assignment);
    }
}

bool cdcl(int n, vector<vector<int>> clauses, vector<int> assignment, int &learn_count, int &backjump_count) {
    // a list of variables that have been assigned and the decision flag
    vector<pair<int, bool>> M;
    // vector<pair<float, float>> activity;
    // for (int i = 0; i < n; i++) {
    //     activity.push_back(make_pair(0, 0));
    // }

    // a list of index of clauses now
    vector<int> index;
    for (int i = 0; i < clauses.size(); i++) {
        index.push_back(i);
    }
    vector<vector<int>> original_clauses = clauses;

    vector<vector<vector<int>>> clauses_stack;
    vector<vector<int>> assignment_stack;
    vector<vector<int>> index_stack;

    while (true) {
        // unit resolution
        {
            bool change = true;
            while (change) {
                change = false;
                for (int i = 0; i < clauses.size(); i++) {
                    if (clauses[i].size() == 1) {
                        change = true;
                        bool conflict = false;
                        for (int j = 0; j < clauses.size(); j++) {
                            if (clauses[j].size() == 1 && clauses[i][0] == -clauses[j][0]) { // conflict
                                if (clauses_stack.size() == 0) {
                                    return false;
                                } else {
                                    learn_count++;
                                    vector<int> conflict_clause;
                                    // activity[abs(clauses[i][0]) - 1].first += 1;
                                    // activity[abs(clauses[i][0]) - 1].second += 1;
                                    for (int k = 0; k < original_clauses[index[i]].size(); k++) {
                                        if (original_clauses[index[i]][k] != clauses[i][0]) {
                                            if (find(conflict_clause.begin(), conflict_clause.end(), original_clauses[index[i]][k]) == conflict_clause.end()) {
                                                conflict_clause.push_back(original_clauses[index[i]][k]);
                                                // if (original_clauses[index[i]][k] > 0) {
                                                //     activity[abs(original_clauses[index[i]][k]) - 1].first += 1;
                                                // } else {
                                                //     activity[abs(original_clauses[index[i]][k]) - 1].second += 1;
                                                // }
                                            }
                                        }
                                    }
                                    for (int k = 0; k < original_clauses[index[j]].size(); k++) {
                                        if (original_clauses[index[j]][k] != clauses[j][0]) {
                                            if (find(conflict_clause.begin(), conflict_clause.end(), original_clauses[index[j]][k]) == conflict_clause.end()) {
                                                conflict_clause.push_back(original_clauses[index[j]][k]);
                                                // if (original_clauses[index[j]][k] > 0) {
                                                //     activity[abs(original_clauses[index[j]][k]) - 1].first += 1;
                                                // } else {
                                                //     activity[abs(original_clauses[index[j]][k]) - 1].second += 1;
                                                // }
                                            }
                                        }
                                    }
                                    // original_clauses.push_back(conflict_clause);
                                    // backjump
                                    int m = 0;
                                    for (int k = 0; k < M.size(); k++) {
                                        if (conflict_clause.size() == 1) {
                                            m++;
                                            if (m < clauses_stack.size()) {
                                                while (k < M.size() && M[k].second == false) {
                                                    k++;
                                                }
                                                M.erase(M.begin() + k, M.end());
                                            }
                                            break;
                                        }

                                        // if (M[k].second == true) {
                                        //     clauses_stack[m].push_back(conflict_clause);
                                        //     index_stack[m].push_back(original_clauses.size() - 1);
                                        // }

                                        auto it = find(conflict_clause.begin(), conflict_clause.end(), -M[k].first);
                                        if (it != conflict_clause.end()) {
                                            conflict_clause.erase(it);
                                        }

                                        if (M[k].second == true) {
                                            m++;
                                        }
                                    }
                                    if (m < clauses_stack.size()) {
                                        backjump_count++;
                                        clauses_stack.erase(clauses_stack.begin() + m, clauses_stack.end());
                                        assignment_stack.erase(assignment_stack.begin() + m, assignment_stack.end());
                                        index_stack.erase(index_stack.begin() + m, index_stack.end());
                                        M.push_back(make_pair(conflict_clause[0], false));
                                    } else { // backtrack
                                        for (int k = M.size() - 1; k >= 0; k--) {
                                            if (M[k].second == false) {
                                                M.pop_back();
                                            } else {
                                                break;
                                            }
                                        }
                                        M[M.size() - 1].first = -M[M.size() - 1].first;
                                        M[M.size() - 1].second = false;
                                    }
                                    clauses = clauses_stack[clauses_stack.size() - 1];
                                    clauses_stack.pop_back();
                                    assignment = assignment_stack[assignment_stack.size() - 1];
                                    assignment_stack.pop_back();
                                    index = index_stack[index_stack.size() - 1];
                                    index_stack.pop_back();
                                    unit_resolution(M[M.size() - 1].first, clauses, assignment, index);
                                    conflict = true;
                                    break;
                                }
                            }
                        }
                        if (!conflict) {
                            M.push_back(make_pair(clauses[i][0], false));
                            unit_resolution(clauses[i][0], clauses, assignment, index);
                        }
                        break;
                    }
                }
            }
        }

        // // pure literal elimination
        // {
        //     vector<pair<int, int>> pure;
        //     for (int i = 0; i < n; i++) {
        //         pure.push_back(make_pair(0, 0));
        //     }
        //     for (int i = 0; i < clauses.size(); i++) {
        //         for (int j = 0; j < clauses[i].size(); j++) {
        //             if (clauses[i][j] > 0) {
        //                 pure[abs(clauses[i][j]) - 1].first++;
        //             } else {
        //                 pure[abs(clauses[i][j]) - 1].second++;
        //             }
        //         }
        //     }
        //     for (int i = 0; i < n; i++) {
        //         if ((pure[i].first != 0 && pure[i].second == 0) ||
        //             (pure[i].first == 0 && pure[i].second != 0)) {
        //             M.push_back(make_pair((pure[i].first != 0) ? (i + 1) : -(i + 1), false));
        //             unit_resolution((pure[i].first != 0) ? (i + 1) : -(i + 1), clauses, assignment, index);
        //         }
        //     }
        // }

        // choose a variable
        int x = 0;
        if (clauses.size() != 0) {
            x = var_selection3(n, clauses);
        }

        // find a solution
        if (x == 0) {
            end_time = clock();
            cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
            cout << "SAT" << endl;
            for (int i = 0; i < n; i++) {
                if (assignment[i] == 0) {
                    assignment[i] = i + 1;
                }
                cout << assignment[i] << " ";
            }
            cout << "0" << endl;
            return true;
        }

        // branch
        clauses_stack.push_back(clauses);
        assignment_stack.push_back(assignment);
        index_stack.push_back(index);
        M.push_back(make_pair(x, true));
        unit_resolution(x, clauses, assignment, index);
    }
}

int main() {
    string input_file;
    cout << "Input file: ";
    cin >> input_file;
    freopen(input_file.c_str(), "r", stdin);

    char c;
    string s;
    int n, m; // n: number of variables, m: number of clauses
    while (true) {
        cin >> c;
        if (c == 'c') { // comment
            getline(cin, s);
        } else if (c == 'p') { // problem description
            cin >> s >> n >> m;
            break;
        }
    }

    vector<vector<int>> clauses;
    vector<int> assignment;
    for (int i = 0; i < m; i++) {
        int x;
        vector<int> clause;
        while (true) {
            cin >> x;
            if (x == 0) {
                break;
            }
            clause.push_back(x);
        }
        clauses.push_back(clause);
    }
    for (int i = 0; i <= n; i++) {
        assignment.push_back(0);
    }

    int backjump_count = 0;
    int learn_count = 0;
    bool find;

    // cdcl
    cout << "---cdcl---" << endl;
    start_time = clock();
    find = cdcl(n, clauses, assignment, learn_count, backjump_count);
    end_time = clock();

    if (!find) {
        cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
        cout << "UNSAT" << endl;
    }
    cout << "backjump_count:" << backjump_count << endl;
    cout << "learn_count:" << learn_count << endl;

    // dpll_opt
    cout << "---dpll_opt---" << endl;
    start_time = clock();
    find = dpll_opt(n, clauses, assignment);
    end_time = clock();

    if (!find) {
        cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
        cout << "UNSAT" << endl;
    }

    // dpll
    cout << "---dpll---" << endl;
    start_time = clock();
    dpll(n, clauses, assignment);
    end_time = clock();

    cout << "Time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;
    cout << "UNSAT" << endl;
}