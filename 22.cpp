#include <fstream>
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

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
