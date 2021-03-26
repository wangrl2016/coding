/**
 * The algorithm that people often use to sort bridge hands is to consider
 * the cards one at a time, inserting each into its proper place among those
 * already considered (keeping them sorted).
 *
 * In computer implementation, we need to make space to insert the current
 * item by moving larger items one position to the right, before inserting
 * the current item into hte vacated position.
 *
 * Insertion sort uses O(N^2) compares and O(N^2) exchanges to sort a randomly
 * ordered array of length N with distinct keys, on the average.
 */
public class InsertionSort<Key extends Comparable<Key>> {

    // This class should not be instantiated.
    public InsertionSort() {
    }

    /**
     * Rearranges the array in ascending order, using the natural order.
     *
     * @param a the array to be sorted
     */
    public void sort(Key[] a) {
        int n = a.length;
        for (int i = 1; i < n; i++) {
            for (int j = i; j > 0 && less(a[j], a[j - 1]); j--) {
                exch(a, j, j - 1);
            }
            assert isSorted(a, 0, i);
        }
        assert isSorted(a);
    }

    /**
     * Rearranges the sub-array a[lo..hi] in ascending order, using the natural order.
     *
     * @param a  the array to be sorted
     * @param lo left endpoint (inclusive)
     * @param hi right endpoint (exclusive)
     */
    public void sort(Key[] a, int lo, int hi) {
        for (int i = lo + 1; i < hi; i++) {
            for (int j = i; j > lo && less(a[j], a[j - 1]); j--) {
                exch(a, j, j - 1);
            }
        }
        assert isSorted(a, lo, hi);
    }

    // Is v < w ?
    private boolean less(Key v, Key w) {
        return v.compareTo(w) < 0;
    }

    // Exchange a[i] and a[j]
    private void exch(Key[] a, int i, int j) {
        Key swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    // Check if array is sorted - useful for debugging.
    private boolean isSorted(Key[] a) {
        return isSorted(a, 0, a.length);
    }

    // Is the array a[lo..hi] sorted.
    private boolean isSorted(Key[] a, int lo, int hi) {
        for (int i = lo + 1; i < hi; i++)
            if (less(a[i], a[i - 1])) return false;
        return true;
    }

    // Print array to standard output.
    public void show(Key[] a) {
        for (Key key : a) {
            System.out.print(key + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        String[] ss = args[0].split(" ");
        InsertionSort<String> insertionSort = new InsertionSort<>();
        insertionSort.sort(ss);
        insertionSort.sort(ss, 1, 4);
        insertionSort.show(ss);
    }
}
