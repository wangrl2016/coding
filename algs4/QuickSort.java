import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

/**
 * Quick sort is a recursive program that sorts a sub-array a[lo...hi] by using
 * a partition() method that puts a[i] into position and arranges the rest of
 * entries such that the recursive calls finish the sort.
 *
 * Quick sort is complementary to merge sort:for merge sort, we break the array into
 * two sub-arrays to be sorted and then combine the ordered sub-arrays to make the
 * whole ordered array; for quick sort, we rearrange the array such that, when the
 * two sub-arrays are sorted, the whole array is ordered.
 *
 * For mergesort, the array is divided in half; for quick sort, the position of the
 * partition depends on the contents of the array.
 *
 * Partition过程
 *
 * The entry a[j] is in its final place in the array, for some j.
 * No entry in a[lo] through a[j-1] is greater than a[j].
 * No entry in a[j+1] through a[hi] is less than a[j].
 *
 * To complete the implementation, we need to implement the partitioning
 * method.
 *
 * We use the following general strategy: First, we arbitrarily
 * choose a[1o] to be the partitioning item - the one that will go into
 * its final position.
 *
 * Next, we scan from the left end of the array until we find an entry
 * greater than (or equal to) the partitioning item, and we scan from the
 * right end of the array until we find an entry less than (or equal to)
 * the partitioning item. The two item that stopped the scans are out of
 * place in the final partitioned array, so we exchanged them.
 *
 * Continuing in this way, we ensure that no array entries to the left of the
 * left index i are greater than the partitioning item, and no array
 * entries to the right of the right index j are less than the partitioning
 * item.
 *
 * When the scan indices cross, all that we need to do to complete the
 * partitioning process is to exchange the partitioning item a[lo] with the
 * right most entry of the left sub-array (a[j] and return its index j.
 */

public class QuickSort {

    // This class should not be instantiated.
    private QuickSort() {
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

    // Quicksort the sub-array from a[lo] to a[hi].
    private static void sort(Comparable[] a, int lo, int hi) {
        if (hi <= lo) return;
        int j = partition(a, lo, hi);
        sort(a, lo, j - 1);
        sort(a, j + 1, hi);
        assert isSorted(a, lo, hi);
    }

    // Partition the sub-array a[lo..hi] so that a[lo..j-1] <= a[j] <= a[j+1..hi]
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
    @SuppressWarnings("unchecked")
    private static boolean less(Comparable v, Comparable w) {
        if (v == w) return false;   // optimization when reference equals
        return v.compareTo(w) < 0;
    }

    // Exchange a[i] and a[j].
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
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
        for (Comparable comparable : a) {
            System.out.print(comparable + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        String[] ss = args[0].split(" ");
        QuickSort.sort(ss);
        show(ss);
    }
}
