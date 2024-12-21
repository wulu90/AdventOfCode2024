#include <algorithm>
#include <array>
#include <fstream>
#include <map>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std;

const map<char, pair<int, int>> num_keypad{{'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}},
                                           {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}}, {'0', {3, 1}}, {'A', {3, 2}}};

const map<char, pair<int, int>> dir_keypad{{'^', {0, 1}}, {'A', {0, 2}}, {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}}};
const vector<array<int, 2>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const string dirsymbol{"^>v<"};

vector<string> padpath(const char& from, const char& to, const map<char, pair<int, int>>& pad, int rownum, int colnum,
                       pair<int, int> disablecoord) {
    vector<string> res;

    queue<pair<pair<int, int>, string>> q;
    q.push({pad.at(from), {}});

    while (!q.empty()) {
        auto [curr, str] = q.front();

        // find shortest path
        if (curr == pad.at(to)) {
            while (!q.empty()) {
                if (q.front().first == pad.at(to))
                    res.push_back(q.front().second);
                q.pop();
            }
            break;
        }

        q.pop();

        for (size_t i = 0; i < 4; ++i) {
            auto [rd, cd] = dirs[i];
            auto r        = curr.first + rd;
            auto c        = curr.second + cd;
            if (r >= 0 && r < rownum && c >= 0 & c < colnum && make_pair(r, c) != disablecoord) {
                q.push({{r, c}, str + dirsymbol[i]});
            }
        }
    }

    return res;
}

set<string> symbolpath(const string& str_ori, const map<char, pair<int, int>>& pad, int rownum, int colnum, pair<int, int> disablecoord) {
    auto str = 'A' + str_ori;
    vector<vector<string>> pathvec;
    for (size_t i = 0; i < str.length() - 1; ++i) {
        pathvec.push_back(padpath(str[i], str[i + 1], pad, rownum, colnum, disablecoord));
    }
    queue<string> q;
    for (auto& s : pathvec[0]) {
        q.push(s + 'A');
    }

    for (auto& vec : pathvec | ranges::views::drop(1)) {
        auto qsize = q.size();
        for (size_t i = 0; i < qsize; ++i) {
            auto prev = q.front();
            q.pop();
            for (auto& s : vec) {
                q.push(prev + s + 'A');
            }
        }
    }

    set<string> res;
    while (!q.empty()) {
        res.insert(q.front());
        q.pop();
    }

    return res;
}

void part1() {
    ifstream input("input/input-21");

    vector<string> codes;
    for (string line; getline(input, line);) {
        codes.push_back(line);
    }

    uint64_t res = 0;
    for (auto& line : codes) {
        auto numpadpath = symbolpath(line, num_keypad, 4, 3, {3, 0});

        set<string> vec1;
        for (auto& path : numpadpath) {
            for (auto& str : symbolpath(path, dir_keypad, 2, 3, {0, 0})) {
                vec1.insert(str);
            }
        }

        set<string> vec2;
        for (auto& path : vec1) {
            for (auto& str : symbolpath(path, dir_keypad, 2, 3, {0, 0})) {
                vec2.insert(str);
            }
        }

        uint64_t minlen = UINT64_MAX;
        for (auto& path : vec2) {
            minlen = min(path.length(), minlen);
        }
        res += stoi(line) * minlen;
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
