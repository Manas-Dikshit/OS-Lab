#include <stdio.h>

int main() {
    int frames, n, i, j, p, hit, faults = 0, next = 0, ref[100], f[100];

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of page references: ");
    scanf("%d", &n);
    printf("Enter the reference string: ");
    for (i = 0; i < n; i++) scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++) f[i] = -1;   /* -1 marks an empty frame */

    printf("\nPage\t");
    for (j = 0; j < frames; j++) printf("F%d\t", j + 1);
    printf("\n");

    for (i = 0; i < n; i++) {
        p = ref[i];
        hit = 0;
        for (j = 0; j < frames; j++) if (f[j] == p) hit = 1;

        if (!hit) {
            f[next] = p;                        /* replace the oldest frame */
            next = (next + 1) % frames;
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
