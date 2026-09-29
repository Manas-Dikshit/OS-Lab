import java.util.Arrays;
import java.util.Scanner;

public class SafetyAlgorithm {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of processes: ");
        int n = sc.nextInt();
        System.out.print("Enter number of resource types: ");
        int m = sc.nextInt();

        int[] avail = new int[m];
        System.out.print("Enter available instances of each resource: ");
        for (int j = 0; j < m; j++)
            avail[j] = sc.nextInt();

        int[][] max = new int[n][m];
        System.out.println("Enter max demand matrix (Max):");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                max[i][j] = sc.nextInt();

        int[][] alloc = new int[n][m];
        System.out.println("Enter allocation matrix (Allocation):");
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                alloc[i][j] = sc.nextInt();

        int[][] need = new int[n][m];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                need[i][j] = max[i][j] - alloc[i][j];

        System.out.print("\nNeed matrix (Max - Allocation):\nProcess\t");
        for (int j = 0; j < m; j++)
            System.out.print("R" + (j + 1) + "\t");
        System.out.println();
        for (int i = 0; i < n; i++) {
            System.out.print("P" + (i + 1) + "\t");
            for (int j = 0; j < m; j++)
                System.out.print(need[i][j] + "\t");
            System.out.println();
        }

        int[] work = Arrays.copyOf(avail, m);
        boolean[] finish = new boolean[n];
        int[] seq = new int[n];
        int count = 0;
        boolean safe = true;

        while (count < n) {
            boolean found = false;
            for (int i = 0; i < n; i++) {
                if (finish[i]) continue;
                boolean ok = true;
                for (int j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { ok = false; break; }
                if (ok) {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    finish[i] = true;
                    seq[count++] = i;
                    found = true;
                    break;
                }
            }
            if (!found) { safe = false; break; }
        }

        System.out.print("\nAvailable vector:\t");
        for (int j = 0; j < m; j++)
            System.out.print(avail[j] + "\t");
        System.out.println("\nWork vector at end:\t");
        for (int j = 0; j < m; j++)
            System.out.print(work[j] + "\t");

        System.out.println();
        if (safe) {
            System.out.print("\nSystem is in a SAFE state. Safe sequence: ");
            for (int k = 0; k < n; k++)
                System.out.print("P" + (seq[k] + 1) + (k < n - 1 ? " -> " : "\n"));
        } else {
            System.out.println("\nSystem is in an UNSAFE state. No safe sequence exists.");
        }
        sc.close();
    }
}