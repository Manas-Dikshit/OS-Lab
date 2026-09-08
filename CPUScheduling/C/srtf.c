#include <stdio.h>

int main() {
    int n, i;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pid[n], at[n], bt[n], rt[n], ct[n], tat[n], wt[n];

    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &at[i]);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    int time = 0x7fffffff, completed = 0;
    for (i = 0; i < n; i++)
        if (at[i] < time) time = at[i];

    while (completed < n) {
        int idx = -1, min_rt = 0x7fffffff;
        for (i = 0; i < n; i++) {
            if (at[i] <= time && rt[i] > 0 && rt[i] < min_rt) {
                min_rt = rt[i];
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        rt[idx]--;
        if (rt[idx] == 0) {
            ct[idx] = time + 1;
            completed++;
        }
        time++;
    }

    float avg_tat = 0, avg_wt = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        avg_tat += tat[i];
        avg_wt += wt[i];
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }

    printf("\nAverage TAT: %.2f\n", avg_tat / n);
    printf("Average WT:  %.2f\n", avg_wt / n);
    return 0;
}