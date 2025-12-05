#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

// #define TIME_IT

#ifdef TIME_IT
#include <chrono>
#endif

void highest_valid_number(std::string_view line, std::size_t numbers_len, char &largest_digit, std::size_t &largest_digit_index){
    largest_digit = 0;
    largest_digit_index = 0;
    for (std::size_t i=0; i<line.size()-numbers_len + 1; ++i){
        // std::cout << line[i] - '0' << '\n';
        if ((line[i] - '0') > largest_digit){
            // std::cout << line[i] - '0' << '\n';
            largest_digit = line[i] - '0';
            largest_digit_index = i;
            // std::cout << largest_digit_index << '\n';
            if (largest_digit == '9') break;
        }
    }
    largest_digit = largest_digit + '0';
}

int main(){

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

    long long part_1 = 0;
    long long part_2 = 0;


    std::size_t part_1_num_len = 2;
    std::size_t part_2_num_len = 12;
    // char ans = '0';
    // size_t index = 0;
    // highest_valid_number("0123456789", 2, ans, index);

    // std::cout << ans << ", " << index << '\n';

    for (char* ptr = data; ptr < end; ++ptr) {
        if (*ptr == '\n' || ptr == end - 1) {
            size_t line_length = ptr - line_start + (*ptr != '\n' ? 1 : 0); // include last char if no '\n'
            std::string_view line(line_start, line_length);
            // std::cout << line << '\n';


            char ans = '0';
            size_t index = 0;
            long long result = 0;
            for (std::size_t i=0; i < part_1_num_len; ++i){
                std::string_view sub_line(line_start + index, line_length-index);
                highest_valid_number(sub_line, part_1_num_len - i, ans, index);
                index += 1;
                result = result * 10 + (ans - '0');
            }
            part_1 += result;

            
            index = 0;
            ans = '0';
            result = 0;
            std::string_view sub_line(line_start + index, line_length-index);
            for (std::size_t i=0; i < part_2_num_len; ++i){
                highest_valid_number(sub_line, part_2_num_len - i, ans, index);
                index += 1;
                sub_line = sub_line.substr(index);
                result = result * 10 + (ans - '0');
            }
            // std::cout << result << '\n';
            part_2 += result;



            line_start = ptr + 1;
        }
    }

    printf("part1: %lld\n", part_1);
    printf("part2: %lld\n", part_2);

    #ifdef TIME_IT
    auto timer_end = std::chrono::high_resolution_clock::now();
    // Duration in microseconds
    auto timer_duration = std::chrono::duration_cast<std::chrono::microseconds>(timer_end - timer_start);
    printf("time: %ld us\n", timer_duration.count());
    #endif

    // char ans = '0';
    // size_t index = 0;
    // long long result = 0;
    // part_2_num_len = 12;

    // std::string input = "818181911112111";
    // std::size_t line_length = 15;
    // line_start = &input[0];
    // std::string_view sub_line(line_start + index, line_length-index);
    // for (std::size_t i=0; i < part_2_num_len; ++i){
    //     std::cout << sub_line << '\n';
    //     highest_valid_number(sub_line, part_2_num_len - i, ans, index);
    //     std::cout << ans << ", " << index << '\n';
    //     index += 1;
    //     sub_line = sub_line.substr(index);
    //     result = result * 10 + (ans - '0');
    // }
    // printf("%lld\n", result);

    return 0;
}