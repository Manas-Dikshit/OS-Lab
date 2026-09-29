import java.util.Arrays;
import java.util.Scanner;

public class LRU {
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
        int[] used = new int[frames];   // timestamp of last use
        int clock = 0, faults = 0;

        String head = "\nPage\t";
        for (int i = 0; i < frames; i++) head += "F" + (i + 1) + "\t";
        System.out.println(head);

        for (int p : ref) {
            int idx = -1;
            for (int i = 0; i < frames; i++) if (f[i] == p) { idx = i; break; }

            if (idx < 0) {
                idx = 0;                                  // fallback: first slot
                for (int i = 1; i < frames; i++)           // empty slots have used == 0
                    if (used[i] < used[idx]) idx = i;
                f[idx] = p;                                // evict least recently used
                faults++;
            }
            used[idx] = clock++;

            String row = "";
            for (int v : f) row += v + "\t";
            System.out.println(p + "\t" + row);
        }

        System.out.printf("%nTotal page faults: %d%n", faults);
        System.out.printf("Hit ratio: %.2f%%%n", (n - faults) * 100.0 / n);
        sc.close();
    }
}
