#include <charconv>
#include <format>
#include <fstream>
#include <print>
#include <set>
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

void writebitmap_pbm(const vector<string>& imagevec, int index) {
    string imagename = format("out/bitmaps/{:05d}.pbm", index);
    ofstream output(imagename);
    output << "P1\n101 103\n";
    for (auto& s : imagevec) {
        output << s;
    }
    output.close();
}

void part2() {
    ifstream input("input/input-14");
    vector<robot> robotvec;
    for (string line; getline(input, line);) {
        robotvec.push_back(parse_robot(line));
    }

    int row     = 103;
    int col     = 101;
    int seconds = 1;
    // while (true) {
    while (seconds <= row * col) {
        set<pair<int, int>> xymap;
        vector<string> imagevec(103, string(101, '0'));
        for (auto r : robotvec) {
            r.px = (r.px + r.vx * seconds) % col < 0 ? (r.px + r.vx * seconds) % col + col : (r.px + r.vx * seconds) % col;
            r.py = (r.py + r.vy * seconds) % row < 0 ? (r.py + r.vy * seconds) % row + row : (r.py + r.vy * seconds) % row;

            xymap.insert({r.px, r.py});
            imagevec[r.py][r.px] = '1';
        }

        writebitmap_pbm(imagevec, seconds);
        ++seconds;
        // find a triangle  fail
        // find 10 robots in a row
        /*
       bool find = true;
       for (auto [x, y] : xymap) {
           find = true;
           for (int i = 1; i <= 10; ++i) {
               if (!xymap.contains({x, y + i})) {
                   find = false;
                   break;
               }
           }
           if (find) {
               break;
           }
       }
       if (find) {
           println("{}", seconds);
           break;
       }

       // every tile has only one robot, why? see reddit
       if (xymap.size() == robotvec.size()) {
           println("{}", seconds);
           vector<string> robotout(103, string(101, '.'));
           for (auto [x, y] : xymap) {
               robotout[x][y] = '$';
           }

           for (auto& line : robotout) {
               println("{}", line);
           }

           break;
       }
       ++seconds;
       */
    }
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
