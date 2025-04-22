import java.util.Scanner;
import java.lang.Math;

public class I {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        long xl = Long.MAX_VALUE, yl = Long.MAX_VALUE, xr = Long.MAX_VALUE, yr = Long.MAX_VALUE;
        for (int i = 0; i < n; i++) {
            long xi = sc.nextLong(), yi = sc.nextLong(), hi = sc.nextLong();
            xl = Math.min(xl, xi - hi);
            xr = Math.max(xr, xi + hi);
            yl = Math.min(yl, yi - hi);
            yr = Math.max(yr, yi + hi);
        }
        System.out.println((xl + xr) / 2 + " " + (yl + yr) / 2 + " " + (Math.max(xr - xl, yr - yl) + 1) / 2);
    }
}
