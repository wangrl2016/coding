import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * 2.3.25 Cutoff to insertion sort. Implement quicksort with a cutoff to insertion sort
 * for subarrays with less than M elements, and empirically determine the value of M for
 * which quicksort runs fastest in your computing environment to sort random arrays
 * of N doubles, for N = 10^3, 10^4, 10^5, and 10^6. Plot average running times for M from
 * 0 to 30 for each value of M. Note: You need to add a three-argument sort() method to
 * ALGORITHM 2.2 for sorting subarrays such that the call Insertion.sort(a, lo, hi)
 * sorts the subarray a[lo..hi].
 */
public class QuickWithCutoff {

    private static final int M = 4;

    // This class should not be instantiated.
    private QuickWithCutoff() {
    }

    /**
     * Rearranges the array in ascending order, using the natural order.
     *
     * @param a the array to be sorted
     */
    public static void sort(Comparable[] a) {
        sort(a, 0, a.length - 1);
        assert isSorted(a);
    }

    // Quicksort the subarray from a[lo] to a[hi].
    private static void sort(Comparable[] a, int lo, int hi) {
        if (hi <= lo + M) {
            InsertionSort.sort(a, lo, hi);
            return;
        }

        int j = partition(a, lo, hi);
        sort(a, lo, j - 1);
        sort(a, j + 1, hi);
        assert isSorted(a, lo, hi);
    }

    // Partition the subarray a[lo..hi] so that a[lo..j-1] <= a[j] <= a[j+1..hi]
    // and return the index j.
    private static int partition(Comparable[] a, int lo, int hi) {
        int i = lo;
        int j = hi + 1;
        Comparable v = a[lo];
        while (true) {
            // find item on lo to swap
            while (less(a[++i], v)) {
                if (i == hi) break;
            }

            // find item on hi to swap
            while (less(v, a[--j])) {
                if (j == lo) break;     // redundant since a[lo] acts as sentinel
            }

            // check if pointers cross
            if (i >= j) break;

            exch(a, i, j);
        }

        // put partitioning item v at a[j]
        exch(a, lo, j);

        // now, a[lo..j-1] <= a[j] <= a[j+1..hi]
        return j;
    }

    // Is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        if (v == w) return false;   // optimization when reference equals
        return v.compareTo(w) < 0;
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

    // Print array to standard output.
    private static void show(Comparable[] a) {
        for (int i = 0; i < a.length; i++) {
            StdOut.println(a[i]);
        }
    }

    // Exchange a[i] and a[j].
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    public static void main(String[] args) {
        String[] a = StdIn.readAllStrings();
        QuickWithCutoff.sort(a);
        show(a);
    }
}
