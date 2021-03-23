/**
 * Bubble sort is a simple sorting algorithm that repeatedly steps
 * through the list, compares adjacent elements and swaps them if
 * they are in the wrong order.
 *
 * The pass through the list is repeated until the list is sorted.
 *
 * Bubble sort has a worst-case and average complexity of O(n^2), where
 * n is the number of items being sorted.
 */

public class BubbleSort {
    // This class should not be instantiated.
    private BubbleSort() {
    }

    // -i对算法进行优化
    public static void sort(Comparable[] a) {
        for (int i = 0; i < a.length - 1; i++)
            for (int j = 0; j < a.length - i - 1; j++)
                if (less(a[j + 1], a[j]))
                    exch(a, j, j + 1);
    }

    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }

    // Exchange a[i] and a[j].
    private static void exch(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    public static void print(Object[] a) {
        for (Object o : a) System.out.print(o + " ");
        System.out.println();
    }

    public static void main(String[] args) {
        String[] arr = {"S", "O", "R", "T", "E", "X", "A", "M", "P", "L", "E"};
        BubbleSort.sort(arr);
        BubbleSort.print(arr);
    }
}
