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
    /* Possible weaknesses found:
     * Flawfinder open: Check when opening files - can an attacker redirect it (via symlinks), force the opening of special file type (e.g., device files), move things around to create a race condition, control its ancestors, or change its contents? (CWE-362). (risk 2, misc)
     */
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        return 1;
    }
    /* Possible weaknesses found:
     * Flawfinder read: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
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