import java.util.Scanner;

class Sem {
    private int value;

    Sem(int value) {
        this.value = value;
    }

    public synchronized void P() {
        while (value <= 0) {
            try {
                wait();
            } catch (InterruptedException e) {
            }
        }
        value--;
    }

    public synchronized void V() {
        value++;
        notify();
    }
}

public class ProducerConsumer {
    static final int SIZE = 5;
    static final int[] buffer = new int[SIZE];
    static int in = 0, out = 0;
    static Sem empty = new Sem(SIZE);
    static Sem full = new Sem(0);
    static Sem mutex = new Sem(1);

    public static void main(String[] args) throws InterruptedException {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter number of items to produce: ");
        final int items = sc.nextInt();
        sc.close();

        Thread producer = new Thread(() -> {
            for (int i = 0; i < items; i++) {
                empty.P();
                mutex.P();
                buffer[in] = i;
                System.out.println("Produced: " + i + " at slot " + in);
                in = (in + 1) % SIZE;
                mutex.V();
                full.V();
            }
        });

        Thread consumer = new Thread(() -> {
            for (int i = 0; i < items; i++) {
                full.P();
                mutex.P();
                int item = buffer[out];
                System.out.println("Consumed: " + item + " from slot " + out);
                out = (out + 1) % SIZE;
                mutex.V();
                empty.V();
            }
        });

        producer.start();
        consumer.start();
        producer.join();
        consumer.join();
    }
}