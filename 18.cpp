#include <charconv>
#include <fstream>
#include <map>
#include <print>
#include <queue>
#include <set>
#include <string>
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

bool exitable(const vector<pair<int, int>>& fallbytes, size_t endindex, const vector<array<int, 2>>& adjs) {
    set<pair<int, int>> fallbytesset;
    fallbytesset.insert(fallbytes.begin(), fallbytes.begin() + endindex);

    set<pair<int, int>> visited;
    queue<pair<int, int>> q;
    q.push({0, 0});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (auto [i, j] : adjs) {
            if (x + i >= 0 && x + i <= 70 && y + j >= 0 && y + j <= 70 && !fallbytesset.contains({x + i, y + j}) &&
                !visited.contains({x + i, y + j})) {
                visited.insert({x + i, y + j});
                q.push({x + i, y + j});
            }
        }
    }

    return visited.contains({70, 70});
}

void part2() {
    ifstream input("input/input-18");
    string line;
    vector<pair<int, int>> fallbytes;
    int n1 = 0;
    int n2 = 0;
    while (getline(input, line)) {
        auto commapos = line.find(',');
        from_chars(line.data(), line.data() + commapos, n1);
        from_chars(line.data() + commapos + 1, line.data() + line.length(), n2);
        fallbytes.push_back({n1, n2});
    }

    vector<array<int, 2>> adjs{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    size_t left  = 0;
    size_t right = fallbytes.size();

    while (left < right) {
        auto endindex = (left + right) / 2;

        if (exitable(fallbytes, endindex, adjs)) {
            left = endindex + 1;
        } else {
            right = endindex;
        }
    }

    println("{},{}", fallbytes[left - 1].first, fallbytes[left - 1].second);

    for (size_t i = 0; i < fallbytes.size(); ++i) {
        if (!exitable(fallbytes, i, adjs)) {
            println("{},{}", fallbytes[i - 1].first, fallbytes[i - 1].second);
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
