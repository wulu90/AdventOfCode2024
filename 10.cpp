#include <fstream>
#include <print>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

void part1() {
    fstream input("input/input-10");
    // fstream input("input-test");

    vector<string> topographicmap;
    for (string line; getline(input, line);) {
        topographicmap.push_back(line);
    }
    int rownum = topographicmap.size();
    int colnum = topographicmap.front().size();
    vector<pair<int, int>> trailheadvec;
    for (int i = 0; i < rownum; ++i) {
        for (int j = 0; j < colnum; ++j) {
            if (topographicmap[i][j] == '0') {
                trailheadvec.push_back({i, j});
            }
        }
    }

    int res = 0;

    vector<pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (auto [r_start, c_start] : trailheadvec) {
        queue<pair<int, int>> que;
        que.push({r_start, c_start});

        for (int i = 1; i <= 9; ++i) {
            auto quesize = que.size();
            set<pair<int, int>> unique_next;

            for (size_t j = 0; j < quesize; ++j) {
                auto [r_curr, c_curr] = que.front();
                for (auto [r_delta, c_delta] : dirs) {
                    auto r_next = r_curr + r_delta;
                    auto c_next = c_curr + c_delta;
                    if (r_next >= 0 && r_next < rownum && c_next >= 0 && c_next < colnum && topographicmap[r_next][c_next] == '0' + i) {
                        if (!unique_next.contains({r_next, c_next})) {
                            que.push({r_next, c_next});
                            unique_next.insert({r_next, c_next});
                        }
                    }
                }
                que.pop();
            }
        }

        res += que.size();
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
