#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
// #include <iostream>
#include <string_view>
#include <cstdint>
#include <cstdio>
#include <charconv>
#include <cstdlib>

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}


std::pair<int, int> turn_dial(int dial, bool left, int number, int &p1){
    int res = 0;
    int clicks = 0;
    if (dial == 0 && left){
        clicks = -1;
    }


    if (left == true){
        res = dial - number;
        while (res < 0) {
            res += 100;
            clicks++;
        }
    } else {
        res = dial + number;
        while (res >= 100) {
            res -= 100;
            clicks++;
        }
    }
    if (res == 0){
        p1 += 1;
       if (left == 1){
            clicks += 1;
        }
        
    }
    dial = res;
    return {dial, clicks};
}

std::pair<int, int> turn_dial_slow(int dial, bool left, int number, int &p1){
    int res = 0;
    int clicks = 0;
    if (left == true){
        res = dial - number;
        int n;
        if (res < 0){
            if (dial == 0){
                n = std::abs(int(res/100));
                if (res % 100 == 0){
                    n -= 1;
                }
            } else {
                n = 1 + std::abs(int(res/100));
                if (res % 100 == 0){
                    n -= 1;
                }

            }
            clicks += n;
            res = mod(res, 100);
        }
    } else {
        res = dial + number;
        int n;
        if (res > 99){
            n = std::abs(int(res/100));
            clicks += n;
            res = mod(res, 100);
            if (res == 0){
                clicks -= 1;
            }
        }
    }

    dial = res;
    if (dial == 0){
        p1 += 1;
        clicks += 1;
    }

    return {dial, clicks};
}

// void test(int dial, bool left, int number, int expected_dial, int expected_clicks){
//     int add_p1 = 0;
//     auto [new_dial, new_clicks] = turn_dial(dial, left, number, add_p1);

//     printf("dial_start: %d, turn: %c%d, dial_end: %d, expected dial_end: %d, clicks: %d, expected clicks: %d",
//            dial, (left ? 'L' : 'R'), number, new_dial, expected_dial, new_clicks, expected_clicks);

//     if (new_dial == expected_dial && new_clicks == expected_clicks){
//         printf(" PASSED!\n");
//     } else {
//         printf(" FAILED!\n");
//     }
// }


int main() {
    int fd = open("input.txt", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);

    size_t len = sb.st_size;
    char* data = (char*)mmap(nullptr, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    char* line_start = data;
    char* end = data + len;

    int dial = 50;
    int part1 = 0;
    int part2 = 0;

    for (char* ptr = data; ptr < end; ++ptr) {
        if (*ptr == '\n' || ptr == end - 1) {
            size_t line_length = ptr - line_start + (*ptr != '\n' ? 1 : 0); // include last char if no '\n'
            std::string_view line(line_start, line_length);

            int number;
            auto result = std::from_chars(line.data()+1, line.data() + line.size(), number);

            int add_p1 = 0;
            auto [new_dial, new_clicks] = turn_dial(dial, *line_start == 'L', number, add_p1);
            part1 += add_p1;
            part2 += new_clicks;
            dial = new_dial;
            line_start = ptr + 1;
        }
    }
    // munmap(data, len);

    printf("part1: %d\n", part1);
    printf("part2: %d\n", part2);

    // auto [test_dial, test_clicks] = turn_dial(50, 1, 50);
    // std::cout << test_dial << " " << test_clicks << std::endl;

    // test(50, 1, 50, 0, 1);
    // test(50, 1, 200, 50, 2);

    // test(50, 1, 50, 0, 1);
    // test(50, 1, 200, 50, 2);
    // test(0, 1, 50, 50, 0);
    // test(100, 1, 100, 0, 1);
    // test(0, 1, 100, 0, 1); // Hard one
    // test(0, 1, 200, 0, 2);
    // test(200, 1, 200, 0, 1);

    // test(50, 0, 50, 0, 1);
    // test(50, 0, 51, 1, 1);
    // test(50, 0, 250, 0, 3);
    // test(50, 0, 251, 1, 3);
    // test(50, 0, 249, 99, 2);
    // test(0, 0, 50, 50, 0);
    // test(0, 0, 100, 0, 1);
    // test(0, 0, 250, 50, 2);
    // test(0, 0, 200, 0, 2);
    // test(50, 0, 1000, 50, 10);

    // test(3, 1, 303, 0, 4); // Hard one

}
