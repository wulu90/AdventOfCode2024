#include <charconv>
#include <fstream>
#include <print>
#include <string>
#include <vector>

using namespace std;

struct robot {
    int px;
    int py;
    int vx;
    int vy;
};

robot parse_robot(const string& line) {
    robot r;
    auto comma = line.find(',');
    from_chars(line.data() + 2, line.data() + comma, r.px);
    auto spacepos = line.find(' ');
    from_chars(line.data() + comma + 1, line.data() + spacepos, r.py);
    comma = line.find(',', spacepos);
    from_chars(line.data() + spacepos + 3, line.data() + comma, r.vx);
    from_chars(line.data() + comma + 1, line.data() + line.length(), r.vy);
    return r;
}

void part1() {
    ifstream input("input/input-14");
    vector<robot> robotvec;
    for (string line; getline(input, line);) {
        robotvec.push_back(parse_robot(line));
    }

    int row     = 103;
    int col     = 101;
    int seconds = 100;

    int num_tl = 0;
    int num_tr = 0;
    int num_bl = 0;
    int num_br = 0;

    for (auto r : robotvec) {
        r.px = (r.px + r.vx * 100) % col < 0 ? (r.px + r.vx * 100) % col + col : (r.px + r.vx * 100) % col;
        r.py = (r.py + r.vy * 100) % row < 0 ? (r.py + r.vy * 100) % row + row : (r.py + r.vy * 100) % row;

        if (r.px < col / 2 && r.py < row / 2) {
            ++num_tl;
        } else if (r.px > col / 2 && r.py < row / 2) {
            ++num_tr;
        } else if (r.px < col / 2 && r.py > row / 2) {
            ++num_bl;
        } else if (r.px > col / 2 && r.py > row / 2) {
            ++num_br;
        }
    }

    println("{}", num_tl * num_tr * num_bl * num_br);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
