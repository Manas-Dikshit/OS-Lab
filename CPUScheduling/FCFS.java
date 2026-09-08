import java.util.Scanner;

public class FCFS {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();

        int[] pid = new int[n];
        int[] at = new int[n];
        int[] bt = new int[n];
        int[] ct = new int[n];
        int[] tat = new int[n];
        int[] wt = new int[n];

        for (int i = 0; i < n; i++) {
            pid[i] = i + 1;
            System.out.print("Enter arrival time for P" + (i + 1) + ": ");
            at[i] = sc.nextInt();
            System.out.print("Enter burst time for P" + (i + 1) + ": ");
            bt[i] = sc.nextInt();
        }

        int time = 0;
        for (int i = 0; i < n; i++) {
            if (time < at[i])
                time = at[i];
            ct[i] = time + bt[i];
            time = ct[i];
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
