import java.util.Arrays;
import java.util.Scanner;

public class LFU {
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
        int[] count = new int[frames];   // references served by each frame
        int[] used = new int[frames];    // timestamp, breaks count ties (oldest wins)
        int clock = 0, faults = 0;

        String head = "\nPage\t";
        for (int i = 0; i < frames; i++) head += "F" + (i + 1) + "\t";
        System.out.println(head);

        for (int p : ref) {
            int idx = -1;
            for (int i = 0; i < frames; i++) if (f[i] == p) { idx = i; break; }

            if (idx < 0) {
                idx = 0;                                  // empty slot
                for (int i = 1; i < frames; i++) {
                    if (count[i] < count[idx]) idx = i;
                    else if (count[i] == count[idx] && used[i] < used[idx]) idx = i;
                }
                f[idx] = p;
                faults++;
            }
            count[idx]++;
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
