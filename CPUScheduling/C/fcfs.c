#include <stdio.h>

int main() {
    int n, i, j;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], ct[n], tat[n], wt[n];

    // Input process details
    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &at[i]);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    // Sort processes by arrival time (FCFS requirement)
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (at[i] > at[j]) {
                // Swap arrival times
                int temp = at[i]; at[i] = at[j]; at[j] = temp;
                // Swap burst times
                temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
                // Swap process IDs
                temp = pid[i]; pid[i] = pid[j]; pid[j] = temp;
            }
        }
    }

    // Calculate completion times
    int time = 0;
    for (i = 0; i < n; i++) {
        if (time < at[i])
            time = at[i];  // CPU waits if process hasn't arrived
        ct[i] = time + bt[i];
        time = ct[i];
    }

    // Calculate TAT and WT
    float avg_tat = 0, avg_wt = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        avg_tat += tat[i];
        avg_wt += wt[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    // Print averages
    printf("\nAverage TAT: %.2f\n", avg_tat / n);
    printf("Average WT:  %.2f\n", avg_wt / n);

    return 0;
}
