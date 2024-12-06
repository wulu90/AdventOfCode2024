#include <cstddef>
#include <fstream>
#include <print>
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

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
