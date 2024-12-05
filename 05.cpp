#include <algorithm>
#include <charconv>
#include <cstddef>
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

bool check_order(const map<int, set<int>>& rules, const vector<int>& update) {
    for (auto i = update.rbegin(); i != next(update.rend(), -1); ++i) {
        if (!rules.contains(*i)) {
            continue;
        }
        auto greater_curr = rules.at(*i);
        for (auto j = next(i); j != update.rend(); ++j) {
            if (greater_curr.contains(*j)) {
                return false;
            }
        }
    }
    return true;
}

bool check_order_1(const map<int, set<int>>& rules, const vector<int>& update) {
    return is_sorted(update.begin(), update.end(), [&rules](auto num1, auto num2) -> bool {
        if (rules.contains(num1) && rules.at(num1).contains(num2)) {
            return true;
        }
        return false;
    });
}

void fix_order(const map<int, set<int>>& rules, vector<int>& update) {
    sort(update.begin(), update.end(), [&rules](auto num1, auto num2) -> bool {
        if (rules.contains(num1) && rules.at(num1).contains(num2)) {
            return true;
        }
        return false;
    });
}

void fix_order_1(const map<int, set<int>>& rules, vector<int>& update) {
    for (size_t i = update.size() - 1; i > 1; --i) {
        for (size_t j = update.size() - 1; j > update.size() - 1 - i; --j) {
            if (rules.contains(update[j]) && rules.at(update[j]).contains(update[j - 1])) {
                swap(update[j], update[j - 1]);
            }
        }
    }
}

void part1() {
    ifstream input("input/input-5");

    map<int, set<int>> rules;
    vector<vector<int>> updatevec;
    for (string line; getline(input, line);) {
        if (line.contains('|')) {
            auto sep = line.find('|');
            int num1 = 0;
            from_chars(line.data(), line.data() + sep, num1);
            int num2 = 0;
            from_chars(line.data() + sep + 1, line.data() + line.size(), num2);
            rules[num1].insert(num2);
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
        if (check_order_1(rules, update)) {
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
