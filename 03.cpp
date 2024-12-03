#include <cstdint>
#include <fstream>
#include <print>
#include <regex>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    fstream input("input/input-3");
    string line;
    regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

    int64_t res = 0;

    while (getline(input, line)) {
        for (sregex_iterator it(line.begin(), line.end(), pattern); it != sregex_iterator(); ++it) {
            smatch match = *it;
            res += stoi(match[1].str()) * stoi(match[2].str());
        }
    }

    println("{}", res);
}
