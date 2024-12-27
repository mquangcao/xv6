#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096 

int main() {
    char *buffer = sbrk(2 * PGSIZE);
    if (buffer == (char *)-1) {
        printf("Failed to allocate memory\n");
        exit(1);
    }

    buffer[0] = 'x';

    uint64 accessed = 0;
    if (pgaccess(buffer, 2, (char *)&accessed) < 0) {
        printf("pgaccess failed\n");
        exit(1);
    }

    if (accessed & 0x1) {
        printf("Page 0 accessed\n");
    } else {
        printf("Page 0 not accessed\n");
    }

    if (accessed & 0x2) {
        printf("Page 1 accessed\n");
    } else {
        printf("Page 1 not accessed\n");
    }

    exit(0);
}
