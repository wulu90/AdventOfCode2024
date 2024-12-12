#include <cstddef>
#include <fstream>
#include <map>
#include <print>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input("input/input-12");
    // ifstream input("input-test");

    vector<string> gardenplots;
    for (string line; getline(input, line);) {
        gardenplots.push_back(line);
    }

    int rownum = gardenplots.size();
    int colnum = gardenplots.front().size();
    vector<pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    map<int, set<pair<int, int>>> id_coords;
    vector<vector<bool>> visited(rownum, vector<bool>(colnum, false));
    int id = -1;
    for (int i = 0; i < rownum; ++i) {
        for (int j = 0; j < colnum; ++j) {
            if (visited[i][j]) {
                continue;
            }

            ++id;
            visited[i][j] = true;
            id_coords[id].insert({i, j});
            queue<pair<int, int>> que;
            que.push({i, j});

            while (!que.empty()) {
                auto qsize = que.size();
                for (size_t k = 0; k < qsize; ++k) {
                    auto [r_cur, c_cur] = que.front();
                    for (auto [rd, cd] : dirs) {
                        auto r_nei = r_cur + rd;
                        auto c_nei = c_cur + cd;
                        if (r_nei >= 0 && r_nei < rownum && c_nei >= 0 && c_nei < colnum &&
                            gardenplots[i][j] == gardenplots[r_nei][c_nei] && !visited[r_nei][c_nei]) {
                            id_coords[id].insert({r_nei, c_nei});
                            visited[r_nei][c_nei] = true;
                            que.push({r_nei, c_nei});
                        }
                    }
                    que.pop();
                }
            }
        }
    }

    int res = 0;

    for (auto [_, coords] : id_coords) {
        int area      = coords.size();
        int perimeter = area * 4;
        for (auto [r, c] : coords) {
            for (auto [rd, cd] : dirs) {
                auto r_nei = r + rd;
                auto c_nei = c + cd;

                if (r_nei >= 0 && r_nei < rownum && c_nei >= 0 && c_nei < colnum && coords.contains({r_nei, c_nei})) {
                    --perimeter;
                }
            }
        }
        res += area * perimeter;
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
