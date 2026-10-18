#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __linux__
#include <sys/random.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

bool is_even(int number) {
    return (number % 2) == 0;
}

int main(void) {
    unsigned int num;
#ifdef __linux__
    if (getrandom(&num, sizeof(num), 0) != sizeof(num)) {
        return 1;
    }
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        return 1;
    }
    if (read(fd, &num, sizeof(num)) != sizeof(num)) {
        close(fd);
        return 1;
    }
    close(fd);
#endif

    if (is_even(num)) {
        printf("Even\n");
    } else {
        printf("Odd\n");
    }
    return 0;
}