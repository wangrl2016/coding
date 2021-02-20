import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * 2.3.17 Sentinels. Modify the code in ALGORITHM 2.5 to remove both bounds checks
 * in the inner while loops. The test against the left end of the subarray is redundant since
 * the partition item acts as a sentinels ( v is never less than a[lo]). To enable removal of
 * the other test, put an item whose key is the largest in the whole array into a[length-1]
 * just after the shuffle. This item will never move (except possibly to be swapped with an
 * item having the same key) and will serve as a sentinel in all subarrays involving the end
 * of the array. Note: When sorting interior subarray, the leftmost entry in the subarray
 * to the right serves as a sentinel for the right end of the subarray.
 */
public class QuickWithSentinels {

    // This class should not be instantiated.
    private QuickWithSentinels() {
    }

    public static void sort(Comparable[] a) {
        // Place biggest item on the right end.
        Comparable max = a[0];
        int maxIndex = 0;
        for (int i = 1; i < a.length; i++) {
            if (less(max, a[i])) {
                max = a[i];
                maxIndex = i;
            }
        }
        exch(a, maxIndex, a.length - 1);

        sort(a, 0, a.length - 1);
    }

    private static void sort(Comparable[] a, int lo, int hi) {
        if (hi <= lo) return;

        int j = partition(a, lo, hi);
        sort(a, lo, j - 1);
        sort(a, j + 1, hi);
        assert isSorted(a, lo, hi);
    }

    private static int partition(Comparable[] a, int lo, int hi) {
        int i = lo;
        int j = hi + 1;
        Comparable v = a[lo];
        while (true) {
            // find item on lo to swap
            while (less(a[++i], v)) ;

            // find item on hi to swap
            while (less(v, a[--j])) ;

            // check if pointers cross
            if (i >= j) break;

            exch(a, i, j);
        }

        // put partitioning item v at a[j]
        exch(a, lo, j);

        // now, a[lo..j-1] <= a[j] <= a[j+1..hi]
        return j;
    }

    // Check if array is sorted - useful for debugging.
    private static boolean isSorted(Comparable[] a) {
        return isSorted(a, 0, a.length - 1);
    }

    private static boolean isSorted(Comparable[] a, int lo, int hi) {
        for (int i = lo + 1; i <= hi; i++) {
            if (less(a[i], a[i - 1])) return false;
        }
        return true;
    }

    // Is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        if (v == w) return false;       // optimization when reference equals
        return v.compareTo(w) < 0;
    }

    // Exchange a[i] and a[j].
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    // Print array to standard output.
    private static void show(Comparable[] a) {
        for (int i = 0; i < a.length; i++) {
            StdOut.println(a[i]);
        }
    }

    public static void main(String[] args) {
        String[] a = StdIn.readAllStrings();
        QuickWithSentinels.sort(a);
        show(a);
    }
}
