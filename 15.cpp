#include <cstddef>
#include <fstream>
#include <functional>
#include <iterator>
#include <print>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input("input/input-15");
    string line;
    vector<string> warehousemap;
    while (getline(input, line) && !line.empty()) {
        warehousemap.push_back(line);
    }

    vector<char> moves;
    while (getline(input, line)) {
        for (auto c : line) {
            moves.push_back(c);
        }
    }

    size_t i      = 0;
    size_t j      = 0;
    size_t rownum = warehousemap.size();
    size_t colnum = warehousemap.front().size();

    for (; i < rownum; ++i) {
        if ((j = warehousemap[i].find('@')) != string::npos) {
            break;
        }
    }
    warehousemap[i][j] = '.';
    for (auto c : moves) {
        if (c == '^') {
            if (warehousemap[i - 1][j] == '#') {
                continue;
            } else if (warehousemap[i - 1][j] == '.') {
                i -= 1;
            } else {
                auto boxstart = i - 1;
                while (warehousemap[boxstart][j] == 'O') {
                    --boxstart;
                }
                if (warehousemap[boxstart][j] == '#') {
                    continue;
                } else {
                    warehousemap[boxstart][j] = 'O';
                    warehousemap[i - 1][j]    = '.';

                    i -= 1;
                }
            }
        } else if (c == '>') {
            if (warehousemap[i][j + 1] == '#') {
                continue;
            } else if (warehousemap[i][j + 1] == '.') {
                j += 1;
            } else {
                auto boxstart = j + 1;
                while (warehousemap[i][boxstart] == 'O') {
                    ++boxstart;
                }
                if (warehousemap[i][boxstart] == '#') {
                    continue;
                } else {
                    warehousemap[i][boxstart] = 'O';
                    warehousemap[i][j + 1]    = '.';
                    j += 1;
                }
            }
        } else if (c == 'v') {
            if (warehousemap[i + 1][j] == '#') {
                continue;
            } else if (warehousemap[i + 1][j] == '.') {
                i += 1;
            } else {
                auto boxstart = i + 1;
                while (warehousemap[boxstart][j] == 'O') {
                    ++boxstart;
                }
                if (warehousemap[boxstart][j] == '#') {
                    continue;
                } else {
                    warehousemap[boxstart][j] = 'O';
                    warehousemap[i + 1][j]    = '.';
                    i += 1;
                }
            }
        } else {
            if (warehousemap[i][j - 1] == '#') {
                continue;
            } else if (warehousemap[i][j - 1] == '.') {
                j -= 1;
            } else {
                auto boxstart = j - 1;
                while (warehousemap[i][boxstart] == 'O') {
                    --boxstart;
                }
                if (warehousemap[i][boxstart] == '#') {
                    continue;
                } else {
                    warehousemap[i][boxstart] = 'O';
                    warehousemap[i][j - 1]    = '.';
                    j -= 1;
                }
            }
        }
    }
    int64_t gpssum = 0;
    for (size_t r = 0; r < rownum; ++r) {
        for (size_t c = 0; c < colnum; ++c) {
            if (warehousemap[r][c] == 'O') {
                gpssum += 100 * r + c;
            }
        }
    }
    println("{}", gpssum);
}

void find_square_brackets(vector<string>& warehousemap, size_t& r, size_t c, function<size_t(size_t, size_t)> next_rc,
                          function<size_t(size_t, size_t)> back_rc) {
    size_t rr = next_rc(r, 1);
    // set<pair<size_t, size_t>> boxes_move;
    vector<vector<size_t>> boxes_move;
    vector<size_t> nextrowbox;
    if (warehousemap[next_rc(r, 1)][c] == '[') {
        nextrowbox.push_back(c);
    } else {
        nextrowbox.push_back(c - 1);
    }
    boxes_move.push_back(nextrowbox);

    auto check = [&warehousemap, &next_rc](const vector<size_t>& curr, size_t rr) -> bool {
        bool allcanmove = true;
        for (auto box : curr) {
            if (warehousemap[next_rc(rr, 1)][box] == '#' || warehousemap[next_rc(rr, 1)][box + 1] == '#') {
                allcanmove = false;
            }
        }
        return allcanmove;
    };
    bool canmove = true;
    while ((canmove = check(*boxes_move.crbegin(), rr))) {
        nextrowbox.clear();
        for (auto box : *boxes_move.crbegin()) {
            if (warehousemap[next_rc(rr, 1)][box] == '[') {
                nextrowbox.push_back(box);
            }
            if (warehousemap[next_rc(rr, 1)][box] == ']') {
                nextrowbox.push_back(box - 1);
            }

            if (warehousemap[next_rc(rr, 1)][box + 1] == '[') {
                nextrowbox.push_back(box + 1);
            }
        }
        if (!nextrowbox.empty()) {
            boxes_move.push_back(nextrowbox);
            rr = next_rc(rr, 1);
        } else {
            break;
        }
    }
    if (canmove) {
        // rr = next_rc(r, 1);
        reverse(boxes_move.begin(), boxes_move.end());
        for (auto& boxrow : boxes_move) {
            for (auto box : boxrow) {
                warehousemap[rr][box]                 = '.';
                warehousemap[rr][box + 1]             = '.';
                warehousemap[next_rc(rr, 1)][box]     = '[';
                warehousemap[next_rc(rr, 1)][box + 1] = ']';
            }
            rr = back_rc(rr, 1);
        }
        r = next_rc(r, 1);
    }
}

