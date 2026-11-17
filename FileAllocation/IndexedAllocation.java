package FileAllocation;

import java.util.Scanner;

public class IndexedAllocation {
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

        System.out.println("\nFile\tSize\tIndex\tData blocks");
        for (int i = 0; i < n; i++) {
            int[] chosen = new int[size[i] + 1];  // one index block + data blocks
            int count = 0;
            for (int b = 0; b < blocks && count < chosen.length; b++) {
                if (disk[b] == 0) chosen[count++] = b;
            }

            if (count < chosen.length) {
                System.out.printf("%s\t%d\t-\tNot allocated%n", names[i], size[i]);
                continue;
            }

            int index = chosen[0];
            disk[index] = 1;
            StringBuilder path = new StringBuilder();
            for (int k = 1; k < chosen.length; k++) {
                disk[chosen[k]] = 1;
                if (path.length() > 0) path.append("->");
                path.append(chosen[k]);
            }
            System.out.printf("%s\t%d\t%d\t%s%n", names[i], size[i], index, path);
        }

        int free = 0;
        for (int v : disk) if (v == 0) free++;
        System.out.printf("%nFree blocks: %d%n", free);
        sc.close();
    }
}
