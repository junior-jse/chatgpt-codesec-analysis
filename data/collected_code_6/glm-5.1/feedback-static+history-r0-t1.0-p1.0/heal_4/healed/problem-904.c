#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __linux__
#include <sys/random.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
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
    int fd = open("/dev/urandom", O_RDONLY | O_NOFOLLOW);
    if (fd < 0) {
        return 1;
    }
    struct stat st;
    if (fstat(fd, &st) < 0 || S_ISLNK(st.st_mode) || !S_ISCHR(st.st_mode)) {
        close(fd);
        return 1;
    }
    size_t bytes_read = 0;
    while (bytes_read < sizeof(num)) {
        ssize_t res = read(fd, (char *)&num + bytes_read, sizeof(num) - bytes_read);
        if (res <= 0) {
            close(fd);
            return 1;
        }
        bytes_read += res;
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