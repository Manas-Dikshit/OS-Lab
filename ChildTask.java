public class ChildTask {
    public static void main(String[] args) {
        System.out.println("Child process PID: " + ProcessHandle.current().pid());
        if (args.length > 0) {
            System.out.println("Child received: " + args[0]);
        }
        System.exit(0);
    }
}
