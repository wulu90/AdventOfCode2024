#include <fstream>
#include <map>
#include <print>
#include <queue>

using namespace std;

int digits_count(int64_t n) {
    int dc = 0;
    while (n / 10) {
        n /= 10;
        ++dc;
    }
    return dc + 1;
}

pair<int64_t, int64_t> splitnum(int64_t n) {
    int dc          = digits_count(n);
    int64_t divider = 1;
    for (int i = 0; i < dc / 2; ++i) {
        divider *= 10;
    }
    return {n / divider, n % divider};
}

void part1() {
    fstream input("input/input-11");
    queue<int64_t> stones;
    for (int64_t n; input >> n;) {
        stones.push(n);
    }

    for (int i = 0; i < 25; ++i) {
        auto qsize = stones.size();
        for (size_t j = 0; j < qsize; ++j) {
            auto n = stones.front();

            if (n == 0) {
                stones.push(1);
            } else if (digits_count(n) % 2 == 0) {
                auto [a, b] = splitnum(n);
                stones.push(a);
                stones.push(b);
            } else {
                stones.push(n * 2024);
            }
            stones.pop();
        }
    }
    println("{}", stones.size());
}

void part2() {
    fstream input("input/input-11");
    map<int64_t, int64_t> stonenum_count;
    for (int64_t n; input >> n;) {
        stonenum_count[n] += 1;
    }

    for (int i = 0; i < 75; ++i) {
        map<int64_t, int64_t> tmpmap;
        for (auto [n, count] : stonenum_count) {
            if (n == 0) {
                tmpmap[1] += count;
            } else if (digits_count(n) % 2 == 0) {
                auto [a, b] = splitnum(n);
                tmpmap[a] += count;
                tmpmap[b] += count;
            } else {
                tmpmap[n * 2024] += count;
            }
        }
        stonenum_count = tmpmap;
    }
    println("{}", stonenum_count.size());
    int64_t res = 0;
    for (auto [n, count] : stonenum_count) {
        res += count;
    }
    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
