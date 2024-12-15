#include <fstream>
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

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
