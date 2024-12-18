#include <charconv>
#include <fstream>
#include <functional>
#include <map>
#include <print>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

void part1() {
    ifstream input("input/input-18");
    string line;
    set<pair<int, int>> fallbytes;
    for (int i = 0; i < 1024; ++i) {
        getline(input, line);
        int n1        = 0;
        int n2        = 0;
        auto commapos = line.find(',');
        from_chars(line.data(), line.data() + commapos, n1);
        from_chars(line.data() + commapos + 1, line.data() + line.length(), n2);
        fallbytes.insert({n1, n2});
    }

    vector<array<int, 2>> adjs{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    set<pair<int, int>> visited;
    map<pair<int, int>, int> costmap;
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> pq;
    pq.push({0, {0, 0}});
    costmap[{0, 0}] = 0;

    while (!pq.empty()) {
        auto [cost, curr] = pq.top();
        if (curr == make_pair(70, 70)) {
            println("{}", cost);
            break;
        }
        pq.pop();

        if (visited.contains(curr)) {
            continue;
        }

        visited.insert(curr);
        auto [x, y] = curr;
        for (auto [i, j] : adjs) {
            if (x + i >= 0 && x + i <= 70 && y + j >= 0 && y + j <= 70 && !fallbytes.contains({x + i, y + j})) {
                if (!costmap.contains({x + i, y + j})) {
                    costmap[{x + i, y + j}] = costmap[curr] + 1;
                    pq.push({costmap[{x + i, y + j}], {x + i, y + j}});
                } else {
                    if (costmap[{x + i, y + j}] > costmap[curr] + 1) {
                        costmap[{x + i, y + j}] = costmap[curr] + 1;
                        pq.push({costmap[{x + i, y + j}], {x + i, y + j}});
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
