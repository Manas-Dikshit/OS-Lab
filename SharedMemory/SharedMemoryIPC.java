package SharedMemory;

import java.io.RandomAccessFile;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Scanner;

public class SharedMemoryIPC {
    private static final int SIZE = 4096;
    private static final int FLAG = 0;     // 0 = empty, 1 = message ready
    private static final int LENGTH = 4;
    private static final int DATA = 8;
    private static final Path FILE =
            Paths.get(System.getProperty("java.io.tmpdir"), "oslab_shared_memory.bin");

    public static void main(String[] args) throws Exception {
        if (args.length > 0 && args[0].equals("child")) {
            child();
        } else {
            parent();
        }
    }

    private static void parent() throws Exception {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter a message to send through shared memory: ");
        String message = sc.nextLine();
        sc.close();

        try (RandomAccessFile file = new RandomAccessFile(FILE.toFile(), "rw")) {
            file.setLength(SIZE);
            MappedByteBuffer mem = file.getChannel().map(FileChannel.MapMode.READ_WRITE, 0, SIZE);
            mem.putInt(FLAG, 0);

            ProcessBuilder pb = new ProcessBuilder(
                    "java", "-cp", System.getProperty("java.class.path"),
                    "SharedMemory.SharedMemoryIPC", "child");
            pb.inheritIO();
            Process child = pb.start();

            byte[] bytes = message.getBytes(StandardCharsets.UTF_8);
            // shortcut: messages longer than the region are truncated, enlarge SIZE to send more
            if (bytes.length > SIZE - DATA) bytes = Arrays.copyOf(bytes, SIZE - DATA);
            mem.putInt(LENGTH, bytes.length);
            mem.position(DATA);
            mem.put(bytes);
            mem.force();
            System.out.println("Parent wrote: " + new String(bytes, StandardCharsets.UTF_8));
            mem.putInt(FLAG, 1);
            mem.force();

            child.waitFor();
        }
    }

    private static void child() throws Exception {
        try (RandomAccessFile file = new RandomAccessFile(FILE.toFile(), "rw")) {
            MappedByteBuffer mem = file.getChannel().map(FileChannel.MapMode.READ_WRITE, 0, SIZE);
            while (mem.getInt(FLAG) == 0) Thread.sleep(10);   // wait for the parent's message
            byte[] bytes = new byte[mem.getInt(LENGTH)];
            mem.position(DATA);
            mem.get(bytes);
            System.out.println("Child read:  " + new String(bytes, StandardCharsets.UTF_8));
            mem.putInt(FLAG, 0);
        }
    }
}
