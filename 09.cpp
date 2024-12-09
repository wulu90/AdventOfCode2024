#include <fstream>
#include <print>
#include <vector>

using namespace std;

struct layout {
    int fileid;
    int length;
};

void part1() {
    ifstream input("input/input-9");

    vector<layout> diskmap;
    string line;
    getline(input, line);
    bool freespace = false;
    int fileid         = 0;

    for (auto c : line) {
        int len = c - '0';
        if (len == 0) {
            freespace = false;
            continue;
        }

        if (!freespace) {
            diskmap.push_back({fileid, len});
            ++fileid;
            freespace = true;
        } else {
            diskmap.push_back({-1, len});
            freespace = false;
        }
    }

    size_t i = 0;
    size_t j = diskmap.size() - 1;

    vector<layout> disknew;
    while (i < j) {
        if (diskmap[i].fileid != -1) {
            disknew.push_back(diskmap[i]);
            ++i;
            continue;
        }

        if (diskmap[j].fileid == -1) {
            --j;
            continue;
        }

        if (diskmap[i].length > diskmap[j].length) {    // freespace > tail file length
            disknew.push_back(diskmap[j]);
            diskmap[i].length -= diskmap[j].length;
            --j;
        } else if (diskmap[i].length < diskmap[j].length) {
            disknew.push_back({diskmap[j].fileid, diskmap[i].length});
            diskmap[j].length -= diskmap[i].length;
            ++i;
        } else {
            disknew.push_back(diskmap[j]);
            ++i;
            --j;
        }
    }

    if (diskmap[j].fileid != -1 && diskmap[j].length > 0)
        disknew.push_back(diskmap[j]);

    int64_t res = 0;
    int blockid          = 0;
    for (auto [fileid, len] : disknew) {
        while (len > 0) {
            res += fileid * blockid;
            ++blockid;
            --len;
        }
    }

    println("{}", res);
}

void part2() {
    ifstream input("input/input-9");

    vector<layout> diskmap;
    string line;
    getline(input, line);
    bool freespace = false;
    int id         = 0;

    for (auto c : line) {
        int len = c - '0';
        if (len == 0) {
            freespace = false;
            continue;
        }

        if (!freespace) {
            diskmap.push_back({id, len});
            ++id;
            freespace = true;
        } else {
            diskmap.push_back({-1, len});
            freespace = false;
        }
    }

    // auto disknew = diskmap;

    auto j = diskmap.size() - 1;

    while (j != 0) {
        if (diskmap[j].fileid == -1) {
            --j;
            continue;
        }

        bool find_enough_space = false;
        int needsize           = diskmap[j].length;
        size_t i               = 0;
        for (i = 0; i < j; ++i) {
            if (diskmap[i].fileid == -1 && diskmap[i].length >= needsize) {
                find_enough_space = true;
                break;
            }
        }

        if (!find_enough_space) {
            --j;
            continue;
        }

        if (diskmap[i].length == needsize) {
            diskmap[i].fileid = diskmap[j].fileid;
            diskmap[j].fileid = -1;
            --j;
        } else {
            diskmap[i].length -= needsize;
            auto tmp          = diskmap[j];
            diskmap[j].fileid = -1;
            diskmap.insert(diskmap.cbegin() + i, tmp);
        }
    }
    int64_t res = 0;
    int blockid = 0;
    for (auto [fileid, len] : diskmap) {
        while (len > 0) {
            res += blockid * (fileid == -1 ? 0 : fileid);
            --len;
            ++blockid;
        }
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
