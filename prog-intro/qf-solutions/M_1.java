import java.util.*;

public class M {
    public static void main(String[] agrs) {
        Scanner in = new Scanner(System.in);
        int t = in.nextInt();
        for (int q = 0; q < t; q++) {
            int n = in.nextInt();
            int[] a = new int[n];
            for (int i = 0; i < n; i++) {
                a[i] = in.nextInt();
            }
            Map<Integer, Integer> map = new HashMap<>();
            int ans = 0;
            for (int j = n - 1; j >= 0; j--) {
                for (int i = 0; i < j; i++) {
                    ans += map.getOrDefault(2 * a[j] - a[i], 0);
                }
                map.merge(a[j], 1, Integer::sum);
            }
            map.clear();
            System.out.println(ans);
        }
        in.close();
    }
}
