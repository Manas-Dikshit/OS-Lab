#include <stdio.h>

int main() {
    int frames, n, i, j, k, p, hit, faults = 0;
    int ref[100], f[100], nextUse[100];

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of page references: ");
    scanf("%d", &n);
    printf("Enter the reference string: ");
    for (i = 0; i < n; i++) scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++) f[i] = -1;

    printf("\nPage\t");
    for (j = 0; j < frames; j++) printf("F%d\t", j + 1);
    printf("\n");

    for (i = 0; i < n; i++) {
        p = ref[i];
        hit = 0;
        for (j = 0; j < frames; j++) if (f[j] == p) hit = 1;

        if (!hit) {
            /* Next use of each resident page from i+1 onwards; -1 = never used again */
            for (j = 0; j < frames; j++) {
                nextUse[j] = -1;
                for (k = i + 1; k < n; k++)
                    if (f[j] == ref[k]) { nextUse[j] = k; break; }
            }
            int victim = -1;
            for (j = 0; j < frames; j++)
                if (f[j] == -1) { victim = j; break; }   /* free slot first */
            if (victim < 0) {                              /* else evict the farthest use */
                victim = 0;
                for (j = 1; j < frames; j++)
                    if (nextUse[j] > nextUse[victim]) victim = j;
            }
            f[victim] = p;
            faults++;
        }

        printf("%d\t", p);
        for (j = 0; j < frames; j++) printf("%d\t", f[j]);
        printf("\n");
    }

    printf("\nTotal page faults: %d\n", faults);
    printf("Hit ratio: %.2f%%\n", (n - faults) * 100.0 / n);

    return 0;
}
