import java.util.Arrays;
import java.util.Scanner;

public class Clock {
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
        boolean[] refBit = new boolean[frames];   // second-chance bit
        int hand = 0, faults = 0;

        String head = "\nPage\t";
        for (int i = 0; i < frames; i++) head += "F" + (i + 1) + "\t";
        System.out.println(head);

        for (int p : ref) {
            boolean hit = false;
            for (int i = 0; i < frames; i++) {
                if (f[i] == p) { refBit[i] = true; hit = true; break; }
            }

            if (!hit) {
                while (refBit[hand]) {               // give recent pages a second chance
                    refBit[hand] = false;
                    hand = (hand + 1) % frames;
                }
                f[hand] = p;
                refBit[hand] = true;
                faults++;
            }
            hand = (hand + 1) % frames;

            String row = "";
            for (int v : f) row += v + "\t";
            System.out.println(p + "\t" + row);
        }

        System.out.printf("%nTotal page faults: %d%n", faults);
        System.out.printf("Hit ratio: %.2f%%%n", (n - faults) * 100.0 / n);
        sc.close();
    }
}
