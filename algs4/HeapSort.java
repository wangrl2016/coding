import edu.princeton.cs.algs4.StdOut;

/**
 * Sort a sequence of strings using heap sort.
 *
 * The HeapSort class provides a static method to sort an array
 * using heapsort.
 *
 * This implementation takes Q(NlogN) time to sort any array of length n
 * (assuming comparisons take constant time). It makes at most
 * 2nlog2n compares.
 *
 * The sorting algorithm is not stable.
 * It uses Q(1) extra memory (not including the input array).
 */
public class HeapSort {

    // This class should not be instantiated.
    private HeapSort() {
    }

    public static void sort(Comparable[] pq) {
        int n = pq.length;

        // heapify phase
        for (int k = n / 2; k >= 1; k--)
            sink(pq, k, n);

        // sort down phase
        int k = n;
        while (k > 1) {
            exch(pq, 1, k--);
            sink(pq, 1, k);
        }
    }

    // base 1 array
    private static void sink(Comparable[] pq, int k, int n) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && less(pq, j, j + 1)) j++;
            if (!less(pq, k, j)) break;
            exch(pq, k, j);
            k = j;
        }
    }

    private static void exch(Object[] pq, int i, int j) {
        Object swap = pq[i - 1];
        pq[i - 1] = pq[j - 1];
        pq[j - 1] = swap;
    }

    // Indices are off-by-one to support 1-based indexing.
    @SuppressWarnings("unchecked")
    private static boolean less(Comparable[] pq, int i, int j) {
        return pq[i - 1].compareTo(pq[j - 1]) < 0;
    }

    private static void show(Comparable[] a) {
        for (Comparable comparable : a) {
            System.out.print(comparable + " ");
        }
        System.out.println();
    }

    public static void main(String[] args) {
        String[] ss = args[0].split(" ");
        HeapSort.sort(ss);
        show(ss);
    }
}
