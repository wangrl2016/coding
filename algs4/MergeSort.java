/**
 * The MergeSort class provides methods for sorting an array using
 * a top-down, recursive version of merge sort.
 *
 * This implementation take o(NlogN) time to sort any array of length n
 * (assuming comparisons take constant time.
 */
public class MergeSort {

    // This class should not be instantiated.
    private MergeSort() {
    }

    // Merge sort a[lo..hi] using auxiliary array aux[lo..hi].
    public static void sort(Comparable[] a) {
        Comparable[] aux = new Comparable[a.length];
        sort(a, aux, 0, a.length - 1);
        assert isSorted(a);
    }

    // Merge sort a[lo..hi] using auxiliary array aux[lo..hi].
    private static void sort(Comparable[] a, Comparable[] aux, int lo, int hi) {
        if (hi <= lo) return;
        int mid = lo + (hi - lo) / 2;
        sort(a, aux, lo, mid);
        sort(a, aux, mid + 1, hi);
        merge(a, aux, lo, mid, hi);
    }

    // Stably merge a[lo..mid] with a[mid+1..hi] using aux[lo..hi].
    private static void merge(Comparable[] a, Comparable[] aux, int lo, int mid, int hi) {
        // Precondition: a[lo..mid] and a[mid+1..hi] are sorted subarrays.
        assert isSorted(a, lo, mid);
        assert isSorted(a, mid + 1, hi);

        // copy to aux[]
        if (hi + 1 - lo >= 0) System.arraycopy(a, lo, aux, lo, hi + 1 - lo);

        // merge back to a[]
        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid) a[k] = aux[j++];
            else if (j > hi) a[k] = aux[i++];
            else if (less(aux[j], aux[i])) a[k] = aux[j++];
            else a[k] = aux[i++];
        }
        // Post-condition: a[lo..hi] is sorted.
        assert isSorted(a, lo, hi);
    }

    // Is v < w ?
    @SuppressWarnings("unchecked")
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }

    // Check if array is sorted - useful for debugging.
    private static boolean isSorted(Comparable[] a) {
        return isSorted(a, 0, a.length - 1);
    }

    private static boolean isSorted(Comparable[] a, int lo, int hi) {
        for (int i = lo + 1; i <= hi; i++)
            if (less(a[i], a[i - 1])) return false;
        return true;
    }

    // Print array to standard output.
    private static void show(Comparable[] a) {
        for (int i = 0; i < a.length; i++) {
            System.out.print(a[i] + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        String[] ss = args[0].split(" ");
        MergeSort.sort(ss);
        show(ss);
    }
}
