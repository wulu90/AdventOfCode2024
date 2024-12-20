#include <fstream>
#include <map>
#include <print>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input("input/input-20");
    vector<string> trackmap;
    for (string line; getline(input, line);) {
        trackmap.push_back(line);
    }
    auto rownum = trackmap.size();
    auto colnum = trackmap.front().size();

    pair<size_t, size_t> start;
    pair<size_t, size_t> end;

    size_t tmpcol = 0;
    for (size_t i = 0; i < trackmap.size(); ++i) {
        if ((tmpcol = trackmap[i].find('S')) != string::npos) {
            start = {i, tmpcol};
        }
        if ((tmpcol = trackmap[i].find('E')) != string::npos) {
            end = {i, tmpcol};
        }
    }
    trackmap[end.first][end.second] = '.';
    vector<pair<size_t, size_t>> trackcoords;
    map<pair<size_t, size_t>, size_t> coord_idx;
    vector<array<int, 2>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    auto curr = start;
    size_t i  = 0;
    while (curr != end) {
        trackcoords.push_back(curr);
        coord_idx.insert({curr, i});

        for (auto [rd, cd] : dirs) {
            auto r = curr.first + rd;
            auto c = curr.second + cd;
            if (r < rownum && c < colnum && trackmap[r][c] == '.' && !coord_idx.contains({r, c})) {
                curr = {r, c};
                ++i;
                break;
            }
        }
    }
    trackcoords.push_back(end);
    coord_idx.insert({end, i});

    int count = 0;
    for (auto [i, j] : trackcoords) {
        for (auto [rd, cd] : dirs) {
            auto r  = i + rd;
            auto c  = j + cd;
            auto r1 = i + rd * 2;
            auto c1 = j + cd * 2;
            if (r < rownum && c < colnum && r1 < rownum && c1 < colnum && trackmap[r][c] == '#' && trackmap[r1][c1] == '.') {
                if (coord_idx[{r1, c1}] > coord_idx[{i, j}] && coord_idx[{r1, c1}] - coord_idx[{i, j}] >= 102) {
                    ++count;
                }
                // if (coord_idx[{r1, c1}] > coord_idx[{i, j}] ) {
                //     println("{}", coord_idx[{r1, c1}] - coord_idx[{i, j}]-2);
                // }
            }
        }
    }

    println("{}", count);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
