#include <algorithm>
#include <charconv>
#include <fstream>
#include <print>
#include <string>
#include <vector>

using namespace std;

struct claw {
    int xa;
    int ya;
    int xb;
    int yb;
    int xprize;
    int yprize;
};

void parse_xy(const string& line, int& x, int& y) {
    auto pluspos = line.find('+');
    auto comma   = line.find(',');
    from_chars(line.data() + pluspos + 1, line.data() + comma, x);
    pluspos = line.find('+', comma);
    from_chars(line.data() + pluspos + 1, line.data() + line.length(), y);
}

void parse_prize(const string& line, int& xp, int& yp) {
    auto equalpos = line.find('=');
    auto comma    = line.find(',');
    from_chars(line.data() + equalpos + 1, line.data() + comma, xp);
    equalpos = line.find('=', comma);
    from_chars(line.data() + equalpos + 1, line.data() + line.length(), yp);
}

void part1() {
    ifstream input("input/input-13");
    vector<claw> clawvec;
    for (string line; getline(input, line);) {
        if (!line.empty()) {
            claw cl;
            parse_xy(line, cl.xa, cl.ya);
            getline(input, line);
            parse_xy(line, cl.xb, cl.yb);
            getline(input, line);
            parse_prize(line, cl.xprize, cl.yprize);
            clawvec.push_back(cl);
        }
    }
    input.close();

    int res = 0;
    for (auto cl : clawvec) {
        vector<int> tokenvec;
        for (int a = 0; a < 100; ++a) {
            for (int b = 0; b < 100; ++b) {
                if (a * cl.xa + b * cl.xb == cl.xprize && a * cl.ya + b * cl.yb == cl.yprize) {
                    tokenvec.push_back(a * 3 + b);
                }
            }
        }
        if (!tokenvec.empty()) {
            res += *min_element(tokenvec.begin(), tokenvec.end());
        }
    }
    println("{}", res);
}

void part2() {
    ifstream input("input/input-13");
    vector<claw> clawvec;
    for (string line; getline(input, line);) {
        if (!line.empty()) {
            claw cl;
            parse_xy(line, cl.xa, cl.ya);
            getline(input, line);
            parse_xy(line, cl.xb, cl.yb);
            getline(input, line);
            parse_prize(line, cl.xprize, cl.yprize);
            clawvec.push_back(cl);
        }
    }
    input.close();

    /**
     * xa*a+xb*b=xp; ya*a+yb*b=yp; a,b
     * ①*ya ②*xa
     * xa*ya*a+xb*ya*b=xp*ya
     * xa*ya*a+yb*xa*b=yp*xa
     * b=(yp*xa-xp*ya)/(yb*xa-xb*ya)
     * ①*yb ②*xb
     * xa*yb*a+xb*yb*b=xp*yb
     * ya*xb*a+xb*yb*b=yp*xb
     * a=(xp*yb-yp*xb)/(xa*yb-ya*xb)
     **/

    int64_t off{10000000000000};
    int64_t res = 0;
    for (auto cl : clawvec) {
        int64_t divisor = cl.xa * cl.yb - cl.xb * cl.ya;
        if (divisor == 0) {
            continue;
        }
        int64_t a_dividend = (cl.xprize + off) * cl.yb - (cl.yprize + off) * cl.xb;
        int64_t b_dividend = (cl.yprize + off) * cl.xa - (cl.xprize + off) * cl.ya;

        if (a_dividend % divisor == 0 && b_dividend % divisor == 0) {
            res += 3 * a_dividend / divisor + b_dividend / divisor;
        }
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
