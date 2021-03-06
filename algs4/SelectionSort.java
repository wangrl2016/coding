/**
 * First, find the smallest item in the array and exchange it with the first entry (itself if
 * the first entry is already the smallest). Then, find the next smallest item and exchange
 * it with the second entry. Continue in this way until the entire array is sorted.
 *
 * This method is called selection sort because it works by repeatedly selecting the smallest
 * remaining item.
 *
 * Proposition: Selection sort uses ~N^2 / 2 compares and N exchanges to sort an array of
 * length N.
 *
 * Proof: More precisely, examination of the code reveals that, for each i from 0 to N - 1, there
 * is one exchange and N - 1 - i compares, so the total are N exchanges and
 * (N - 1) + (N - 2) + ... + 2 + 1 + 0 = N(N - 1) / 2 ~ N^2 / 2 compares.
 */

public class SelectionSort<Key extends Comparable<Key>> {

    // This class should not be instantiated.
    private SelectionSort() {
    }

    /**
     * Rearranges the array in ascending order, using the natural order.
     *
     * @param a the array to be sorted
     */
    public void sort(Key[] a) {
        int n = a.length;
        for (int i = 0; i < n; i++) {
            int min = i;
            for (int j = i + 1; j < n; j++) {
                if (less(a[j], a[min])) min = j;
            }
            exch(a, i, min);
            assert isSorted(a, 0, i);
        }
        assert isSorted(a);
    }

    // Is v < w ?
    private boolean less(Key v, Key w) {
        return v.compareTo(w) < 0;
    }

    // Exchange a[i] and a[j].
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    private boolean isSorted(Key[] a) {
        return isSorted(a, 0, a.length - 1);
    }

    private boolean isSorted(Key[] a, int lo, int hi) {
        for (int i = lo + 1; i <= hi; i++) {
            if (less(a[i], a[i - 1])) return false;
        }
        return true;
    }

    // Print array to standard output.
    private void show(Key[] a) {
        for (Key key : a) {
            System.out.print(key + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        SelectionSort<String> selectionSort = new SelectionSort<>();
        String[] ss = args[0].split(" ");
        selectionSort.sort(ss);
        selectionSort.show(ss);
    }
}
