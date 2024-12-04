#include <array>
#include <cstddef>
#include <fstream>
#include <print>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input("input/input-4");
    vector<string> charmap;
    for (string line; getline(input, line);) {
        charmap.push_back(line);
    }
    input.close();
    auto rownum = charmap.size();
    auto colnum = charmap.front().size();
    array<array<array<int, 2>, 4>, 4> dirs{{{{{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
                                            {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
                                            {{{0, 0}, {1, 1}, {2, 2}, {3, 3}}},
                                            {{{0, 0}, {1, -1}, {2, -2}, {3, -3}}}}};

    array<array<char, 4>, 2> xmass{{{{'X', 'M', 'A', 'S'}}, {{'S', 'A', 'M', 'X'}}}};
    int count = 0;
    for (size_t i = 0; i < charmap.size(); ++i) {
        for (size_t j = 0; j < charmap.front().size(); ++j) {
            for (size_t k = 0; k < 4; ++k) {
                auto r0 = i + dirs[k][0][0];
                auto c0 = j + dirs[k][0][1];
                auto r1 = i + dirs[k][1][0];
                auto c1 = j + dirs[k][1][1];
                auto r2 = i + dirs[k][2][0];
                auto c2 = j + dirs[k][2][1];
                auto r3 = i + dirs[k][3][0];
                auto c3 = j + dirs[k][3][1];

                if (r0 < rownum && c0 < colnum && r1 < rownum && c1 < colnum && r2 < rownum && c2 < colnum && r3 < rownum && c3 < colnum) {
                    if (charmap[r0][c0] == xmass[0][0] && charmap[r1][c1] == xmass[0][1] && charmap[r2][c2] == xmass[0][2] &&
                        charmap[r3][c3] == xmass[0][3]) {
                        ++count;
                    }
                    if (charmap[r0][c0] == xmass[1][0] && charmap[r1][c1] == xmass[1][1] && charmap[r2][c2] == xmass[1][2] &&
                        charmap[r3][c3] == xmass[1][3]) {
                        ++count;
                    }
                }
            }
        }
    }

    println("{}", count);
}

void part2() {
    ifstream input("input/input-4");
    vector<string> charmap;
    for (string line; getline(input, line);) {
        charmap.push_back(line);
    }
    input.close();

    // array<array<char, 3>, 2> x_mass{{{{'M', 'A', 'S'}}, {{'S', 'A', 'M'}}}};
    auto rownum = charmap.size();
    auto colnum = charmap.front().size();
    string mas{"MAS"};
    string sam{"SAM"};
    int count = 0;
    for (size_t i = 0; i < charmap.size(); ++i) {
        for (size_t j = 0; j < charmap.front().size(); ++j) {
            // top left
            auto rtl = i;
            auto ctl = j;
            // top right
            auto rtr = i;
            auto ctr = j + 2;
            // center
            auto rcc = i + 1;
            auto ccc = j + 1;
            // bottom left
            auto rbl = i + 2;
            auto cbl = j;
            // bottom right;
            auto rbr = i + 2;
            auto cbr = j + 2;

            if (rtl < rownum && ctl < colnum && rtr < rownum && ctr < colnum && rcc < rownum && ccc < colnum && rbl < rownum &&
                cbl < colnum && rbr < rownum && cbr < colnum) {
                string s1{charmap[rtl][ctl], charmap[rcc][ccc], charmap[rbr][cbr]};
                string s2{charmap[rtr][ctr], charmap[rcc][ccc], charmap[rbl][cbl]};

                if ((s1 == mas || s1 == sam) && (s2 == mas || s2 == sam)) {
                    ++count;
                }
            }
        }
    }

    println("{}", count);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
