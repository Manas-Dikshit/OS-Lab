public class ProcessSyscalls {
    public static void main(String[] args) {
        System.out.println("Parent process PID: " + ProcessHandle.current().pid());

        try {
            ProcessBuilder pb = new ProcessBuilder(
                "java", "-cp", ".", "ChildTask",
                "Hello from child process"
            );
            Process child = pb.start();
            System.out.println("Child process PID: " + child.pid());

            child.waitFor();
            System.out.println("Child exited with code: " + child.exitValue());
        } catch (Exception e) {
            e.printStackTrace();
        }

        System.exit(0);
    }
}
