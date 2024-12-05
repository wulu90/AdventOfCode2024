#include <algorithm>
#include <charconv>
#include <cstdio>
#include <fstream>
#include <map>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

bool check_order(const map<int, set<int>>& rules_r, const vector<int>& update) {
    for (size_t i = 0; i < update.size() - 1; ++i) {
        if (!rules_r.contains(update[i])) {
            continue;
        }
        auto before_cur = rules_r.at(update[i]);
        for (size_t j = i + 1; j < update.size(); ++j) {
            if (before_cur.contains(update[j])) {
                return false;
            }
        }
    }
    return true;
}

void fix_order(const map<int, set<int>>& rules, vector<int>& update) {
    sort(update.begin(), update.end(), [&rules](auto num1, auto num2) -> bool {
        if (rules.contains(num1) && rules.at(num1).contains(num2)) {
            return true;
        }
        return false;
    });
}

void part1() {
    ifstream input("input/input-5");

    map<int, set<int>> rules;
    map<int, set<int>> rules_r;
    vector<vector<int>> updatevec;
    for (string line; getline(input, line);) {
        if (line.contains('|')) {
            auto sep = line.find('|');
            int num1 = 0;
            from_chars(line.data(), line.data() + sep, num1);
            int num2 = 0;
            from_chars(line.data() + sep + 1, line.data() + line.size(), num2);
            rules[num1].insert(num2);
            rules_r[num2].insert(num1);
        } else if (line.contains(',')) {
            auto update = line | ranges::views::split(',') | views::transform([](auto&& subr) -> int {
                              string_view sv{subr.begin(), subr.end()};
                              int num = 0;
                              from_chars(sv.data(), sv.data() + sv.size(), num);
                              return num;
                          }) |
                          ranges::to<vector<int>>();
            updatevec.push_back(update);
        }
    }

    int res  = 0;
    int res2 = 0;
    for (auto& update : updatevec) {
        if (check_order(rules_r, update)) {
            // println("{}", update[update.size() / 2]);
            res += update[update.size() / 2];
        } else {
            fix_order(rules, update);
            res2 += update[update.size() / 2];
        }
    }
    println("{}", res);
    println("{}", res2);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
