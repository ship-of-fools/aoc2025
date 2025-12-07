#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>

// Custom hash for pair<int,int>
struct PairHash {
    std::size_t operator()(const std::pair<int,int>& p) const noexcept {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

std::vector<std::string> lines;
int last_row_idx;
int last_col_idx;

// Memoization: map (row,col) -> splits
std::unordered_map<std::pair<int,int>, long long, PairHash> memo;

// Track used splitters
std::set<std::pair<int,int>> used_splitter_ids;

long long do_recursion(int row, int col, long long splits = 0) {
    auto key = std::make_pair(row,col);

    // Check memoization
    auto it = memo.find(key);
    if (it != memo.end()) {
        return it->second;
    }

    // Out of bounds, probably don't need to check this?
    if (col < 0 || col > last_col_idx) {
        memo[key] = splits;
        return splits;
    }

    // Bottom row
    if (row == last_row_idx) {
        memo[key] = splits;
        return splits;
    }

    if (lines[row][col] == '.') {
        splits = do_recursion(row+1, col, splits);
        memo[key] = splits;
        return splits;
    }

    if (lines[row][col] == '^') {
        if (used_splitter_ids.find(key) == used_splitter_ids.end()) {
            used_splitter_ids.insert(key);
        }
        splits = 1 + splits
                 + do_recursion(row+1, col-1, splits)
                 + do_recursion(row+1, col+1, splits);
        memo[key] = splits;
        return splits;
    }

    memo[key] = splits;
    return splits;
}

int main() {
    // Read file
    std::ifstream file("input.txt");
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    last_row_idx = (int)lines.size() - 1;
    last_col_idx = (int)lines[0].size() - 1;

    // Find starting position
    int starting_pos = 0;
    for (int i = 0; i < (int)lines[0].size(); ++i) {
        if (lines[0][i] == 'S') {
            starting_pos = i;
            break;
        }
    }

    long long splits = do_recursion(1, starting_pos, 0);
    std::cout << used_splitter_ids.size() << '\n';
    std::cout << (splits + 1) << '\n';

    return 0;
}
