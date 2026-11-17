package MemoryAllocation;

import java.util.Arrays;
import java.util.Scanner;

public class WorstFit {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of partitions: ");
        int np = sc.nextInt();
        int[] part = new int[np];
        for (int i = 0; i < np; i++) {
            System.out.print("Enter size of partition " + (i + 1) + ": ");
            part[i] = sc.nextInt();
        }

        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();
        int[] proc = new int[n];
        for (int i = 0; i < n; i++) {
            System.out.print("Enter size of process " + (i + 1) + ": ");
            proc[i] = sc.nextInt();
        }

        int[] owner = new int[np];
        Arrays.fill(owner, -1);          // -1 marks a free partition
        int allocated = 0, wasted = 0;

        System.out.println("\nProcess\tSize\tPartition\tWasted");
        for (int i = 0; i < n; i++) {
            int chosen = -1, worstGap = -1;
            for (int j = 0; j < np; j++) {
                int gap = part[j] - proc[i];
                if (owner[j] == -1 && gap >= 0 && gap > worstGap) {
                    worstGap = gap;             // largest leftover that still fits
                    chosen = j;
                }
            }

            if (chosen == -1) {
                System.out.printf("%d\t%d\tNot allocated\t-%n", i + 1, proc[i]);
                continue;
            }

            owner[chosen] = i;
            int gap = part[chosen] - proc[i];
            wasted += gap;
            allocated++;
            System.out.printf("%d\t%d\t%d\t%d%n", i + 1, proc[i], chosen + 1, gap);
        }

        System.out.printf("%nAllocated processes: %d/%d%n", allocated, n);
        System.out.printf("Total internal fragmentation: %d%n", wasted);
        System.out.printf("Processes not allocated: %d%n", n - allocated);
        sc.close();
    }
}
