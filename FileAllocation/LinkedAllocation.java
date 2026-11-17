package FileAllocation;

import java.util.Scanner;

public class LinkedAllocation {
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

        System.out.println("\nFile\tSize\tChain");
        for (int i = 0; i < n; i++) {
            int[] chain = new int[size[i]];
            int count = 0;
            for (int b = 0; b < blocks && count < size[i]; b++) {
                if (disk[b] == 0) chain[count++] = b;  // any free block will do
            }

            if (count < size[i]) {
                System.out.printf("%s\t%d\tNot allocated%n", names[i], size[i]);
                continue;
            }

            StringBuilder path = new StringBuilder();
            for (int b : chain) {
                disk[b] = 1;
                if (path.length() > 0) path.append("->");
                path.append(b);
            }
            System.out.printf("%s\t%d\t%s%n", names[i], size[i], path);
        }

        int free = 0;
        for (int v : disk) if (v == 0) free++;
        System.out.printf("%nFree blocks: %d%n", free);
        sc.close();
    }
}
