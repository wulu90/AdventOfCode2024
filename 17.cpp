#include <array>
#include <cerrno>
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

vector<size_t> run(const vector<size_t>& programs, array<size_t, 3> regs) {
    size_t instrction_pointer = 0;
    vector<size_t> outputs;
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

    return outputs;
}

pair<vector<size_t>, array<size_t, 3>> parse_input() {
    ifstream input("input/input-17");
    string line;
    array<size_t, 3> regs;
    for (size_t i = 0; i < 3; ++i) {
        getline(input, line);
        auto colonpos = line.find(':');
        from_chars(line.data() + colonpos + 2, line.data() + line.length(), regs[i]);
    }
    getline(input, line);    // empty line;
    getline(input, line);

    auto colonpos           = line.find(':');
    vector<size_t> programs = string_view{line.begin() + colonpos + 2, line.end()} | views::split(',') |
                              views::transform([](auto subr) -> size_t {
                                  string_view subsv(subr.begin(), subr.end());
                                  int num = 0;
                                  from_chars(subsv.data(), subsv.data() + subsv.size(), num);
                                  return num;
                              }) |
                              ranges::to<vector>();

    input.close();
    return {programs, regs};
}

void part1() {
    auto [programs, regs] = parse_input();
    fmt::println("{}", fmt::join(run(programs, regs), ","));
}

vector<size_t> run2() {
    vector<size_t> outputs;
    size_t r0 = 136904920099226;
    size_t r1 = 0;
    size_t r2 = 0;

    while (true) {
        r1 = r0 % 8;
        r1 ^= 5;
        // r2 = r0 / (1ul << r1);
        r2 = r0 >> r1;
        r1 ^= 6;
        // r0 /= 8;
        r0 >>= 3;
        r1 ^= r2;
        outputs.push_back(r1 % 8);
        if (r0 == 0)
            break;
    }
    fmt::println("{}", fmt::join(outputs, ","));
    return outputs;
}

size_t run3(size_t r0) {
    size_t r1 = r0 % 8;
    r1 ^= 5;
    size_t r2 = r0 >> r1;
    r1 ^= 6;
    r0 >>= 3;
    r1 ^= r2;
    return r1 % 8;
}

void dfs(size_t index, const vector<size_t>& programs, vector<size_t>& nums, size_t curr) {
    for (size_t i = 0; i < 8; ++i) {
        if (run3(curr * 8 + i) == programs[15 - index]) {
            if (index == 15) {
                nums.push_back(curr * 8 + i);
                return;
            }
            dfs(index + 1, programs, nums, curr * 8 + i);
        }
    }
}

void part2() {
    auto [programs, orig] = parse_input();
    // vector<size_t> programs{2,4,1,5,7,5,1,6,0,3,4,6,5,5,3,0};
    vector<size_t> nums;
    dfs(0, programs, nums, 0);

    for (auto n : nums) {
        println("{}", n);
    }
}

int main(int argc, char* argv[]) {
    part1();
    run2();
    part2();
    return 0;
}
