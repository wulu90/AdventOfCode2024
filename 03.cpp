#include <fstream>
#include <iterator>
#include <map>
#include <ostream>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
using namespace std;

void part1() {
    ifstream input("input/input-3");
    stringstream iss;
    iss << input.rdbuf();
    string line = iss.str();
    regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

    int64_t res = 0;

    for (sregex_iterator it(line.begin(), line.end(), pattern); it != sregex_iterator(); ++it) {
        smatch match = *it;
        res += stoi(match[1].str()) * stoi(match[2].str());
    }

    println("{}", res);
}

bool check_mul(const map<unsigned long, bool>& indexmap, unsigned long inx) {
    auto it = indexmap.upper_bound(inx);

    if (it == indexmap.begin()) {
        return true;
    }

    return (--it)->second;
}

void part2() {
    ifstream input("input/input-3");
    /*
    while (getline(input, tmpline)) {
        line += tmpline;
        }
    */
    stringstream iss;
    iss << input.rdbuf();
    string line = iss.str();
    regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    regex pattern_do(R"(do\(\))");
    regex pattern_dont(R"(don't\(\))");
    int64_t res = 0;

    map<unsigned long, bool> indexmap;

    for (sregex_iterator it(line.begin(), line.end(), pattern_do); it != sregex_iterator(); ++it) {
        indexmap.insert({it->position(), true});
    }

    for (sregex_iterator it(line.begin(), line.end(), pattern_dont); it != sregex_iterator(); ++it) {
        indexmap.insert({it->position(), false});
    }

    for (sregex_iterator it(line.begin(), line.end(), pattern); it != sregex_iterator(); ++it) {
        if (check_mul(indexmap, it->position())) {
            smatch match = *it;
            res += stoi(match[1].str()) * stoi(match[2].str());
        }
    }

    println("{}", res);
}

void part2_1() {
    ifstream input("input/input-3");
    stringstream iss;
    iss << input.rdbuf();
    string line = iss.str();
    regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    int64_t res = 0;
    string tmpline;

    size_t start = 0;

    while (start != string::npos) {
        auto nextdont   = line.find(R"(don't())", start);
        auto matchbegin = line.begin() + start;
        auto matchend   = nextdont == string::npos ? line.end() : line.begin() + nextdont;
        for (sregex_iterator it(matchbegin, matchend, pattern); it != sregex_iterator(); ++it) {
            smatch match = *it;
            res += stoi(match[1].str()) * stoi(match[2].str());
        }
        start = line.find(R"(do())", nextdont);
    }

    println("{}", res);
}

void part2_2() {
    ifstream input("input/input-3");
    string line{istreambuf_iterator<char>(input), istreambuf_iterator<char>()};
    regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
    regex pattern_do(R"(do\(\))");
    regex pattern_dont(R"(don't\(\))");
    vector<pair<size_t, variant<bool, int>>> index_value;

    for (sregex_iterator it(line.begin(), line.end(), pattern_do); it != sregex_iterator(); ++it) {
        index_value.push_back({it->position(), true});
    }

    for (sregex_iterator it(line.begin(), line.end(), pattern_dont); it != sregex_iterator(); ++it) {
        index_value.push_back({it->position(), false});
    }

    for (sregex_iterator it(line.begin(), line.end(), pattern); it != sregex_iterator(); ++it) {
        smatch match = *it;
        index_value.push_back({it->position(), stoi(match[1].str()) * stoi(match[2].str())});
    }

    sort(index_value.begin(), index_value.end());

    bool enable = true;
    int res     = 0;
    for (auto [_, va] : index_value) {
        if (va.index() == 0) {
            if (get<bool>(va)) {
                enable = true;
            } else {
                enable = false;
            }
        } else {
            if (enable) {
                res += get<int>(va);
            }
        }
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    part2_1();
    part2_2();
}
