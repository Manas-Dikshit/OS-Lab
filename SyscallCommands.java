import java.io.BufferedReader;
import java.io.InputStreamReader;

public class SyscallCommands {
    public static void main(String[] args) {
        try {
            runCommand("ls", new String[] { "C:\\Program Files\\Git\\bin\\bash.exe", "-c", "ls" });
            runCommand("cp", new String[] { "C:\\Program Files\\Git\\bin\\bash.exe", "-c", "cp SyscallCommands.java newdir/SyscallCommandsCopy.java" });
            runCommand("ls", new String[] { "C:\\Program Files\\Git\\bin\\bash.exe", "-c", "ls newdir" });
            runCommand("grep", new String[] { "C:\\Program Files\\Git\\bin\\bash.exe", "-c", "grep class SyscallCommands.java" });
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.exit(0);
    }

    private static void runCommand(String name, String[] cmd) throws Exception {
        System.out.println("------- " + name + " -------");
        ProcessBuilder pb = new ProcessBuilder(cmd);
        pb.redirectErrorStream(true);
        Process p = pb.start();
        BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
        String line;
        while ((line = reader.readLine()) != null) {
            System.out.println(line);
        }
        int code = p.waitFor();
        System.out.println("Exit code: " + code);
        System.out.println();
    }
}
