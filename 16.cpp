#include <fstream>
#include <limits>
#include <map>
#include <print>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

using node = tuple<size_t, size_t, size_t>;

vector<vector<int>> dirs{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

size_t rotate_clockwise(size_t facing) {
    return facing == 3 ? 0 : facing + 1;
}

size_t rotate_counterclockwise(size_t facing) {
    return facing == 0 ? 3 : facing - 1;
}

vector<pair<node, size_t>> adj(const node& curr, const vector<string>& labyrinth) {
    vector<pair<node, size_t>> adjs;

    auto [r, c, f] = curr;
    adjs.push_back({{r, c, rotate_clockwise(f)}, 1000});
    adjs.push_back({{r, c, rotate_counterclockwise(f)}, 1000});
    if (labyrinth[r + dirs[f][0]][c + dirs[f][1]] != '#') {
        adjs.push_back({{r + dirs[f][0], c + dirs[f][1], f}, 1});
    }
    return adjs;
}

void part1() {
    ifstream input("input/input-16");
    vector<string> labyrinth;
    for (string line; getline(input, line);) {
        labyrinth.push_back(line);
    }

    size_t i_start = 0;
    size_t j_start = 0;
    size_t i_end   = 0;
    size_t j_end   = 0;

    for (size_t i = 0; i < labyrinth.size(); ++i) {
        if (labyrinth[i].contains('S')) {
            i_start = i;
            j_start = labyrinth[i].find('S');
        }
        if (labyrinth[i].contains('E')) {
            i_end = i;
            j_end = labyrinth[i].find('E');
        }
    }
    node node_start{i_start, j_start, 1};

    priority_queue<pair<size_t, node>, vector<pair<size_t, node>>, greater<pair<size_t, node>>> pq;
    pq.push({0, node_start});

    set<node> visited;
    map<node, size_t> dist;
    dist[node_start] = 0;
    bool reachend    = false;
    while (!pq.empty()) {
        node currnode = pq.top().second;
        pq.pop();
        if (visited.contains(currnode)) {
            continue;
        }
        visited.insert(currnode);

        for (auto [node, cost] : adj(currnode, labyrinth)) {
            if (get<0>(node) == i_end && get<1>(node) == j_end) {
                reachend = true;
                break;
            }

            if (!dist.contains(node)) {
                dist[node] = dist[currnode] + cost;
                pq.push({dist[node], node});
            } else {
                if (dist[node] > dist[currnode] + cost) {
                    dist[node] = dist[currnode] + cost;
                    pq.push({dist[node], node});
                }
            }
        }

        if (reachend) {
            break;
        }
    }
    println("{}", pq.top().first + 1);
}

vector<pair<vector<node>, size_t>> adj2(const node& curr, const vector<string>& labyrinth) {
    vector<pair<vector<node>, size_t>> res;

    auto [r, c, f] = curr;

    auto findcorner = [&labyrinth](node currnode, size_t cost) -> pair<vector<node>, size_t> {
        auto [rc, cc, fc] = currnode;
        auto rnext        = rc + dirs[fc][0];
        auto cnext        = cc + dirs[fc][1];
        vector<node> nodevec;
        while (labyrinth[rnext][cnext] != '#') {
            nodevec.push_back({rnext, cnext, fc});
            ++cost;
            if (labyrinth[rnext + dirs[rotate_clockwise(fc)][0]][cnext + dirs[rotate_clockwise(fc)][1]] == '.' ||
                labyrinth[rnext + dirs[rotate_counterclockwise(fc)][0]][cnext + dirs[rotate_counterclockwise(fc)][1]] == '.') {
                break;
            }
            rnext += dirs[fc][0];
            cnext += dirs[fc][1];
        }
        return {nodevec, cost};
    };

    res.push_back(findcorner(curr, 0));
    auto f1 = rotate_clockwise(f);
    res.push_back(findcorner({r, c, f1}, 1000));
    auto f2 = rotate_counterclockwise(f);
    res.push_back(findcorner({r, c, f2}, 1000));

    return res;
}

void part2() {
    ifstream input("input/input-16");
    vector<string> labyrinth;
    for (string line; getline(input, line);) {
        labyrinth.push_back(line);
    }

    size_t i_start = 0;
    size_t j_start = 0;
    size_t i_end   = 0;
    size_t j_end   = 0;

    for (size_t i = 0; i < labyrinth.size(); ++i) {
        if (labyrinth[i].contains('S')) {
            i_start = i;
            j_start = labyrinth[i].find('S');
        }
        if (labyrinth[i].contains('E')) {
            i_end = i;
            j_end = labyrinth[i].find('E');
        }
    }
    node node_start{i_start, j_start, 1};

    priority_queue<tuple<size_t, node, vector<node>>, vector<tuple<size_t, node, vector<node>>>, greater<tuple<size_t, node, vector<node>>>>
        pq;
    pq.push({0, node_start, {node_start}});

    map<node, size_t> bestscore;
    bestscore[node_start] = 0;
    vector<vector<node>> allpath;
    size_t lowest = numeric_limits<size_t>::max();
    while (!pq.empty()) {
        auto [score, currnode, path] = pq.top();
        pq.pop();

        if (lowest != numeric_limits<size_t>::max() && score > lowest) {
            continue;
        }

        if (bestscore.contains(currnode) && bestscore[currnode] < score) {
            continue;
        }

        bestscore[currnode] = score;

        auto [r, c, f] = currnode;
        if (r == i_end && c == j_end) {
            if (score <= lowest) {
                if (score < lowest) {
                    lowest = score;
                    allpath.clear();
                }
                allpath.push_back(path);
            }
            continue;
        }

        for (auto [node, cost] : adj(currnode, labyrinth)) {
            if (find(path.begin(), path.end(), node) == path.end()) {
                auto tmppath = path;
                tmppath.push_back(node);
                pq.push({score + cost, node, tmppath});
            }
        }
    }

    set<pair<size_t, size_t>> uniquetiles;

    for (auto& path : allpath) {
        for (auto [r, c, f] : path) {
            uniquetiles.insert({r, c});
        }
    }

    println("{}", uniquetiles.size());
}

int main(int argc, char* argv[]) {
    part1();
    part2();
    return 0;
}
