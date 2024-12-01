#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  ifstream input("input-1");

  int num;
  vector<int> leftside;
  vector<int> rightside;
  map<int, int> num_times;

  while (input >> num) {
    // input >> num;
    //  cout << num << endl;
    leftside.push_back(num);
    input >> num;
    // cout << num << endl;
    rightside.push_back(num);

    num_times[num] += 1;
  }
  sort(leftside.begin(), leftside.end());
  sort(rightside.begin(), rightside.end());

  int64_t res = 0;
  for (int i = 0; i < leftside.size(); ++i) {
    res += abs(leftside[i] - rightside[i]);
  }

  cout << res << endl;

  res = 0;
  for (size_t i = 0; i < leftside.size(); ++i) {

    res += leftside[i] *
           (num_times.contains(leftside[i]) ? num_times[leftside[i]] : 0);
  }
  cout << res << endl;
}
