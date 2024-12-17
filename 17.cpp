#include <array>
#include <charconv>
#include <fmt/base.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

void part1() {
    ifstream input("input/input-17");
    string line;
    array<int, 3> regs;
    for (size_t i = 0; i < 3; ++i) {
        getline(input, line);
        auto colonpos = line.find(':');
        from_chars(line.data() + colonpos + 2, line.data() + line.length(), regs[i]);
    }
    getline(input, line);    // empty line;
    getline(input, line);

    auto colonpos        = line.find(':');
    vector<int> programs = string_view{line.begin() + colonpos + 2, line.end()} | views::split(',') |
                           views::transform([](auto subr) -> int {
                               string_view subsv(subr.begin(), subr.end());
                               int num = 0;
                               from_chars(subsv.data(), subsv.data() + subsv.size(), num);
                               return num;
                           }) |
                           ranges::to<vector>();

    size_t instrction_pointer = 0;
    vector<int> outputs;
    while (true) {
        if (instrction_pointer >= programs.size()) {
            break;
        }

        auto opcode = programs[instrction_pointer];
        auto oprand = programs[instrction_pointer + 1];

        if (opcode == 3 && regs[0] != 0) {    // jnp
            instrction_pointer = oprand;
            continue;
        }
        if (opcode == 0) {    // adv
            oprand  = oprand < 4 ? oprand : regs[oprand - 4];
            regs[0] = regs[0] / (1 << oprand);
        } else if (opcode == 1) {    // bxl
            regs[1] = regs[1] ^ oprand;
        } else if (opcode == 2) {    // bst
            oprand  = oprand < 4 ? oprand : regs[oprand - 4];
            regs[1] = oprand % 8;
        } else if (opcode == 4) {    // bxc
            regs[1] = regs[1] ^ regs[2];
        } else if (opcode == 5) {    // out
            oprand = oprand < 4 ? oprand : regs[oprand - 4];
            outputs.push_back(oprand % 8);
        } else if (opcode == 6) {    // bdv
            oprand  = oprand < 4 ? oprand : regs[oprand - 4];
            regs[1] = regs[0] / (1 << oprand);
            regs[1] = regs[0] >> (oprand + 1);
        } else if (opcode == 7) {    // cdv
            oprand  = oprand < 4 ? oprand : regs[oprand - 4];
            regs[2] = regs[0] / (1 << oprand);
        }
        instrction_pointer += 2;
    }

    fmt::println("{}", fmt::join(outputs, ","));
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
