import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class DirectorySetup {

    public static void main(String[] args) {
        String path = args.length > 0 ? args[0] : ".";

        try (DirectoryStream<Path> stream = Files.newDirectoryStream(Paths.get(path))) {
            System.out.printf("Contents of directory '%s':%n", path);
            for (Path entry : stream) {
                System.out.println("  " + entry.getFileName());
            }
        } catch (IOException e) {
            System.err.println("opendir/readdir/closedir: " + e.getMessage());
        }

        System.out.println("Directory listing complete.");
    }
}
