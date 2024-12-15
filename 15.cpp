#include <fstream>
#include <functional>
#include <print>
#include <set>
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

void move_up_down(vector<string>& warehousemap, size_t& r, size_t c, function<size_t(size_t, size_t)> next_rc,
                  function<size_t(size_t, size_t)> back_rc) {
    vector<set<size_t>> boxes_to_move;
    set<size_t> next_row_boxes;
    if (warehousemap[next_rc(r, 1)][c] == '[') {
        next_row_boxes.insert(c);
    } else {
        next_row_boxes.insert(c - 1);
    }
    boxes_to_move.push_back(next_row_boxes);

    auto check = [&warehousemap, &next_rc](const set<size_t>& curr_row_boxes, size_t rr) -> bool {
        bool allcanmove = true;
        for (auto box_col : curr_row_boxes) {
            if (warehousemap[next_rc(rr, 1)][box_col] == '#' || warehousemap[next_rc(rr, 1)][box_col + 1] == '#') {
                allcanmove = false;
                break;
            }
        }
        return allcanmove;
    };
    bool canmove = true;
    size_t rr    = next_rc(r, 1);

    while ((canmove = check(*boxes_to_move.crbegin(), rr))) {
        next_row_boxes.clear();
        for (auto box_col : *boxes_to_move.crbegin()) {
            if (warehousemap[next_rc(rr, 1)][box_col] == '[') {
                next_row_boxes.insert(box_col);
            }
            if (warehousemap[next_rc(rr, 1)][box_col] == ']') {
                next_row_boxes.insert(box_col - 1);
            }

            if (warehousemap[next_rc(rr, 1)][box_col + 1] == '[') {
                next_row_boxes.insert(box_col + 1);
            }
        }
        if (!next_row_boxes.empty()) {
            boxes_to_move.push_back(next_row_boxes);
            rr = next_rc(rr, 1);
        } else {
            break;
        }
    }
    if (canmove) {
        reverse(boxes_to_move.begin(), boxes_to_move.end());
        for (auto& boxrow : boxes_to_move) {
            for (auto box_col : boxrow) {
                warehousemap[rr][box_col]                 = '.';
                warehousemap[rr][box_col + 1]             = '.';
                warehousemap[next_rc(rr, 1)][box_col]     = '[';
                warehousemap[next_rc(rr, 1)][box_col + 1] = ']';
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
            if (warehousemap[i - 1][j] == '#') {
                continue;
            } else if (warehousemap[i - 1][j] == '.') {
                i -= 1;
            } else {
                move_up_down(warehousemap, i, j, minus<size_t>(), plus<size_t>());
            }
        } else if (c == '>') {
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
        } else if (c == 'v') {
            if (warehousemap[i + 1][j] == '#') {
                continue;
            } else if (warehousemap[i + 1][j] == '.') {
                i += 1;
            } else {
                move_up_down(warehousemap, i, j, plus<size_t>(), minus<size_t>());
            }

        } else {
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
