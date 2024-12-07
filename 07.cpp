#include <charconv>
#include <deque>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using namespace std;

struct equation {
    int64_t testval;
    vector<int64_t> nums;
};

equation parse_line(const string& line) {
    equation equ;

    auto colon_pos = line.find(':');
    from_chars(line.data(), line.data() + colon_pos, equ.testval);

    string_view sv{line.begin() + colon_pos + 2, line.end()};

    equ.nums = sv | ranges::views::split(' ') | ranges::views::transform([](auto&& subr) {
                   string_view subsv{subr.begin(), subr.end()};
                   int64_t n = 0;
                   from_chars(subsv.data(), subsv.data() + subsv.length(), n);
                   return n;
               }) |
               ranges::to<vector>();
    return equ;
}

bool produce(const equation& equ) {
    deque<int64_t> q;
    q.push_back(equ.nums[0] + equ.nums[1]);
    q.push_back(equ.nums[0] * equ.nums[1]);
    for (size_t i = 2; i < equ.nums.size(); ++i) {
        auto qsize = q.size();
        for (size_t j = 0; j < qsize; ++j) {
            auto prev = q.front();
            q.push_back(prev + equ.nums[i]);
            q.push_back(prev * equ.nums[i]);
            q.pop_front();
        }
    }

    bool res = false;
    while (!q.empty()) {
        if (q.front() == equ.testval) {
            res = true;
            break;
        } else {
            q.pop_front();
        }
    }
    return res;
}

int64_t concat(int64_t n1, int64_t n2) {
    int64_t tmp = n2;
    while (tmp / 10) {
        tmp /= 10;
        n1 *= 10;
    }

    return n1 * 10 + n2;
}

bool produce_2(const equation& equ) {
    deque<int64_t> q;
    q.push_back(equ.nums[0] + equ.nums[1]);
    q.push_back(equ.nums[0] * equ.nums[1]);
    q.push_back(concat(equ.nums[0], equ.nums[1]));
    for (size_t i = 2; i < equ.nums.size(); ++i) {
        auto qsize = q.size();
        for (size_t j = 0; j < qsize; ++j) {
            auto prev = q.front();
            q.push_back(prev + equ.nums[i]);
            q.push_back(prev * equ.nums[i]);
            q.push_back(concat(prev, equ.nums[i]));
            q.pop_front();
        }
    }

    bool res = false;
    while (!q.empty()) {
        if (q.front() == equ.testval) {
            res = true;
            break;
        } else {
            q.pop_front();
        }
    }
    return res;
}

void part1() {
    ifstream input("input/input-7");

    int64_t res  = 0;
    int64_t res2 = 0;
    for (string line; getline(input, line);) {
        auto equ = parse_line(line);
        if (produce(equ)) {
            res += equ.testval;
        }
        if (produce_2(equ)) {
            res2 += equ.testval;
        }
    }
    println("{}", res);
    println("{}", res2);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
