#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream input("input/input-1");

    int num;
    vector<int> leftside;
    vector<int> rightside;
    map<int, int> num_times;

    while (input >> num) {
        leftside.push_back(num);
        input >> num;
        rightside.push_back(num);

        num_times[num] += 1;
    }
    sort(leftside.begin(), leftside.end());
    sort(rightside.begin(), rightside.end());

    int res = 0;
    for (size_t i = 0; i < leftside.size(); ++i) {
        res += abs(leftside[i] - rightside[i]);
    }

    cout << res << endl;

    res = 0;
    for (size_t i = 0; i < leftside.size(); ++i) {
        res += num_times.contains(leftside[i]) ? leftside[i] * num_times[leftside[i]] : 0;
    }
    cout << res << endl;
}
