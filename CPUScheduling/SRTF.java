import java.util.Scanner;

public class SRTF {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();

        int[] pid = new int[n];
        int[] at = new int[n];
        int[] bt = new int[n];
        int[] rt = new int[n];
        int[] ct = new int[n];
        int[] tat = new int[n];
        int[] wt = new int[n];

        for (int i = 0; i < n; i++) {
            pid[i] = i + 1;
            System.out.print("Enter arrival time for P" + (i + 1) + ": ");
            at[i] = sc.nextInt();
            System.out.print("Enter burst time for P" + (i + 1) + ": ");
            bt[i] = sc.nextInt();
            rt[i] = bt[i];
        }

        int completed = 0, time = Integer.MAX_VALUE;
        for (int i = 0; i < n; i++)
            if (at[i] < time) time = at[i];

        while (completed < n) {
            int idx = -1, minRt = Integer.MAX_VALUE;
            for (int i = 0; i < n; i++) {
                if (at[i] <= time && rt[i] > 0 && rt[i] < minRt) {
                    minRt = rt[i];
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

        double avgTat = 0, avgWt = 0;
        System.out.println("\nPID\tAT\tBT\tCT\tTAT\tWT");
        for (int i = 0; i < n; i++) {
            tat[i] = ct[i] - at[i];
            wt[i] = tat[i] - bt[i];
            avgTat += tat[i];
            avgWt += wt[i];
            System.out.println("P" + pid[i] + "\t" + at[i] + "\t" + bt[i] + "\t" + ct[i] + "\t" + tat[i] + "\t" + wt[i]);
        }

        System.out.printf("\nAverage TAT: %.2f\n", avgTat / n);
        System.out.printf("Average WT:  %.2f\n", avgWt / n);
        sc.close();
    }
}
