#include <stdio.h>

int main() {
    int frames, n, i, j, p, hit, hand = 0, faults = 0;
    int ref[100], f[100], refBit[100];   /* refBit[] is the second-chance bit */

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    printf("Enter number of page references: ");
    scanf("%d", &n);
    printf("Enter the reference string: ");
    for (i = 0; i < n; i++) scanf("%d", &ref[i]);

    for (i = 0; i < frames; i++) { f[i] = -1; refBit[i] = 0; }

    printf("\nPage\t");
    for (j = 0; j < frames; j++) printf("F%d\t", j + 1);
    printf("\n");

    for (i = 0; i < n; i++) {
        p = ref[i];
        hit = 0;
        for (j = 0; j < frames; j++)
            if (f[j] == p) { refBit[j] = 1; hit = 1; break; }

        if (!hit) {
            while (refBit[hand]) {                  /* give recent pages a second chance */
                refBit[hand] = 0;
                hand = (hand + 1) % frames;
            }
            f[hand] = p;
            refBit[hand] = 1;
            faults++;
        }
        hand = (hand + 1) % frames;

        printf("%d\t", p);
        for (j = 0; j < frames; j++) printf("%d\t", f[j]);
        printf("\n");
    }

    printf("\nTotal page faults: %d\n", faults);
    printf("Hit ratio: %.2f%%\n", (n - faults) * 100.0 / n);

    return 0;
}
