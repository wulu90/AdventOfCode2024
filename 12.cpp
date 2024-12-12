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

    vector<string> gardenplots;
    for (string line; getline(input, line);) {
        gardenplots.push_back(line);
    }

    int rownum = gardenplots.size();
    int colnum = gardenplots.front().size();
    vector<pair<int, int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    map<int, set<pair<int, int>>> id_coords;    // group coord;
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

    int res  = 0;
    int res2 = 0;

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

    // 1 has side
    // 0 has neighbour no side
    // -1 share side with up / left
    vector<array<int, 4>> sides_count(rownum * colnum);

    for (auto [_, coords] : id_coords) {
        int area  = coords.size();
        int index = 0;
        for (auto [r, c] : coords) {
            for (int i = 0; i < 4; ++i) {
                // check facing neighbour exits
                if (coords.contains({r + dirs[i].first, c + dirs[i].second})) {
                    sides_count[r * colnum + c][i] = 0;
                } else {
                    // check up/left neighbour exits,than check same side status
                    int up_left = 0;

                    switch (i) {
                    case 0:
                    case 2:
                        up_left = 3;
                        break;
                    default:
                        up_left = 0;
                        break;
                    }
                    int r_upleft = r + dirs[up_left].first;
                    int c_upleft = c + dirs[up_left].second;
                    if (coords.contains({r_upleft, c_upleft})) {
                        if (sides_count[r_upleft * colnum + c_upleft][i] == 1) {
                            sides_count[r * colnum + c][i] = -1;
                        } else if (sides_count[r_upleft * colnum + c_upleft][i] == 0) {
                            sides_count[r * colnum + c][i] = 1;
                        } else {
                            sides_count[r * colnum + c][i] = -1;
                        }
                    } else {
                        sides_count[r * colnum + c][i] = 1;
                    }
                }
            }
        }

        int sides = 0;
        for (auto [r, c] : coords) {
            for (int i = 0; i < 4; ++i) {
                if (sides_count[r * colnum + c][i] == 1) {
                    ++sides;
                }
            }
        }
        res2 += area * sides;
    }

    println("{}", res);
    println("{}", res2);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
