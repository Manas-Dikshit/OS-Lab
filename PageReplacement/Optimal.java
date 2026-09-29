import java.util.Arrays;
import java.util.Scanner;

public class Optimal {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of frames: ");
        int frames = sc.nextInt();
        System.out.print("Enter number of page references: ");
        int n = sc.nextInt();
        int[] ref = new int[n];
        System.out.print("Enter the reference string: ");
        for (int i = 0; i < n; i++) ref[i] = sc.nextInt();

        int[] f = new int[frames];
        Arrays.fill(f, -1);
        int faults = 0;

        String head = "\nPage\t";
        for (int i = 0; i < frames; i++) head += "F" + (i + 1) + "\t";
        System.out.println(head);

        for (int i = 0; i < n; i++) {
            int p = ref[i];
            boolean hit = false;
            for (int v : f) if (v == p) hit = true;

            if (!hit) {
                // Next use of each resident page from i+1 onwards; -1 = never used again
                int[] nextUse = new int[frames];
                for (int j = 0; j < frames; j++) {
                    nextUse[j] = -1;
                    for (int k = i + 1; k < n; k++) {
                        if (f[j] == ref[k]) { nextUse[j] = k; break; }
                    }
                }
                int victim = -1;
                for (int j = 0; j < frames; j++) if (f[j] == -1) { victim = j; break; }  // free slot first
                if (victim < 0) {                      // else evict the farthest use
                    victim = 0;
                    for (int j = 1; j < frames; j++) if (nextUse[j] > nextUse[victim]) victim = j;
                }
                f[victim] = p;
                faults++;
            }

            String row = "";
            for (int v : f) row += v + "\t";
            System.out.println(p + "\t" + row);
        }

        System.out.printf("%nTotal page faults: %d%n", faults);
        System.out.printf("Hit ratio: %.2f%%%n", (n - faults) * 100.0 / n);
        sc.close();
    }
}
