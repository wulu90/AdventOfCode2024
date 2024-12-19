#include <algorithm>
#include <fstream>
#include <map>
#include <print>
#include <ranges>
#include <string>
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

void make_design_count(const string& design, map<size_t, size_t> index_count, const multimap<char, string>& ch_str_map, size_t& num) {
    if (all_of(index_count.begin(), index_count.end(), [&design](auto it) { return it.first >= design.length(); })) {
        num = index_count[design.length()];
        return;
    }
    map<size_t, size_t> tmpmap;
    for (auto [index, count] : index_count) {
        if (index >= design.length()) {
            tmpmap[index] += count;
            continue;
        }

        auto patts = ch_str_map.equal_range(design[index]);
        if (patts.first == ch_str_map.end()) {
            continue;
        }
        for (auto it = patts.first; it != patts.second; ++it) {
            if (design.find(it->second, index) == index) {
                tmpmap[index + it->second.length()] += count;    // only first count can be zero,so just set count=1,not count==0?1:count
            }
        }
    }
    if (!tmpmap.empty()) {
        make_design_count(design, tmpmap, ch_str_map, num);
    }
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

    int count     = 0;
    size_t count2 = 0;
    for (auto& design : designs) {
        if (make_design(design, 0, ch_str_map)) {
            ++count;
        }

        size_t n = 0;
        make_design_count(design, {{0, 1}}, ch_str_map, n);
        count2 += n;
    }

    println("{}", count);
    println("{}", count2);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
