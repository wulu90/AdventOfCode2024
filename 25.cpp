#include <array>
#include <fstream>
#include <print>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream input{"input/input-25"};

    vector<array<uint8_t, 5>> locks;
    vector<array<uint8_t, 5>> keys;

    for (string line; getline(input, line);) {
        if (line.starts_with('#')) {
            array<uint8_t, 5> lock{0, 0, 0, 0, 0};
            while (getline(input, line) && !line.empty()) {
                for (size_t i = 0; i < 5; ++i) {
                    if (line[i] == '#') {
                        ++lock[i];
                    }
                }
            }
            locks.push_back(lock);
        } else if (line.starts_with('.')) {
            array<uint8_t, 5> key{5, 5, 5, 5, 5};
            while (getline(input, line) && !line.empty()) {
                for (size_t i = 0; i < 5; ++i) {
                    if (line[i] == '.') {
                        --key[i];
                    }
                }
            }
            keys.push_back(key);
        }
    }

    input.close();

    uint res = 0;

    for (size_t i = 0; i < locks.size(); ++i) {
        for (size_t j = 0; j < keys.size(); ++j) {
            bool fit = true;
            for (size_t k = 0; k < 5; ++k) {
                if (locks[i][k] + keys[j][k] > 5) {
                    fit = false;
                    break;
                }
            }
            if (fit) {
                ++res;
            }
        }
    }
    println("{}", res);
    return 0;
}
