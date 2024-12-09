#include <chrono>
#include <cstddef>
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
    int fileid     = 0;

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
    int blockid = 0;
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

struct layoutnode {
    int fileid;
    int length;
    layoutnode* prev;
    layoutnode* next;
};

void part2_list() {
    ifstream input("input/input-9");
    // ifstream input("input-test");

    vector<layout> diskmap;
    string line;
    getline(input, line);
    bool freespace = false;
    int fileid     = 0;

    layoutnode* head = new layoutnode;
    head->fileid     = fileid;
    head->length     = line[0] - '0';
    head->prev       = nullptr;
    head->next       = nullptr;
    layoutnode* tail = head;

    ++fileid;
    freespace = true;

    for (size_t i = 1; i < line.size(); ++i) {
        int len = line[i] - '0';
        if (len == 0) {
            freespace = false;
            continue;
        }

        layoutnode* node = new layoutnode;
        node->length     = len;
        node->prev       = tail;
        tail->next       = node;
        if (!freespace) {
            node->fileid = fileid;
            ++fileid;
            freespace = true;
        } else {
            node->fileid = -1;
            freespace    = false;
        }
        tail = node;
    }

    layoutnode* curr = tail;

    while (curr != head) {
        if (curr->fileid == -1) {
            curr = curr->prev;
            continue;
        }

        bool find_enouth_space = false;
        int needsize           = curr->length;
        layoutnode* findnode   = head;
        while (findnode != curr) {
            if (findnode->fileid == -1 && findnode->length >= needsize) {
                find_enouth_space = true;
                break;
            }
            findnode = findnode->next;
        }

        if (!find_enouth_space) {
            curr = curr->prev;
            continue;
        }

        if (findnode->length == needsize) {
            findnode->fileid = curr->fileid;
            curr->fileid     = -1;
            curr             = curr->prev;
        } else {
            layoutnode* tmp = new layoutnode;
            findnode->length -= curr->length;
            tmp->fileid          = curr->fileid;
            tmp->length          = curr->length;
            tmp->prev            = findnode->prev;
            findnode->prev->next = tmp;
            tmp->next            = findnode;
            findnode->prev       = tmp;
            curr->fileid         = -1;
            curr                 = curr->prev;
        }
    }

    curr = head;

    int64_t res = 0;
    int blockid = 0;

    while (curr != nullptr) {
        auto len = curr->length;
        while (len > 0) {
            res += blockid * (curr->fileid == -1 ? 0 : curr->fileid);
            --len;
            ++blockid;
        }
        curr = curr->next;
    }

    println("{}", res);
}

int main(int argc, char* argv[]) {
    part1();
    auto t1 = chrono::high_resolution_clock::now();
    part2();
    auto t2 = chrono::high_resolution_clock::now();
    part2_list(); // compiler is faster than you 😅
    auto t3 = chrono::high_resolution_clock::now();
    println("{}", chrono::duration_cast<chrono::milliseconds>(t2 - t1));
    println("{}", chrono::duration_cast<chrono::milliseconds>(t3 - t2));
    return 0;
}
