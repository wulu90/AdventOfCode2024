#include <algorithm>
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

enum class gate { AND, OR, XOR };

struct gate_wires {
    string wire_input_0;
    string wire_input_1;
    string wire_output;
    gate gate;
};

gate parse_gate(const string_view& str) {
    if (str == "AND"sv) {
        return gate::AND;
    } else if (str == "OR") {
        return gate::OR;
    } else {
        return gate::XOR;
    }
}

function<uint8_t(uint8_t, uint8_t)> gatefunc(const gate& g) {
    switch (g) {
    case gate::AND:
        return bit_and<uint8_t>();
    case gate::OR:
        return bit_or<uint8_t>();
    default:
        return bit_xor<uint8_t>();
    }
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
        ++it;    // ->
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
                inputmap[gw.wire_output] = gatefunc(gw.gate)(inputmap[gw.wire_input_0], inputmap[gw.wire_input_1]);
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

string find_output(const string& x, const string& y, const gate& gate, const vector<gate_wires>& gate_wires_vec) {
    string str;
    for (const auto& gw : gate_wires_vec) {
        if (gw.wire_input_0 == x && gw.wire_input_1 == y && gw.gate == gate) {
            str = gw.wire_output;
        }

        if (gw.wire_input_0 == y && gw.wire_input_1 == x && gw.gate == gate) {
            str = gw.wire_output;
        }
    }
    return str;
}

pair<string, string> full_adder(const string& x, const string& y, const string& c_in, const vector<gate_wires>& gate_wires_vec,
                                vector<string>& swapoutputs) {
    // z: x XOR y XOR c_in
    // c_out: (x AND y) OR ((x XOR y) AND c_in)
    // m = x XOR y
    // n = x AND y
    // z = m XOR c_in
    // r = m AND c_in
    // c_out = n OR r

    auto m = find_output(x, y, gate::XOR, gate_wires_vec);
    auto n = find_output(x, y, gate::AND, gate_wires_vec);

    if (!c_in.empty()) {
        auto r = find_output(m, c_in, gate::AND, gate_wires_vec);
        if (r.empty()) {
            swap(m, n);
            swapoutputs.push_back(m);
            swapoutputs.push_back(n);
            r = find_output(m, c_in, gate::AND, gate_wires_vec);
        }

        auto z = find_output(m, c_in, gate::XOR, gate_wires_vec);
        if (m.starts_with('z')) {
            swap(m, z);
            swapoutputs.push_back(m);
            swapoutputs.push_back(z);
        }

        if (n.starts_with('z')) {
            swap(n, z);
            swapoutputs.push_back(z);
            swapoutputs.push_back(n);
        }

        if (r.starts_with('z')) {
            swap(r, z);
            swapoutputs.push_back(r);
            swapoutputs.push_back(z);
        }

        auto c_out = find_output(n, r, gate::OR, gate_wires_vec);
        if (c_out.starts_with('z') && c_out != "z45"s) {    // last carry out
            swap(c_out, z);
            swapoutputs.push_back(c_out);
            swapoutputs.push_back(z);
        }
        if (c_out.empty()) {    // ?
            c_out = m;
        }

        return {z, c_out};
    } else {
        return {m, n};
    }
}

void part2() {
    ifstream input{"input/input-24"};
    for (string line; getline(input, line) && !line.empty();) {}

    vector<gate_wires> gate_wires_vec;
    for (string line; getline(input, line);) {
        gate_wires gw;

        auto splitview  = line | ranges::views::split(" "sv);
        auto it         = splitview.cbegin();
        gw.wire_input_0 = string_view{*it};
        gw.gate         = parse_gate(string_view{*++it});
        gw.wire_input_1 = string_view{*++it};
        ++it;    // ->
        gw.wire_output = string_view{*++it};
        gate_wires_vec.push_back(gw);
    }
    input.close();

    // https://en.wikipedia.org/wiki/Adder_(electronics)#Full_adder
    // https://github.com/xhyrom/aoc/blob/main/2024/24/part_2.py
    vector<string> swapoutputs;
    string x, y, c;
    for (int i = 0; i < 45; ++i) {
        x = format("x{:02}", i);
        y = format("y{:02}", i);

        auto [z, c_out] = full_adder(x, y, c, gate_wires_vec, swapoutputs);
        c               = c_out;
    }
    sort(swapoutputs.begin(), swapoutputs.end());
    for_each_n(swapoutputs.begin(), swapoutputs.size() - 1, [](const auto& s) { return print("{},", s); });
    println("{}", *swapoutputs.crbegin());
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
