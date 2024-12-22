#include <array>
#include <fstream>
#include <map>
#include <print>
#include <vector>

using namespace std;

uint64_t generate_secret(uint64_t initn, size_t idx) {
    uint64_t secnum = initn;
    for (size_t i = 1; i <= idx; ++i) {
        secnum = ((secnum * 64) ^ secnum) % 16777216ul;
        secnum = ((secnum / 32) ^ secnum) % 16777216ul;
        secnum = ((secnum * 2048) ^ secnum) % 16777216ul;
    }
    return secnum;
}

void part1() {
    ifstream input("input/input-22");
    vector<uint64_t> init_num_vec;
    for (uint64_t n; input >> n;) {
        init_num_vec.push_back(n);
    }

    uint64_t sum = 0;
    for (auto initn : init_num_vec) {
        sum += generate_secret(initn, 2000);
    }
    println("{}", sum);
}

void prices_changes(uint64_t initn, array<int8_t, 2001>& prices_arr, array<int8_t, 2000>& changes_arr) {
    uint64_t secnum = initn;
    prices_arr[0]   = secnum % 10;
    for (size_t i = 0; i < 2000; ++i) {
        secnum            = ((secnum * 64) ^ secnum) % 16777216ul;
        secnum            = ((secnum / 32) ^ secnum) % 16777216ul;
        secnum            = ((secnum * 2048) ^ secnum) % 16777216ul;
        prices_arr[i + 1] = secnum % 10;
        changes_arr[i]    = prices_arr[i + 1] - prices_arr[i];
    }
}

map<tuple<int8_t, int8_t, int8_t, int8_t>, int8_t> generate_seq4_price_map(const array<int8_t, 2001>& prices_arr,
                                                                           const array<int8_t, 2000>& changes_arr) {
    map<tuple<int8_t, int8_t, int8_t, int8_t>, int8_t> seq4_price_map;
    for (size_t i = 1999; i >= 3; --i) {
        seq4_price_map[{changes_arr[i - 3], changes_arr[i - 2], changes_arr[i - 1], changes_arr[i]}] = prices_arr[i + 1];
    }
    return seq4_price_map;
}

void part2() {
    ifstream input("input/input-22");
    vector<uint64_t> init_num_vec;
    for (uint64_t n; input >> n;) {
        init_num_vec.push_back(n);
    }

    vector<map<tuple<int8_t, int8_t, int8_t, int8_t>, int8_t>> seq4_price_map_vec;
    for (auto initn : init_num_vec) {
        array<int8_t, 2001> prices;
        array<int8_t, 2000> changes;
        prices_changes(initn, prices, changes);
        seq4_price_map_vec.push_back(generate_seq4_price_map(prices, changes));
    }
    map<tuple<int8_t, int8_t, int8_t, int8_t>, int> seq4_price_sum_map;
    for (const auto& seq4map : seq4_price_map_vec) {
        for (const auto& [tup, price] : seq4map) {
            seq4_price_sum_map[tup] += price;
        }
    }

    int bananas = 0;
    for (const auto& [tup, pricesum] : seq4_price_sum_map) {
        bananas = max(bananas, pricesum);
    }
    println("{}", bananas);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
