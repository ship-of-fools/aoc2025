#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>

#include <cstdint>
#include <ranges>

#include <cmath>
#include <algorithm>

struct Pos {
    uint64_t x;
    uint64_t y;
    uint64_t z;

    bool operator==(const Pos& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

uint64_t distance(Pos &a, Pos &b){
    uint64_t x = a.x - b.x;
    uint64_t y = a.y - b.y;
    uint64_t z = a.z - b.z;
    uint64_t result = x*x + y*y + z*z;
    result = std::sqrt(result);
    return result;
}

struct PairDist {
    Pos p1;
    Pos p2;
    uint64_t dist;
};

void print_circuits(std::vector<std::vector<Pos>> &circuits){
    std::cout << "\n";
    for (const auto& circuit : circuits){
        std::cout << "[\n";
        for (const auto& node : circuit){
            std::cout << "   (" << node.x << ", " << node.y << ", " << node.z << ")\n";
        }
        std::cout << "]\n";
    }
}

struct PairIdx {
    std::size_t p1;
    std::size_t p2;
};


void merge_circuits(std::vector<std::vector<Pos>> &circuits, std::size_t i, std::size_t j){
    // Should probably check which circuit is smallest and merge that one
    std::vector<Pos> circuit_to_merge = circuits[i];
    
    for (const auto &node : circuit_to_merge){
        // Only add nodes that don't already exist in the other circuit
        auto it = std::find(circuits[j].begin(), circuits[j].end(), node);
        if (it == circuits[j].end()){
            circuits[j].emplace_back(node);
        }
    }
    circuits.erase(circuits.begin() + i);
}

PairIdx find_connections(std::vector<std::vector<Pos>> &circuits){
    std::vector<Pos> visited_nodes;
    std::vector<std::size_t> visited_nodes_circuit_idx;

    PairIdx ret{};

    for (std::size_t i=0; i<circuits.size(); ++i){
        bool in_visited_nodes = false;
        for (std::size_t j=0; j<circuits[i].size(); ++j){
            // Check if node is in visited nodes
            auto node = circuits[i][j];

            auto it = std::find(visited_nodes.begin(), visited_nodes.end(), node);

            if (it != visited_nodes.end()) {
                std::size_t index = std::distance(visited_nodes.begin(), it);
                // node is in visited nodes
                in_visited_nodes = true;
                ret.p1 = visited_nodes_circuit_idx[index];
                ret.p2 = i;
                break;
            } else {
                visited_nodes.emplace_back(node);
                visited_nodes_circuit_idx.emplace_back(i);
            }
        }
        if (in_visited_nodes) break;
    }
    return ret;
}

PairIdx refine_circuits(std::vector<std::vector<Pos>> &circuits){
    std::vector<Pos> visited_nodes;
    std::vector<std::size_t> visited_nodes_circuit_idx;

    PairIdx ret{};
    ret.p1 = 1;
    while (ret.p1 != 0 || ret.p2 != 0){
        ret.p1 = 0;
        ret.p2 = 0;
        ret = find_connections(circuits);

        // Merge circuits
        if (ret.p1 == 0 && ret.p2 == 0){
            // should stop as no merge was found
        } else {
            // std::cout << "merging [" << ret.p1 << ", " << ret.p2 << "]\n";
            // print_circuits(circuits);
            merge_circuits(circuits, ret.p1, ret.p2);
            // print_circuits(circuits);
        }

    }


    return ret;
}

void print_point(Pos &pos){
    std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")";
}

void print_point_pair(Pos &a, Pos &b){
    std::cout << "(" << a.x << ", " << a.y << ", " << a.z << ") " <<
                 "(" << b.x << ", " << b.y << ", " << b.z << ")\n";
}


int main() {
    std::ifstream file("input.txt");
    std::vector<std::string> lines;
    std::string line;
    // while (std::getline(file, line)) lines.push_back(line);
    uint32_t line_idx = 0;
    std::vector<Pos> points;
    while (std::getline(file, line)){
        uint8_t idx = 0;
        Pos p;
        for (auto &&part : std::views::split(line, ',')) {
            std::string s(&*part.begin(), std::ranges::distance(part));
            switch (idx++){
                case 0:
                    p.x = std::stoi(s);
                case 1:
                    p.y = std::stoi(s);
                case 2:
                    p.z = std::stoi(s);
            }
        }
        points.push_back(p);
    }

    // for (const auto& pos : points){
    //     std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
    // }

    std::vector<PairDist> all_distances;

    // Brute force: compute all pairwise distances
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            uint64_t d = distance(points[i], points[j]);
            all_distances.push_back({points[i], points[j], d});
            // print_point_pair(points[i], points[j]);
        }
    }

    // std::cout << "Computed distances: " << all_distances.size() << '\n';

    // Sort by distance
    std::sort(all_distances.begin(), all_distances.end(),
              [](const PairDist& a, const PairDist& b) {
                  return a.dist < b.dist;
              });

    // std::vector<std::vector<Pos>>;

    // for (size_t i = 0; i < all_distances.size()-1; ++i){
    //     const auto &pair_dist = all_distances[i];

    // }

    // Print the 10 shortest (or fewer if not enough pairs)
    size_t limit = std::min<size_t>(10, all_distances.size());
    for (size_t k = 0; k < limit; ++k) {
        const auto& pd = all_distances[k];
        // std::cout << "Pair " << k+1 << ": ("
        //           << pd.p1.x << "," << pd.p1.y << "," << pd.p1.z << ") - ("
        //           << pd.p2.x << "," << pd.p2.y << "," << pd.p2.z << ") "
        //           << "Distance = " << pd.dist << "\n";
    }

    std::vector<std::vector<Pos>> circuits;

    // Add all circuits of single nodes
    for (size_t i = 0; i < points.size(); ++i){
        circuits.emplace_back(std::vector<Pos>{points[i]});
    }
    // print_circuits(circuits);


    constexpr int num_connections = 1000;
    std::size_t i_connected {};

    for (i_connected = 0; i_connected < num_connections; ++i_connected){
        circuits.emplace_back(std::vector<Pos>{all_distances[i_connected].p1, all_distances[i_connected].p2});
    }
    
    // print_circuits(circuits);

    PairIdx pid = refine_circuits(circuits);

    std::vector<std::size_t> circuit_lengths;
    for (std::size_t i=0; i<circuits.size(); ++i){
        circuit_lengths.emplace_back(circuits[i].size());
    }
    std::sort(circuit_lengths.begin(), circuit_lengths.end(), std::greater<std::size_t>());

    std::size_t part1{1};

    for (std::size_t i=0; i<3; ++i){
        part1 *= circuit_lengths[i];
    }

    std::cout << "part1: " << part1 << '\n';

    while(circuits.size() > 1){
        circuits.emplace_back(std::vector<Pos>{all_distances[i_connected].p1, all_distances[i_connected].p2});
        ++i_connected;
        refine_circuits(circuits);
        // print_circuits(circuits);
        // char c;
        // std::cin >> c;
    }

    uint64_t part2 = all_distances[i_connected-1].p1.x * all_distances[i_connected-1].p2.x;
    std::cout << "part2: " << part2 << '\n';

    // std::cout << "[ " << pid.p1 << ", " << pid.p2 << "]\n";
    // merge_circuits(circuits, pid.p1, pid.p2);
    // print_circuits(circuits);


    
}
