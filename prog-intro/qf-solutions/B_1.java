import java.util.Scanner;

public class B {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        for (int i = -25000; i < n - 25000; i++) {
            System.out.println(710 * i);
        }
    }
}
