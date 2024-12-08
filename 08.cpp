#include <cctype>
#include <cstddef>
#include <fstream>
#include <map>
#include <print>
#include <set>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input("input/input-8");

    vector<string> antennas;

    for (string line; getline(input, line);) {
        antennas.push_back(line);
    }

    map<char, vector<pair<size_t, size_t>>> frequency_coords;

    for (size_t i = 0; i < antennas.size(); ++i) {
        for (size_t j = 0; j < antennas.front().size(); ++j) {
            if (isalnum(antennas[i][j])) {
                frequency_coords[antennas[i][j]].push_back({i, j});
            }
        }
    }
    set<pair<size_t, size_t>> unique_coord;
    for (const auto& [c, coordvec] : frequency_coords) {
        for (size_t i = 0; i < coordvec.size() - 1; ++i) {
            for (size_t j = i + 1; j < coordvec.size(); ++j) {
                auto& [r1, c1] = coordvec[i];
                auto& [r2, c2] = coordvec[j];

                int ra1 = static_cast<int>(r1) + (static_cast<int>(r1) - static_cast<int>(r2));
                int ca1 = static_cast<int>(c1) + (static_cast<int>(c1) - static_cast<int>(c2));
                int ra2 = static_cast<int>(r2) + (static_cast<int>(r2) - static_cast<int>(r1));
                int ca2 = static_cast<int>(c2) + (static_cast<int>(c2) - static_cast<int>(c1));

                if (ra1 >= 0 && ra1 < antennas.size() && ca1 >= 0 && ca1 < antennas.front().size()) {
                    unique_coord.insert({ra1, ca1});
                }
                if (ra2 >= 0 && ra2 < antennas.size() && ca2 >= 0 && ca2 < antennas.front().size()) {
                    unique_coord.insert({ra2, ca2});
                }
            }
        }
    }

    println("{}", unique_coord.size());
}

void part2() {
    ifstream input("input/input-8");

    vector<string> antennas;

    for (string line; getline(input, line);) {
        antennas.push_back(line);
    }

    map<char, vector<pair<int, int>>> frequency_coords;
    int rownum = antennas.size();
    int colnum = antennas.front().size();
    for (int i = 0; i < rownum; ++i) {
        for (int j = 0; j < colnum; ++j) {
            auto c = antennas[static_cast<size_t>(i)][static_cast<size_t>(j)];
            if (isalnum(c)) {
                frequency_coords[c].push_back({i, j});
            }
        }
    }
    set<pair<int, int>> unique_coord;
    for (const auto& [c, coordvec] : frequency_coords) {
        for (size_t i = 0; i < coordvec.size() - 1; ++i) {
            for (size_t j = i + 1; j < coordvec.size(); ++j) {
                auto& [r1, c1] = coordvec[i];
                auto& [r2, c2] = coordvec[j];

                int rd = r1 - r2;
                int cd = c1 - c2;
                int ra = r1 + rd;
                int ca = c1 + cd;

                while (ra >= 0 && ra < rownum && ca >= 0 && ca < colnum) {
                    unique_coord.insert({ra, ca});
                    ra += rd;
                    ca += cd;
                }

                rd = r2 - r1;
                cd = c2 - c1;
                ra = r2 + rd;
                ca = c2 + cd;

                while (ra >= 0 && ra < rownum && ca >= 0 && ca < colnum) {
                    unique_coord.insert({ra, ca});
                    ra += rd;
                    ca += cd;
                }

                unique_coord.insert({r1, c1});
                unique_coord.insert({r2, c2});
            }
        }
    }

    println("{}", unique_coord.size());
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
