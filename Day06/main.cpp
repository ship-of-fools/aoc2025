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
#include <cmath>
#include <cctype>

// #include <iostream>

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

    constexpr uint8_t rows = 5;

    uint8_t rows_idx = 0;
    uint16_t cols_idx = 0;
    long long num[rows][1024];
    size_t cols = 0;

    char digits[rows-1][1024][8]; // could probably be less than 8
    char digits_len[rows-1][1024];

    bool is_multiply[1024];

    for (char* ptr = data; ptr < end; ++ptr) {
        // printf("%c\n", *ptr);
        char* l_ptr = ptr;
        char* r_ptr{};
        while (*ptr != '\n' && ptr < end){
            l_ptr = ptr;
            // printf("%c\n", *ptr);
            while (*ptr != ' ' && *ptr != '\n') {
                // printf("%c\n", *ptr);
                // char c;
                // std::cin >> c;
                ++ptr;
            }
            if (*(ptr-1) == '*' || *(ptr-1) == '+'){
                // printf("here\n");
                num[rows_idx][cols_idx++] = *(ptr-1);
                if (*(ptr-1) == '*'){
                    is_multiply[cols_idx-1] = true;
                } else {
                    is_multiply[cols_idx-1] = false;
                }
                // printf("%c\n", num[rows_idx][cols_idx-1]);
            }else{
                auto result = std::from_chars(l_ptr, ptr, num[rows_idx][cols_idx++]);
                std::size_t len = ptr - l_ptr;
                for (int i=0; i<len; i++){
                    digits[rows_idx][cols_idx-1][i] = *(l_ptr + i)-'0';
                    // printf("%d, %d, %d\n", rows_idx, cols_idx-1, i);
                    // printf("%d\n", *(l_ptr + i)-'0');
                }
                digits_len[rows_idx][cols_idx-1] = len;
                // printf("%d\n", num[rows_idx][cols_idx-1]);
                // char c;
                // std::cin >> c;
            }
            
            // printf("%c:%c\n", *l_ptr, *(ptr));
            // printf("%d\n", num[rows_idx][cols_idx-1]);
            // char c;
            // std::cin >> c;
            while (*ptr == ' ' || *ptr == '\n'){
                if (*ptr == '\n'){
                    ++rows_idx;
                    cols = cols_idx;
                    // printf("max cols: %lld\n", cols);
                    cols_idx = 0;
                    
                }
                ++ptr;
            }
        }
        // char c;
        // std::cin >> c;
    }
    // munmap(data, len);

    // for (int i=0; i<rows-1; ++i){
    //     for (int j=0; j<cols; ++j){
    //         for (int k=0; k<8; ++k){
    //             printf("%d", digits[i][j][k]);
    //         }
    //         printf("%d\n", digits_len[i][j]);
    //     }
    // }

    // for (int i=0; i<rows-1; ++i){
    //     for (int j=0; j<cols; ++j){
    //         for (int k=0; k<digits_len[i][j]; ++k){
    //             printf("%d", digits[i][j][k]);
    //         }
    //         printf("\n");
    //     }
    // }
    // char c;
    // std::cin >> c;

    // for (std::size_t row=0; row < rows; ++row){
    //     for (std::size_t col=0; col < cols; ++col){
    //         std::cout << num[row][col] << '\n';
    //     }
    //     // char c;
    //     // std::cin >> c;
    // }

    for (int col = 0; col < cols; ++col){
        long long res = 0LL;
        bool mul = false;
        for (int row = rows-1; row >=0; --row){
            
            // printf("%d, %d: %d\n", row, col, num[row][col]);
            if (row == rows-1){
                if (num[row][col] == '*'){
                    mul = true;
                }
                // printf("sign: %c\n", num[row][col]);
            } else if (row == rows - 2){
                    res = static_cast<long long>(num[row][col]);
                    // printf("res = %lld\n", res);
            } else {
                if (mul){
                    res *= static_cast<long long>(num[row][col]);
                    // printf("res * %lld = %lld\n", num[row][col], res);
                } else {
                    res += num[row][col];
                    // printf("res + %lld = %lld\n", num[row][col], res);
                }
            }
        }
        

        
        // printf("[%d, %d, %d]\n", p2_numbers[0], p2_numbers[1], p2_numbers[2]);
        part1 += res;

        // Part 2
        // rows = 5, we have 4 numbers though

        // Add extra step to parsing to save each char into an array to make up the number
        // that way we can easily and quickly make the vertical numbers
        // use digits[] that we've made in the parsing

    } 

    // ok this is going to be slow because the old loop went bottom to top and now we need to do top to bottom
    // TODO: redo top loop to be top to bottom

    // char digits[rows-1][1024][8]; // could probably be less than 8
    // char digits_len[rows-1][1024];

    

    for (int col = 0; col < cols; ++col){
        uint8_t max_digits = 0;
        for (int row = 0; row<rows-2; ++row){
            uint8_t digits_length = digits_len[row][col];
            if (digits_length > max_digits) max_digits = digits_length;
        }
        // printf("%d\n", max_digits);
        // char c;
        // std::cin >> c;
        for (int i=max_digits-2; i>=0; --i){
            int res_num = 0;
            for (int row = 0; row<rows-2; ++row){
                if (digits_len[row][col] >= i){
                    // printf("max_digits: %d, this number: %lld", i, num[row][col]);
                    // printf("(%d * 10) + %d\n", res_num, digits[row][col][i]);
                    res_num = (res_num * 10) + digits[row][col][i];
                }
            }
        }
        
    }

    // New part 2
    // bool is_multiply[1024];
    char rotated[1024][rows];
    int line_len = 0;
    char* p2_ptr = line_start;
    for (int row=0; row < rows; ++row){
        while (*p2_ptr != '\n') ++p2_ptr;
        line_len = p2_ptr - line_start;
        // printf("line length: %d\n", line_len);
        for (int i=0; i<=line_len-1; ++i){
            rotated[i][row] = *(p2_ptr-i-1);
            // printf("%c\n", rotated[i][0]);
        }
        p2_ptr += 1;
        line_start = p2_ptr;
    }

    // for (int i=0; i<line_len; ++i){
    //     for (int j=0; j<rows; ++j){
    //         printf("%c", rotated[i][j]);
    //     }
    //     printf("\n");
    // }


    int set_id = 0;
    long long result = 1;
    bool multiply = false;
    for (int i=0; i<line_len; ++i){
        uint16_t number{};
        bool found_digit = false;
        multiply = is_multiply[cols-1-set_id];
        
        for (int j=0; j<rows-1; ++j){
            if (isdigit(rotated[i][j])){
                // printf("number = %d * 10 + %d\n", number, rotated[i][j]-'0');
                number = number * 10 + (rotated[i][j]-'0');
                found_digit = true;
            }
            // printf("number = %d * 10 + %d\n", number, rotated[i][j]-'0');
            // number = number * 10 + (rotated[i][j]-'0');
        }
        if (!found_digit){
            if (!multiply){
                --result; 
            }
            part2 += result;
            // printf("part2 = %lld (%lld)\n", part2, result);
            ++set_id;
            result = 1;
            
        } else {
            if (multiply){
                // printf("result = %lld * %lld\n", result, number);
                result = result * number;
            } else {
                // printf("result = %lld + %lld\n", result, number);
                result = result + number;
            }
        }
    }
    if (!multiply){
        part2 = part2 + result - 1;
    } else {
        part2 += result;
    }


    



    //92424980957 too low

    printf("part1: %lld\n", part1);
    printf("part2: %lld\n", part2);

    #ifdef TIME_IT
    auto timer_end = std::chrono::high_resolution_clock::now();
    // Duration in microseconds
    auto timer_duration = std::chrono::duration_cast<std::chrono::microseconds>(timer_end - timer_start);
    printf("time: %ld us\n", timer_duration.count());
    #endif
}
