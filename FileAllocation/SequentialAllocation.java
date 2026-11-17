package FileAllocation;

import java.util.Scanner;

public class SequentialAllocation {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter number of disk blocks: ");
        int blocks = sc.nextInt();
        int[] disk = new int[blocks];            // 0 = free, 1 = used

        System.out.print("Enter number of files: ");
        int n = sc.nextInt();
        String[] names = new String[n];
        int[] size = new int[n];
        for (int i = 0; i < n; i++) {
            System.out.print("Enter name and size for file " + (i + 1) + " (e.g. alpha 4): ");
            names[i] = sc.next();
            size[i] = sc.nextInt();
        }

        System.out.println("\nFile\tSize\tStart\tBlocks");
        for (int i = 0; i < n; i++) {
            int start = -1, run = 0;
            for (int b = 0; b < blocks; b++) {
                if (disk[b] == 0) {
                    run++;
                    if (run == size[i]) {
                        start = b - size[i] + 1;  // first run of consecutive free blocks
                        break;
                    }
                } else {
                    run = 0;
                }
            }

            if (start == -1) {
                System.out.printf("%s\t%d\tNot allocated\t-%n", names[i], size[i]);
                continue;
            }

            StringBuilder list = new StringBuilder();
            for (int b = start; b < start + size[i]; b++) {
                disk[b] = 1;
                if (b > start) list.append(',');
                list.append(b);
            }
            System.out.printf("%s\t%d\t%d\t%s%n", names[i], size[i], start, list);
        }

        int free = 0;
        for (int v : disk) if (v == 0) free++;
        System.out.printf("%nFree blocks: %d%n", free);
        sc.close();
    }
}
