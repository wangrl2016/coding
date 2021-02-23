/**
 * 求解最大公约数
 */
public class GCD {
    static int gcd(int a, int b) {
        assert a > b;
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }

    static int gcd(int a, int b, int c) {
        return gcd(a, gcd(b, c));
    }

    public static void main(String[] args) {
        int a = 270, b = 192;
        System.out.println("The GCD of " +
                a + " and " + b + " is " + gcd(a, b));

        int c = 150, d = 30, e = 12;
        System.out.println("The GCD of " +
                c + ", " + d + " and " + e + " is "
                + gcd(c, d, e));
    }
}
