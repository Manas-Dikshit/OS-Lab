#include <stdio.h>
#include <limits.h>

int main(void) {
    int np, n, i, j, chosen, bestGap, allocated = 0, wasted = 0;
    int part[100], proc[100], owner[100];  /* shortcut: max 100 partitions/processes */

    printf("Enter number of partitions: ");
    scanf("%d", &np);
    for (i = 0; i < np; i++) {
        printf("Enter size of partition %d: ", i + 1);
        scanf("%d", &part[i]);
    }

    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        printf("Enter size of process %d: ", i + 1);
        scanf("%d", &proc[i]);
    }

    for (i = 0; i < np; i++) owner[i] = -1;   /* -1 marks a free partition */

    printf("\nProcess\tSize\tPartition\tWasted\n");
    for (i = 0; i < n; i++) {
        chosen = -1;
        bestGap = INT_MAX;
        for (j = 0; j < np; j++) {
            int gap = part[j] - proc[i];
            if (owner[j] == -1 && gap >= 0 && gap < bestGap) {
                bestGap = gap;                 /* smallest leftover that still fits */
                chosen = j;
            }
        }

        if (chosen == -1) {
            printf("%d\t%d\tNot allocated\t-\n", i + 1, proc[i]);
            continue;
        }

        owner[chosen] = i;
        wasted += part[chosen] - proc[i];
        allocated++;
        printf("%d\t%d\t%d\t%d\n", i + 1, proc[i], chosen + 1, part[chosen] - proc[i]);
    }

    printf("\nAllocated processes: %d/%d\n", allocated, n);
    printf("Total internal fragmentation: %d\n", wasted);
    printf("Processes not allocated: %d\n", n - allocated);

    return 0;
}
