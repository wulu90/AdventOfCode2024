#include <cstddef>
#include <fstream>
#include <print>
#include <set>
#include <string>
#include <vector>

using namespace std;

enum class facing { up, right, down, left };

bool movenext(vector<string>& guardmap, size_t& r, size_t& c, facing& fa) {
    bool goout = false;
    switch (fa) {
    case facing::up:
        while (r != 0 && guardmap[r - 1][c] != '#') {
            guardmap[--r][c] = 'X';
        }
        if (r == 0) {
            goout = true;
        } else {
            fa = facing::right;
        }
        break;
    case facing::right:
        while (c != guardmap.front().size() - 1 && guardmap[r][c + 1] != '#') {
            guardmap[r][++c] = 'X';
        }
        if (c == guardmap.front().size() - 1) {
            goout = true;
        } else {
            fa = facing::down;
        }
        break;
    case facing::down:
        while (r != guardmap.size() - 1 && guardmap[r + 1][c] != '#') {
            guardmap[++r][c] = 'X';
        }
        if (r == guardmap.size() - 1) {
            goout = true;
        } else {
            fa = facing::left;
        }
        break;
    case facing::left:
        while (c != 0 && guardmap[r][c - 1] != '#') {
            guardmap[r][--c] = 'X';
        }
        if (c == 0) {
            goout = true;
        } else {
            fa = facing::up;
        }
        break;
    }
    return goout;
}

void part1() {
    ifstream input("input/input-6");

    vector<string> guardmap;

    for (string line; getline(input, line);) {
        guardmap.push_back(line);
    }

    size_t r = 0;
    size_t c = 0;

    for (; r < guardmap.size(); ++r) {
        if ((c = guardmap[r].find('^')) != string::npos) {
            break;
        }
    }

    // start position
    auto start_r = r;
    auto start_c = c;

    facing fa = facing::up;
    while (!movenext(guardmap, r, c, fa)) {}

    int res = 0;
    for (size_t i = 0; i < guardmap.size(); ++i) {
        res += count(guardmap[i].begin(), guardmap[i].end(), 'X');
    }

    if (guardmap[start_r][start_c] != 'X') {
        ++res;
    }
    println("{}", res);
}

bool movenext_2(vector<string>& guardmap, size_t& r, size_t& c, facing& fa) {
    bool goout = false;
    switch (fa) {
    case facing::up:
        while (r != 0 && guardmap[r - 1][c] != '#') {
            --r;
        }
        if (r == 0) {
            goout = true;
        } else {
            fa = facing::right;
        }
        break;
    case facing::right:
        while (c != guardmap.front().size() - 1 && guardmap[r][c + 1] != '#') {
            ++c;
        }
        if (c == guardmap.front().size() - 1) {
            goout = true;
        } else {
            fa = facing::down;
        }
        break;
    case facing::down:
        while (r != guardmap.size() - 1 && guardmap[r + 1][c] != '#') {
            ++r;
        }
        if (r == guardmap.size() - 1) {
            goout = true;
        } else {
            fa = facing::left;
        }
        break;
    case facing::left:
        while (c != 0 && guardmap[r][c - 1] != '#') {
            --c;
        }
        if (c == 0) {
            goout = true;
        } else {
            fa = facing::up;
        }
        break;
    }
    return goout;
}

void part2() {
    ifstream input("input/input-6");

    vector<string> guardmap;

    for (string line; getline(input, line);) {
        guardmap.push_back(line);
    }

    // start position
    size_t start_r = 0;
    size_t start_c = 0;

    for (; start_r < guardmap.size(); ++start_r) {
        if ((start_c = guardmap[start_r].find('^')) != string::npos) {
            break;
        }
    }

    int res = 0;
    for (size_t i = 0; i < guardmap.size(); ++i) {
        for (size_t j = 0; j < guardmap.front().size(); ++j) {
            if ((i == start_r && j == start_c) || guardmap[i][j] == '#') {
                continue;
            }

            guardmap[i][j] = '#';

            set<tuple<size_t, size_t, facing>> state{{start_r, start_c, facing::up}};
            auto r    = start_r;
            auto c    = start_c;
            facing fa = facing::up;

            bool loop = false;
            while (!movenext_2(guardmap, r, c, fa)) {
                if (!state.contains({r, c, fa})) {
                    state.insert({r, c, fa});
                } else {
                    loop = true;
                    break;
                }
            }

            if (loop) {
                ++res;
            }

            guardmap[i][j] = '.';
        }
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
