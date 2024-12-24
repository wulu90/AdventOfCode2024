#include <cstdint>
#include <fstream>
#include <functional>
#include <map>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct gate_wires {
    string wire_input_0;
    string wire_input_1;
    string wire_output;
    function<uint8_t(uint8_t, uint8_t)> gate;
};

function<uint8_t(uint8_t, uint8_t)> parse_gate(const string_view& str) {
    function<uint8_t(uint8_t, uint8_t)> func;
    if (str == "AND") {
        func = bit_and<uint8_t>();
    } else if (str == "OR") {
        func = bit_or<>();
    } else if (str == "XOR") {
        func = bit_xor<>();
    }
    return func;
}

void part1() {
    ifstream input{"input/input-24"};
    map<string, uint8_t> inputmap;
    for (string line; getline(input, line) && !line.empty();) {
        auto colonpos                                   = line.find(':');
        inputmap[{line.data(), line.data() + colonpos}] = *line.crbegin() - '0';
    }

    vector<gate_wires> gate_wires_vec;
    set<string> unknowset;
    for (string line; getline(input, line);) {
        gate_wires gw;

        auto splitview  = line | ranges::views::split(" "sv);
        auto it         = splitview.cbegin();
        gw.wire_input_0 = string_view{*it};
        gw.gate         = parse_gate(string_view{*++it});
        gw.wire_input_1 = string_view{*++it};
        ++it;
        gw.wire_output = string_view{*++it};
        gate_wires_vec.push_back(gw);
        unknowset.insert(gw.wire_input_0);
        unknowset.insert(gw.wire_input_1);
        unknowset.insert(gw.wire_output);
    }
    input.close();

    for (const auto& [s, _] : inputmap) {
        unknowset.erase(s);
    }

    while (!unknowset.empty()) {
        for (const auto& gw : gate_wires_vec) {
            if (inputmap.contains(gw.wire_input_0) && inputmap.contains(gw.wire_input_1) && !inputmap.contains(gw.wire_output)) {
                inputmap[gw.wire_output] = gw.gate(inputmap[gw.wire_input_0], inputmap[gw.wire_input_1]);
                unknowset.erase(gw.wire_output);
            }
        }
    }

    uint64_t binarybase = 1;
    uint64_t res        = 0;
    for (auto it = inputmap.find("z00"s); it != inputmap.end(); ++it) {
        res += it->second * binarybase;
        binarybase *= 2;
    }
    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
