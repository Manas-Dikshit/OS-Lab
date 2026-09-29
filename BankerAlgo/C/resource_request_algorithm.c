#include <stdio.h>

int isSafe(int n, int m, int need[][m], int alloc[][m], int avail[], int seq[]) {
    int work[m], finish[n], count = 0, i, j;
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
        if (!found)
            return 0;
    }
    return 1;
}

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

    int p;
    printf("Enter process making request (1-%d): ", n);
    scanf("%d", &p);
    p--;
    int req[m];
    printf("Enter request vector: ");
    for (j = 0; j < m; j++)
        scanf("%d", &req[j]);

    for (j = 0; j < m; j++) {
        if (req[j] > need[p][j]) {
            printf("ERROR: request exceeds maximum claim of P%d. Process must be terminated.\n", p + 1);
            return 0;
        }
    }

    for (j = 0; j < m; j++) {
        if (req[j] > avail[j]) {
            printf("Request of P%d EXCEEDS available resources. Process must wait.\n", p + 1);
            return 0;
        }
    }

    int savedAvail[m];
    int savedAlloc[n][m], savedNeed[n][m];
    for (j = 0; j < m; j++)
        savedAvail[j] = avail[j];
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++) {
            savedAlloc[i][j] = alloc[i][j];
            savedNeed[i][j] = need[i][j];
        }

    for (j = 0; j < m; j++) {
        avail[j] -= req[j];
        alloc[p][j] += req[j];
        need[p][j] -= req[j];
    }

    int seq[n];
    int safe = isSafe(n, m, need, alloc, avail, seq);

    if (safe) {
        printf("Request of P%d is GRANTED. System remains in SAFE state.\n", p + 1);
        printf("Safe sequence: ");
        for (i = 0; i < n; i++)
            printf("P%d%s", seq[i] + 1, i < n - 1 ? " -> " : "\n");
        printf("New Available:\t");
        for (j = 0; j < m; j++)
            printf("%d\t", avail[j]);
        printf("\n");
    } else {
        for (j = 0; j < m; j++) {
            avail[j] = savedAvail[j];
            alloc[p][j] = savedAlloc[p][j];
            need[p][j] = savedNeed[p][j];
        }
        printf("Request of P%d is DENIED. Granting it would leave the system UNSAFE.\n", p + 1);
        printf("State rolled back; process must wait.\n");
    }
    return 0;
}