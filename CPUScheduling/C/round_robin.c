#include <stdio.h>

#define MAX 100

int main() {
    int n, i, tq;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter time quantum: ");
    scanf("%d", &tq);

    int pid[n], at[n], bt[n], rt[n], ct[n], tat[n], wt[n];

    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("Enter arrival time for P%d: ", i + 1);
        scanf("%d", &at[i]);
        printf("Enter burst time for P%d: ", i + 1);
        scanf("%d", &bt[i]);
        rt[i] = bt[i];
    }

    int q[MAX], head = 0, tail = 0;
    int in_queue[n], completed = 0, time = 0;

    for (i = 0; i < n; i++) {
        in_queue[i] = 0;
        if (at[i] == 0) {
            q[tail++] = i;
            in_queue[i] = 1;
        }
    }

    while (completed < n) {
        if (head == tail) {
            time++;
            for (i = 0; i < n; i++)
                if (!in_queue[i] && at[i] <= time) {
                    q[tail++] = i;
                    in_queue[i] = 1;
                }
            continue;
        }

        int idx = q[head++];
        int exec = rt[idx] < tq ? rt[idx] : tq;
        rt[idx] -= exec;
        time += exec;

        for (i = 0; i < n; i++)
            if (!in_queue[i] && at[i] <= time) {
                q[tail++] = i;
                in_queue[i] = 1;
            }

        if (rt[idx] > 0) {
            q[tail++] = idx;
        } else {
            ct[idx] = time;
            completed++;
        }
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