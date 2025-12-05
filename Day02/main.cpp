#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


#include <cstdint>
#include <vector>
#include <string>

// #define TIME_IT

#ifdef TIME_IT
#include <chrono>
#endif

bool recursive_check(std::string_view id, std::size_t split_len){
    if (id.size() == split_len){
        return true;
    }
    std::string_view left = id.substr(0, split_len);
    std::string_view right = id.substr(split_len);
    std::string_view right_check = id.substr(split_len, split_len);

    // printf("left: %.*s\n", static_cast<int>(left.size()), left.data());
    // printf("right: %.*s\n", static_cast<int>(right.size()), right.data());
    // printf("right_check: %.*s\n", static_cast<int>(right_check.size()), right_check.data());

    if (left == right_check){
        return recursive_check(right, split_len);
    } else {
        return false;
    }

}

bool check_id(std::string_view id){
    std::size_t mid_len = id.size() >> 1;
    for (std::size_t i=1; i<mid_len+1; ++i){
        if (recursive_check(id, i)){
            return true;
        }
    }
    return false;
}

int main() {
    #ifdef TIME_IT
    auto timer_start = std::chrono::high_resolution_clock::now();
    #endif
    int fd = open("input.txt", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);

    size_t len = sb.st_size;
    char* data = (char*)mmap(nullptr, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    char* line_start = data;
    char* end = data + len;

    long long part1 = 0;
    long long part2 = 0;

    std::vector<std::string> starting_nums;
    std::vector<std::string> ending_nums;

    for (char* ptr = data; ptr < end; ++ptr) {
        if (*ptr == '\n' || ptr == end - 1) {
            size_t line_length = ptr - line_start + (*ptr != '\n' ? 1 : 0); // include last char if no '\n'
            std::string_view line(line_start, line_length);
            printf("%.*s\n", static_cast<int>(line.size()), line.data());
            line_start = ptr + 1;
        }
        // parse until -
        char *start_ptr = ptr;
        while (*ptr != '-'){
            ++ptr;
        }
        starting_nums.emplace_back(start_ptr, ptr);
        ++ptr; // skip the -
        start_ptr = ptr;
        while (*ptr != ',' && ptr != end){
            ++ptr;         
        }
        ending_nums.emplace_back(start_ptr, ptr);

    }

    for (std::size_t i=0; i < starting_nums.size(); ++i){
        auto start = std::stoll(starting_nums[i]);
        auto end = std::stoll(ending_nums[i]);
        for (auto i = start; i <= end; ++i){
            std::string i_str = std::to_string(i);
            std::size_t len = i_str.size();

            if (check_id(i_str)){
                // printf("%d\n", i);
                part2 += i;
            };

            if (len % 2 == 0){
                std::string left = i_str.substr(0, len >> 1);
                std::string right = i_str.substr(len >> 1);
                if (left == right){
                    part1 += i;
                }
            }
        }
    }

    // Part 2



    // printf("%d\n" , recursive_check("56565656", 2));

    printf("part1: %lld\n", part1);
    printf("part2: %lld\n", part2);

    #ifdef TIME_IT
    auto timer_end = std::chrono::high_resolution_clock::now();
    // Duration in microseconds
    auto timer_duration = std::chrono::duration_cast<std::chrono::microseconds>(timer_end - timer_start);
    printf("time: %ld us\n", timer_duration.count());
    #endif



}
