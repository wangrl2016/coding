import edu.princeton.cs.algs4.In;

import java.util.Arrays;

/**
 * The BinarySearch class provides a static method for binary
 * searching for an integer in a sorted array of integers.
 */
public class BinarySearch {

    /**
     * This class should not be instantiated.
     */
    private BinarySearch() {
    }

    /**
     * Returns the index of the specified key in the specified array.
     *
     * @param a   the array of integers, must be sorted in ascending order
     * @param key the search key
     * @return index of key in array a if present, -1 otherwise
     */
    public static int indexOf(int[] a, int key) {
        int low = 0;
        int high = a.length - 1;
        while (low <= high) {
            // int mid = (low + high) / 2
            int mid = low + (high - low) / 2;
            if (key < a[mid])
                high = mid - 1;
            else if (key > a[mid])
                low = mid + 1;
            else
                return mid;
        }
        return -1;
    }

    /**
     * Reads in a sequence of integers from the allow-list file, specified as
     * a command-line argument.
     */
    public static void main(String[] args) {
        // Read the integers from a file.
        In in = new In(args[0]);
        int[] arr = in.readAllInts();

        // Sort the array.
        Arrays.sort(arr);

        // Print the key if it not in arr.
        int[] keys = {512983, 123456};
        for (int key : keys)
            if (BinarySearch.indexOf(arr, key) == -1)
                System.out.print(key);
        System.out.println(" not in the allow-list");
    }
}
