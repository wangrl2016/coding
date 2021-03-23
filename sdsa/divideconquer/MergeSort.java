package divideconquer;

/**
 * 参考链接
 * <p>
 * https://www.geeksforgeeks.org/merge-sort/
 * <p>
 * Merge sort is a divide and conquer algorithm. It devices the input array
 * into two halves, calls itself for the two halves, and then merges the two
 * sorted halves.
 * <p>
 * The merge(arr, l, m, r) is a key process that assumes that arr[l..m] and
 * arr[m+1..r] are sorted and merges the two sorted sub-arrays into one.
 */

public class MergeSort {

    // Merges two sub arrays of arr[].
    // First sub array is arr[l..m].
    // Second sub array is arr[m+1..r]
    static void merge(int arr[], int l, int m, int r) {
        // Find sizes of two sub arrays to be merged.
        int n1 = m - l + 1;
        int n2 = r - m;

        // Create temp arrays.
        int L[] = new int[n1];
        int R[] = new int[n2];

        // Copy data to temp arrays.
        for (int i = 0; i < n1; i++)
            L[i] = arr[l + i];
        for (int i = 0; i < n2; i++)
            R[i] = arr[m + 1 + i];

        // Initial indexes of first and second sub arrays.
        int i = 0, j = 0;

        // Initial index of merged sub array.
        int k = l;

        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy remaining elements of L[] if any.
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        // Copy remaining elements of R[] if any.
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    static void sort(int arr[], int l, int r) {
        if (l < r) {
            int m = (l + r) / 2;

            // Sort first and second halves.
            sort(arr, l, m);
            sort(arr, m + 1, r);

            // Merge the sorted halves.
            merge(arr, l, m, r);
        }
    }

    static void printArray(int arr[]) {
        int n = arr.length;
        for (int i = 0; i < n; i++)
            System.out.print(arr[i] + " ");
        System.out.println();
    }

    public static void main(String args[]) {
        int arr[] = {12, 11, 13, 5, 6, 7};

        System.out.print("Given array ");
        printArray(arr);

        MergeSort.sort(arr, 0, arr.length - 1);
        System.out.print("Sorted array ");
        printArray(arr);
    }
}