void part2() {
    ifstream input("input/input-15");
    string line;
    vector<string> warehousemap;
    while (getline(input, line) && !line.empty()) {
        string widerline;
        widerline.reserve(line.size() * 2);
        for (auto c : line) {
            if (c == '#') {
                widerline.append("##");
            } else if (c == '.') {
                widerline.append("..");
            } else if (c == 'O') {
                widerline.append("[]");
            } else {
                widerline.append("@.");
            }
        }
        warehousemap.push_back(widerline);
    }

    vector<char> moves;
    while (getline(input, line)) {
        for (auto c : line) {
            moves.push_back(c);
        }
    }

    size_t i      = 0;
    size_t j      = 0;
    size_t rownum = warehousemap.size();
    size_t colnum = warehousemap.front().size();

    for (; i < rownum; ++i) {
        if ((j = warehousemap[i].find('@')) != string::npos) {
            break;
        }
    }
    warehousemap[i][j] = '.';
    for (auto c : moves) {
        if (c == '^') {
            warehousemap[i][j] = '.';
            if (warehousemap[i - 1][j] == '#') {
                continue;
            } else if (warehousemap[i - 1][j] == '.') {
                i -= 1;
            } else {
                find_square_brackets(warehousemap, i, j, minus<size_t>(), plus<size_t>());
            }
            warehousemap[i][j] = '@';
        } else if (c == '>') {
            warehousemap[i][j] = '.';

            if (warehousemap[i][j + 1] == '#') {
                continue;
            } else if (warehousemap[i][j + 1] == '.') {
                j += 1;
            } else {
                auto boxstart = j + 1;
                while (warehousemap[i][boxstart] == '[' || warehousemap[i][boxstart] == ']') {
                    ++boxstart;
                }
                if (warehousemap[i][boxstart] == '#') {
                    continue;
                } else {
                    while (boxstart > j + 1) {
                        warehousemap[i][boxstart]     = ']';
                        warehousemap[i][boxstart - 1] = '[';
                        boxstart -= 2;
                    }
                    warehousemap[i][j + 1] = '.';
                    j += 1;
                }
            }
            warehousemap[i][j] = '@';

        } else if (c == 'v') {
            warehousemap[i][j] = '.';

            if (warehousemap[i + 1][j] == '#') {
                continue;
            } else if (warehousemap[i + 1][j] == '.') {
                i += 1;
            } else {
                find_square_brackets(warehousemap, i, j, plus<size_t>(), minus<size_t>());
            }
            warehousemap[i][j] = '@';

        } else {
            warehousemap[i][j] = '.';

            if (warehousemap[i][j - 1] == '#') {
                continue;
            } else if (warehousemap[i][j - 1] == '.') {
                j -= 1;
            } else {
                auto boxstart = j - 1;
                while (warehousemap[i][boxstart] == '[' || warehousemap[i][boxstart] == ']') {
                    --boxstart;
                }
                if (warehousemap[i][boxstart] == '#') {
                    continue;
                } else {
                    while (boxstart < j - 1) {
                        warehousemap[i][boxstart]     = '[';
                        warehousemap[i][boxstart + 1] = ']';
                        boxstart += 2;
                    }
                    warehousemap[i][j - 1] = '.';
                    j -= 1;
                }
            }
            warehousemap[i][j] = '@';
        }
    }
    int64_t gpssum = 0;
    for (size_t r = 0; r < rownum; ++r) {
        for (size_t c = 0; c < colnum; ++c) {
            if (warehousemap[r][c] == '[') {
                gpssum += 100 * r + c;
            }
        }
    }
    println("{}", gpssum);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
