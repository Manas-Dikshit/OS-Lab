#include <stdio.h>

int main(void) {
    int blocks, n, i, b, k, count, freeBlocks = 0, size[20], chain[100];
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

    printf("\nFile\tSize\tChain\n");
    for (i = 0; i < n; i++) {
        count = 0;
        for (b = 0; b < blocks && count < size[i]; b++) {
            if (disk[b] == 0) chain[count++] = b;   /* any free block will do */
        }

        if (count < size[i]) {
            printf("%s\t%d\tNot allocated\n", names[i], size[i]);
            continue;
        }

        printf("%s\t%d\t", names[i], size[i]);
        for (k = 0; k < count; k++) {
            printf("%s%d", k == 0 ? "" : "->", chain[k]);
            disk[chain[k]] = 1;
        }
        printf("\n");
    }

    for (b = 0; b < blocks; b++) if (disk[b] == 0) freeBlocks++;
    printf("\nFree blocks: %d\n", freeBlocks);
    return 0;
}
