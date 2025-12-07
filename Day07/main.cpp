#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

// #define TIME_IT
#ifdef TIME_IT
#include <chrono>
#endif


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
std::unordered_set<std::pair<int,int>, PairHash> used_splitter_ids;

long long do_recursion(int row, int col) {
    auto key = std::make_pair(row,col);

    // Check memoization
    if (auto it = memo.find(key); it != memo.end()) {
        return it->second;
    }

    // Out of bounds or bottom row
    if (col < 0 || col > last_col_idx || row == last_row_idx) {
        return memo[key] = 0;
    }

    char cell = lines[row][col];
    long long result = 0;

    if (cell == '.') {
        result = do_recursion(row+1, col);
    } else if (cell == '^') {
        used_splitter_ids.insert(key);
        result = 1 + do_recursion(row+1, col-1) + do_recursion(row+1, col+1);
    }

    return memo[key] = result;
}

int main() {

    #ifdef TIME_IT
    auto timer_start = std::chrono::high_resolution_clock::now();
    #endif

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

    long long splits = do_recursion(1, starting_pos);
    std::cout << used_splitter_ids.size() << '\n';
    std::cout << (splits + 1) << '\n';

    #ifdef TIME_IT
    auto timer_end = std::chrono::high_resolution_clock::now();
    // Duration in microseconds
    auto timer_duration = std::chrono::duration_cast<std::chrono::microseconds>(timer_end - timer_start);
    printf("time: %ld us\n", timer_duration.count());
    #endif

    return 0;
}
