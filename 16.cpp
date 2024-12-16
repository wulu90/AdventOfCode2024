#include <fstream>
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
    auto r = get<0>(curr);
    auto c = get<1>(curr);
    auto f = get<2>(curr);
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

int main(int argc, char* argv[]) {
    part1();
    return 0;
}
