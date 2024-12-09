#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <print>
#include <vector>

using namespace std;

struct ff {
    int fileid;
    int length;
};

void part1() {
    ifstream input("input/input-9");

    vector<ff> ffvec;

    int id         = 0;
    bool freespace = false;
    while (!input.eof()) {
        int len = input.get() - '0';
        if (len == -38 || len == -49)
            break;
        if (len == 0) {
            freespace = false;
            continue;
        }
        if (!freespace) {
            ffvec.push_back({id, len});
            ++id;
            freespace = true;
        } else {
            ffvec.push_back({-1, len});
            freespace = false;
        }
    }

    size_t i = 0;
    size_t j = ffvec.size() - 1;

    vector<ff> re;
    while (i < j) {
        if (ffvec[i].fileid != -1) {
            re.push_back(ffvec[i]);
            ++i;
            continue;
        }

        if (ffvec[j].fileid == -1) {
            --j;
            continue;
        }

        if (ffvec[i].length > ffvec[j].length) {    // freespace > tail file length
            re.push_back(ffvec[j]);
            ffvec[i].length -= ffvec[j].length;
            --j;
        } else if (ffvec[i].length < ffvec[j].length) {
            re.push_back({ffvec[j].fileid, ffvec[i].length});
            ffvec[j].length -= ffvec[i].length;
            ++i;
        } else {
            re.push_back(ffvec[j]);
            ++i;
            --j;
        }
    }

    if(ffvec[j].fileid!=-1&&ffvec[j].length>0)
        re.push_back(ffvec[j]);

    int64_t res = 0;
    id          = 0;
    for (auto [fileid, len] : re) {
        while (len > 0) {
            res += fileid * id;
            ++id;
            --len;
        }
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
