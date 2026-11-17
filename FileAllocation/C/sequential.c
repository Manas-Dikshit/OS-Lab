#include <stdio.h>

int main(void) {
    int blocks, n, i, b, start, run, freeBlocks = 0, size[20];
    char names[20][16];
    /* shortcut: max 100 disk blocks and 20 files, enlarge the arrays for bigger runs */
    int disk[100] = {0};                     /* 0 = free, 1 = used */

    printf("Enter number of disk blocks: ");
    scanf("%d", &blocks);
    printf("Enter number of files: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        printf("Enter name and size for file %d (e.g. alpha 4): ", i + 1);
        scanf("%15s %d", names[i], &size[i]);
    }

    printf("\nFile\tSize\tStart\tBlocks\n");
    for (i = 0; i < n; i++) {
        start = -1;
        run = 0;
        for (b = 0; b < blocks; b++) {
            if (disk[b] == 0) {
                run++;
                if (run == size[i]) {
                    start = b - size[i] + 1;   /* first run of consecutive free blocks */
                    break;
                }
            } else {
                run = 0;
            }
        }

        if (start == -1) {
            printf("%s\t%d\tNot allocated\t-\n", names[i], size[i]);
            continue;
        }

        printf("%s\t%d\t%d\t", names[i], size[i], start);
        for (b = start; b < start + size[i]; b++) {
            printf("%s%d", b == start ? "" : ",", b);
            disk[b] = 1;
        }
        printf("\n");
    }

    for (b = 0; b < blocks; b++) if (disk[b] == 0) freeBlocks++;
    printf("\nFree blocks: %d\n", freeBlocks);
    return 0;
}
