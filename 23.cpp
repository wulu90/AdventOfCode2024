#include <algorithm>
#include <fstream>
#include <map>
#include <numeric>
#include <print>
#include <set>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream input{"input/input-23"};
    multimap<string, string> adjmap;
    set<string> computers;
    for (string line; getline(input, line);) {
        auto dashpos = line.find('-');
        string str1{line.data(), line.data() + dashpos};
        string str2{line.data() + dashpos + 1, line.data() + line.length()};

        adjmap.insert({str1, str2});
        adjmap.insert({str2, str1});
        computers.insert(str1);
        computers.insert(str2);
    }
    input.close();

    map<string, size_t> computer_idx;
    map<size_t, string> idx_computer;

    size_t idx = 0;
    for (auto& str : computers) {
        computer_idx[str] = idx;
        idx_computer[idx] = str;
        ++idx;
    }

    vector<vector<int8_t>> mat(computers.size(), vector<int8_t>(computers.size(), 0));
    for (const auto& [s1, s2] : adjmap) {
        mat[computer_idx[s1]][computer_idx[s2]] = 1;
    }
    set<set<size_t>> conn_sets;
    for (size_t i = 0; i < computers.size(); ++i) {
        vector<size_t> adjidx;
        for (size_t j = 0; j < computers.size(); ++j) {
            if (mat[i][j] == 1) {
                adjidx.push_back(j);
            }
        }

        for (size_t m = 0; m < adjidx.size() - 1; ++m) {
            for (size_t n = m + 1; n < adjidx.size(); ++n) {
                if (mat[adjidx[m]][adjidx[n]] == 1) {
                    conn_sets.insert({i, adjidx[m], adjidx[n]});
                }
            }
        }
    }
    int res = 0;
    for (const auto& conn : conn_sets) {
        auto it   = conn.begin();
        auto str1 = idx_computer[*it];
        auto str2 = idx_computer[*++it];
        auto str3 = idx_computer[*++it];
        if (str1[0] == 't' || str2[0] == 't' || str3[0] == 't') {
            ++res;
        }
    }
    println("{}", res);
}

// https://iq.opengenus.org/bron-kerbosch-algorithm/
void bron_kerbosch(vector<size_t> r, vector<size_t> p, vector<size_t> x, const vector<vector<int8_t>>& mat,
                   vector<vector<size_t>>& result) {
    if (p.empty() && x.empty()) {
        result.push_back(r);
        return;
    }

    for (auto it = p.begin(); it != p.end();) {
        auto rnew = r;
        rnew.push_back(*it);
        vector<size_t> nei;
        for (size_t i = 0; i < mat.size(); ++i) {
            if (mat[*it][i] == 1) {
                nei.push_back(i);
            }
        }
        vector<size_t> pnew;
        set_intersection(p.begin(), p.end(), nei.begin(), nei.end(), back_inserter(pnew));
        vector<size_t> xnew;
        set_intersection(x.begin(), x.end(), nei.begin(), nei.end(), back_inserter(xnew));
        bron_kerbosch(rnew, pnew, xnew, mat, result);

        x.push_back(*it);
        it = p.erase(it);
    }
}

void part2() {
    ifstream input{"input/input-23"};
    multimap<string, string> adjmap;
    set<string> computers;
    for (string line; getline(input, line);) {
        auto dashpos = line.find('-');
        string str1{line.data(), line.data() + dashpos};
        string str2{line.data() + dashpos + 1, line.data() + line.length()};

        adjmap.insert({str1, str2});
        adjmap.insert({str2, str1});
        computers.insert(str1);
        computers.insert(str2);
    }
    input.close();

    map<string, size_t> computer_idx;
    map<size_t, string> idx_computer;

    size_t idx = 0;
    for (auto& str : computers) {
        computer_idx[str] = idx;
        idx_computer[idx] = str;
        ++idx;
    }

    vector<vector<int8_t>> mat(computers.size(), vector<int8_t>(computers.size(), 0));
    for (const auto& [s1, s2] : adjmap) {
        mat[computer_idx[s1]][computer_idx[s2]] = 1;
    }

    vector<vector<size_t>> result;
    vector<size_t> p(computers.size(), 0);
    iota(p.begin(), p.end(), 0);
    bron_kerbosch({}, p, {}, mat, result);

    auto maxcliques = max_element(result.begin(), result.end(), [](const auto& a, const auto& b) { return a.size() < b.size(); });
    vector<string> cliquesvec;
    for (auto i : *maxcliques) {
        cliquesvec.push_back(idx_computer[i]);
    }

    sort(cliquesvec.begin(), cliquesvec.end());

    for_each_n(cliquesvec.begin(), cliquesvec.size() - 1, [](const auto& str) { print("{},", str); });
    println("{}", *cliquesvec.crbegin());
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
