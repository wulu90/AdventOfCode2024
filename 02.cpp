#include <algorithm>
#include <charconv>
#include <cstddef>
#include <format>
#include <fstream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using namespace std::ranges::views;

vector<int> parse_str(const string_view& sv) {
    auto res = sv | split(" "sv) | views::transform([](auto&& subr) -> int {
                   string_view subsv(subr.begin(), subr.end());
                   int num = 0;
                   from_chars(subsv.data(), subsv.data() + subsv.size(), num);
                   return num;
               }) |
               ranges::to<vector>();

    return res;
}

bool level_safe(const vector<int>& vec) {
    vector<int> diffs;
    diffs.reserve(vec.size());

    adjacent_difference(vec.begin(), vec.end(), back_inserter(diffs));

    auto it = adjacent_find(next(diffs.begin()), diffs.end(), [](const int num1, const int num2) {
        if (abs(num1) > 3 || num1 == 0 || abs(num2) > 3 || num2 == 0 || num1 * num2 < 0)
            return true;
        return false;
    });
    return it == diffs.end() ? true : false;
}

bool level_safe_part2(const vector<int>& vec) {
    if (level_safe(vec))
        return true;

    vector<int> diffs;
    adjacent_difference(vec.begin(), vec.end(), back_inserter(diffs));

    bool find_poss = false;

    for (size_t i = 0; i < vec.size(); ++i) {
        vector<int> difftmp;
        if (i == 0) {
            copy_n(next(diffs.begin(), 2), vec.size() - 2, back_inserter(difftmp));
        } else if (i == vec.size() - 1) {
            copy_n(next(diffs.begin()), vec.size() - 2, back_inserter(difftmp));
        } else {
            copy_n(next(diffs.begin()), i - 1, back_inserter(difftmp));
            difftmp.push_back(diffs[i] + diffs[i + 1]);
            copy_n(next(diffs.begin(), i + 2), vec.size() - 2 - i, back_inserter(difftmp));
        }

        auto it = adjacent_find(difftmp.begin(), difftmp.end(), [](const int num1, const int num2) {
            if (abs(num1) > 3 || num1 == 0 || abs(num2) > 3 || num2 == 0 || num1 * num2 < 0)
                return true;
            return false;
        });
        if (it == difftmp.end()) {
            find_poss = true;
            break;
        }
    }
    return find_poss;
}

int main(int argc, char* argv[]) {
    ifstream input("input/input-2");

    string line;
    int count  = 0;
    int count2 = 0;
    while (getline(input, line)) {
        auto vec = parse_str(line);
        if (level_safe(vec)) {
            ++count;
        }
        if (level_safe_part2(vec)) {
            ++count2;
        }
    }

    println("{}", count);
    println("{}", count2);
}
