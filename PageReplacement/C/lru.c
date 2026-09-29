#include <stdio.h>

int main() {
    int frames, n, i, j, p, idx, clock = 0, faults = 0;
    int ref[100], f[100], used[100];   /* used[] holds the timestamp of last use */

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of page references: ");
    scanf("%d", &n);
    printf("Enter the reference string: ");
    for (i = 0; i < n; i++) scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++) { f[i] = -1; used[i] = 0; }

    printf("\nPage\t");
    for (j = 0; j < frames; j++) printf("F%d\t", j + 1);
    printf("\n");

    for (i = 0; i < n; i++) {
        p = ref[i];
        idx = -1;
        for (j = 0; j < frames; j++) if (f[j] == p) { idx = j; break; }

        if (idx < 0) {
            idx = 0;                                  /* fallback: first slot */
            for (j = 1; j < frames; j++)               /* empty slots have used == 0 */
                if (used[j] < used[idx]) idx = j;
            f[idx] = p;                                /* evict least recently used */
            faults++;
        }
        used[idx] = clock++;

        printf("%d\t", p);
        for (j = 0; j < frames; j++) printf("%d\t", f[j]);
        printf("\n");
    }

    printf("\nTotal page faults: %d\n", faults);
    printf("Hit ratio: %.2f%%\n", (n - faults) * 100.0 / n);

    return 0;
}
