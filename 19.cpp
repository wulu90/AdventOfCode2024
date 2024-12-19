#include <fstream>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

bool make_design(const string& design, size_t index, const multimap<char, string>& ch_str_map) {
    if (index == design.length()) {
        return true;
    }

    auto patts = ch_str_map.equal_range(design[index]);
    if (patts.first == ch_str_map.end()) {
        return false;
    }
    bool canmake = false;
    for (auto it = patts.first; it != patts.second; ++it) {
        if (design.find(it->second, index) == index) {
            if (make_design(design, index + it->second.length(), ch_str_map)) {
                canmake = true;
                break;
            }
        }
    }
    return canmake;
}

void part1() {
    ifstream input("input/input-19");
    string line;
    getline(input, line);
    vector<string> patterns = line | views::split(", "sv) |
                              views::transform([](auto&& subr) -> string { return string{subr.begin(), subr.end()}; }) |
                              ranges::to<vector>();
    getline(input, line);    // empty line

    vector<string> designs;
    while (getline(input, line)) {
        designs.push_back(line);
    }
    input.close();

    multimap<char, string> ch_str_map;
    for (auto& str : patterns) {
        ch_str_map.insert({str[0], str});
    }

    int count = 0;
    for (auto& design : designs) {
        if (make_design(design, 0, ch_str_map)) {
            ++count;
        }
    }

    println("{}", count);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
