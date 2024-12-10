#include <fstream>
#include <print>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

void solve() {
    fstream input("input/input-10");

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

    int res1 = 0;
    int res2 = 0;

    vector<pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (auto [r_start, c_start] : trailheadvec) {
        queue<pair<int, int>> que;
        que.push({r_start, c_start});

        for (int i = 1; i <= 9; ++i) {
            auto quesize = que.size();

            for (size_t j = 0; j < quesize; ++j) {
                auto [r_curr, c_curr] = que.front();
                for (auto [r_delta, c_delta] : dirs) {
                    auto r_next = r_curr + r_delta;
                    auto c_next = c_curr + c_delta;
                    if (r_next >= 0 && r_next < rownum && c_next >= 0 && c_next < colnum && topographicmap[r_next][c_next] == '0' + i) {
                        que.push({r_next, c_next});
                    }
                }
                que.pop();
            }
        }

        res2 += que.size();

        set<pair<int, int>> uniquetail;
        while (!que.empty()) {
            uniquetail.insert(que.front());
            que.pop();
        }
        res1 += uniquetail.size();
    }

    println("{}", res1);
    println("{}", res2);
}

int main(int argc, char* argv[]) {
    solve();
    return 0;
}
