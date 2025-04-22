import java.util.Scanner;
import java.lang.Math;

public class I {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int xl = Integer.MAX_VALUE, yl = Integer.MAX_VALUE, xr = Integer.MAX_VALUE, yr = Integer.MAX_VALUE;
        for (int i = 0; i < n; i++) {
            int xi = sc.nextInt(), yi = sc.nextInt(), hi = sc.nextInt();
            xl = Math.min(xl, xi - hi);
            xr = Math.max(xr, xi + hi);
            yl = Math.min(yl, yi - hi);
            yr = Math.max(yr, yi + hi);
        }
        System.out.println((xl + xr) / 2 + " " + (yl + yr) / 2 + " " + (Math.max(xr - xl, yr - yl) + 1) / 2);
    }
}
