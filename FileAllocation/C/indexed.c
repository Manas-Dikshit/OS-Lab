#include <stdio.h>

int main(void) {
    int blocks, n, i, b, k, count, needed, freeBlocks = 0, size[20], chosen[101];
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

    printf("\nFile\tSize\tIndex\tData blocks\n");
    for (i = 0; i < n; i++) {
        needed = size[i] + 1;                /* one index block + the data blocks */
        count = 0;
        for (b = 0; b < blocks && count < needed; b++) {
            if (disk[b] == 0) chosen[count++] = b;
        }

        if (count < needed) {
            printf("%s\t%d\t-\tNot allocated\n", names[i], size[i]);
            continue;
        }

        disk[chosen[0]] = 1;                 /* the index block */
        printf("%s\t%d\t%d\t", names[i], size[i], chosen[0]);
        for (k = 1; k < needed; k++) {
            printf("%s%d", k == 1 ? "" : "->", chosen[k]);
            disk[chosen[k]] = 1;
        }
        printf("\n");
    }

    for (b = 0; b < blocks; b++) if (disk[b] == 0) freeBlocks++;
    printf("\nFree blocks: %d\n", freeBlocks);
    return 0;
}
