import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class MultipleDirectories {

    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Usage: java MultipleDirectories <dir1> <dir2> ...");
            return;
        }

        for (String path : args) {
            try (DirectoryStream<Path> stream = Files.newDirectoryStream(Paths.get(path))) {
                System.out.printf("Contents of directory '%s':%n", path);
                for (Path entry : stream) {
                    System.out.println("  " + entry.getFileName());
                }
            } catch (IOException e) {
                System.err.println("opendir/readdir/closedir for '" + path + "': " + e.getMessage());
            }
        }

        System.out.println("Directory listing complete.");
    }
}