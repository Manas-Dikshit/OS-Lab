#include <stdio.h>

int main() {
    int n, m, i, j;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    int avail[m];
    printf("Enter available instances of each resource: ");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    int max[n][m], alloc[n][m], need[n][m];
    printf("Enter max demand matrix (Max):\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    printf("Enter allocation matrix (Allocation):\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    printf("\nNeed matrix (Max - Allocation):\n");
    printf("Process\t");
    for (j = 0; j < m; j++)
        printf("R%d\t", j + 1);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("P%d\t", i + 1);
        for (j = 0; j < m; j++)
            printf("%d\t", need[i][j]);
        printf("\n");
    }

    int work[m], finish[n], seq[n], count = 0, safe = 1;
    for (j = 0; j < m; j++)
        work[j] = avail[j];
    for (i = 0; i < n; i++)
        finish[i] = 0;

    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i])
                continue;
            int ok = 1;
            for (j = 0; j < m; j++)
                if (need[i][j] > work[j]) {
                    ok = 0;
                    break;
                }
            if (ok) {
                for (j = 0; j < m; j++)
                    work[j] += alloc[i][j];
                finish[i] = 1;
                seq[count++] = i;
                found = 1;
                break;
            }
        }
        if (!found) {
            safe = 0;
            break;
        }
    }

    printf("\nAvailable vector:\t");
    for (j = 0; j < m; j++)
        printf("%d\t", avail[j]);
    printf("\nWork vector at end:\t");
    for (j = 0; j < m; j++)
        printf("%d\t", work[j]);
    printf("\n");

    if (safe) {
        printf("\nSystem is in a SAFE state. Safe sequence: ");
        for (int k = 0; k < n; k++)
            printf("P%d%s", seq[k] + 1, k < n - 1 ? " -> " : "\n");
    } else {
        printf("\nSystem is in an UNSAFE state. No safe sequence exists.\n");
    }
    return 0;
}