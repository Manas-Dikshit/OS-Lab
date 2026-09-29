import java.util.Arrays;
import java.util.Scanner;

public class FIFO {
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
        Arrays.fill(f, -1);          // -1 marks an empty frame
        int faults = 0, next = 0;

        String head = "\nPage\t";
        for (int i = 0; i < frames; i++) head += "F" + (i + 1) + "\t";
        System.out.println(head);

        for (int p : ref) {
            boolean hit = false;
            for (int v : f) if (v == p) hit = true;

            if (!hit) {
                f[next] = p;                        // replace the oldest frame
                next = (next + 1) % frames;
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
