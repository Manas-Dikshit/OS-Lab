import java.util.Arrays;
import java.util.Scanner;

public class ResourceRequestAlgorithm {

    static boolean isSafe(int n, int m, int[][] need, int[][] alloc, int[] avail, int[] seq) {
        int[] work = new int[m];
        boolean[] finish = new boolean[n];
        int count = 0;
        for (int j = 0; j < m; j++)
            work[j] = avail[j];
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
            if (!found) return false;
        }
        return true;
    }

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

        System.out.print("\nEnter process making request (1-" + n + "): ");
        int p = sc.nextInt() - 1;
        System.out.print("Enter request vector: ");
        int[] req = new int[m];
        for (int j = 0; j < m; j++)
            req[j] = sc.nextInt();

        boolean exceedsNeed = false;
        for (int j = 0; j < m; j++)
            if (req[j] > need[p][j]) { exceedsNeed = true; break; }

        if (exceedsNeed) {
            System.out.println("ERROR: request exceeds maximum claim of P" + (p + 1) + ". Process must be terminated.");
            sc.close();
            return;
        }

        boolean mustWait = false;
        for (int j = 0; j < m; j++)
            if (req[j] > avail[j]) { mustWait = true; break; }

        if (mustWait) {
            System.out.println("Request of P" + (p + 1) + " EXCEEDS available resources. Process must wait.");
            sc.close();
            return;
        }

        int[] savedAvail = Arrays.copyOf(avail, m);
        int[][] savedAlloc = new int[n][m];
        int[][] savedNeed = new int[n][m];
        for (int i = 0; i < n; i++) {
            savedAlloc[i] = Arrays.copyOf(alloc[i], m);
            savedNeed[i] = Arrays.copyOf(need[i], m);
        }

        for (int j = 0; j < m; j++) {
            avail[j] -= req[j];
            alloc[p][j] += req[j];
            need[p][j] -= req[j];
        }

        int[] seq = new int[n];
        boolean safe = isSafe(n, m, need, alloc, avail, seq);

        if (safe) {
            System.out.println("Request of P" + (p + 1) + " is GRANTED. System remains in SAFE state.");
            System.out.print("Safe sequence: ");
            for (int k = 0; k < n; k++)
                System.out.print("P" + (seq[k] + 1) + (k < n - 1 ? " -> " : "\n"));
            System.out.print("\nNew Available:\t");
            for (int j = 0; j < m; j++)
                System.out.print(avail[j] + "\t");
            System.out.println();
        } else {
            for (int j = 0; j < m; j++) {
                avail[j] = savedAvail[j];
                alloc[p][j] = savedAlloc[p][j];
                need[p][j] = savedNeed[p][j];
            }
            System.out.println("Request of P" + (p + 1) + " is DENIED. Granting it would leave the system UNSAFE.\nState rolled back; process must wait.");
        }
        sc.close();
    }
}