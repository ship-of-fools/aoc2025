#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utility>
#include <cstdint>
#include <cstdio>
#include <charconv>
#include <cstdlib>

#include <vector>
#include <unordered_set>
#include <algorithm>

#include <chrono>

// #define TIME_IT
#ifdef TIME_IT
#include <chrono>
#endif

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


    std::vector<std::pair<long long, long long>> ranges;
    std::vector<long long> ids;
    // long long ranges[256][2];

    for (char* ptr = data; ptr < end; ++ptr) {
        char* lower_range_end;
        char* upper_range_start;
        char* upper_range_end;
        while (!(*ptr == '\n' && (*(ptr+1) == '\n' || *(ptr+1) == '\0'))){
            while (*ptr != '-') ++ptr;
            lower_range_end = ptr;
            long long lower_range;
            auto result = std::from_chars(line_start, lower_range_end, lower_range);
            // printf("lower_range: %lld\n", lower_range);
            upper_range_start = lower_range_end+1;
            while(*ptr != '\n') ++ptr;
            upper_range_end = ptr;
            long long upper_range;
            result = std::from_chars(upper_range_start, upper_range_end, upper_range);
            // printf("upper_range: %lld\n", upper_range);
           
            ranges.emplace_back(lower_range, upper_range);

        

            if (*ptr == '\n' || ptr == end - 1) {
                size_t line_length = ptr - line_start + (*ptr != '\n' ? 1 : 0); // include last char if no '\n'
                // std::string_view line(line_start, line_length);
                line_start = ptr + 1;
            }

        }
        // printf("found blank line\n");
        ptr += 2;
        while(ptr < end){
            auto num_start = ptr;
            while (*ptr != '\n' && ptr != end) ++ptr;
            auto num_end = ptr;
            long long num;
            auto result = std::from_chars(num_start, num_end, num);
            ids.emplace_back(num);
            // printf("%lld\n", num);
            ++ptr;
        }

    }
    // munmap(data, len);

    // Sort ranges on low id
    std::sort(ranges.begin(), ranges.end(),
                [](const auto &a, const auto &b){
                    return a.first < b.first;
                });

    // Make new merged vector as it's quicker than removing/resizing in place
    std::vector<std::pair<long long,long long>> merged;
    merged.push_back(ranges[0]);

    for (std::size_t i = 1; i < ranges.size(); ++i) {
        auto& last = merged.back();
        if (ranges[i].first <= last.second) {
            last.second = std::max(last.second, ranges[i].second);
        } else {
            merged.push_back(ranges[i]);
        }
    }

    for (const auto &id : ids){
        for (const auto &[low, high] : merged){
            if (id >= low && id <= high){
                ++part1;
                break;
            }
        }
    }

    for (const auto &[low, high] : merged){
        long long n_in_range = (high - low) + 1;
        part2 += n_in_range;
    }

    printf("part1: %lld\n", part1);
    printf("part2: %lld\n", part2);

    #ifdef TIME_IT
    auto timer_end = std::chrono::high_resolution_clock::now();
    // Duration in microseconds
    auto timer_duration = std::chrono::duration_cast<std::chrono::microseconds>(timer_end - timer_start);
    printf("time: %ld us\n", timer_duration.count());
    #endif
}
