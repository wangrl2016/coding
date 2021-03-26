/**
 * Sorts a sequence of strings using shell sort.
 *
 * Insertion sort is slow for large unordered arrays because the only exchanges it does
 * involve adjacent entries, so items can move through the array only one place at a time.
 * For example, if the item with the smallest key happens to be at the end of the array,
 * N - 1 exchanges are needed to get that one item where it belongs.
 *
 * Shell sort is a simple extension of insertion sort that gain speed by allowing exchanges
 * of array entries that are far apart, to produce partially partially sorted arrays
 * that can be efficiently sorted, eventually by insertion sort.
 *
 * The idea is to rearrange the array to give it the property that taking every hth entry
 * (starting anywhere) yields a sorted subsequence.
 *
 * One way to implement shell sort would be, for each h, to use insertion sort independently
 * on each of the h subsequences.
 *
 * The ShellSort class provides static methods for sorting an array using Shell sort with
 * Knuth's increment sequence(1, 4, 13, 40,...).
 */
public class ShellSort {

    // This class should not be instantiated.
    private ShellSort() {
    }

    /**
     * Rearrange the array in ascending order, using the natural order.
     *
     * @param a the array to be sorted
     */
    public static void sort(Comparable[] a) {
        int n = a.length;

        // 3x+1 increment sequence: 1, 4, 13, 40, 121, 364, 1093, ...
        int h = 1;
        while (h < n / 3) h = 3 * h + 1;

        while (h >= 1) {
            // h-sort the array
            for (int i = h; i < n; i++) {
                for (int j = i; j >= h && less(a[j], a[j - h]); j -= h) {
                    exch(a, j, j - h);
                }
            }
            assert isHSorted(a, h);
            h /= 3;
        }
        assert isSorted(a);
    }

    private static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; i++)
            if (less(a[i], a[i - 1]))
                return false;
        return true;
    }

    private static boolean isHSorted(Comparable[] a, int h) {
        for (int i = h; i < a.length; i++)
            if (less(a[i], a[i - h]))
                return false;
        return true;
    }

    // Is v < w ?
    @SuppressWarnings("unchecked")
    private static boolean less(Comparable v, Comparable w) {
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
        for (Comparable comparable : a) {
            System.out.print(comparable + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        String[] ss = args[0].split(" ");
        ShellSort.sort(ss);
        show(ss);
    }
}
