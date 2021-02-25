
public class ArraySearch {
    /**
     * 查找分割点下标
     *
     * @param arr  数组
     * @param low  起始
     * @param high 结束
     * @return　查找的下标
     */
    static int findPivot(int arr[], int low, int high) {
        // base case
        if (high < low)
            return -1;
        else if (high == low)
            return low;

        int mid = (low + high) / 2;
        if (mid < high && arr[mid] > arr[mid + 1])
            return mid; // 前一个元素比后一个元素大
        if (mid > low && arr[mid] < arr[mid - 1])
            return mid - 1;

        // 最小比中间的大说明分割点在其中
        if (arr[low] >= arr[mid])
            return findPivot(arr, low, mid - 1);
        return findPivot(arr, mid + 1, high);
    }

    static int binarySearch(int arr[], int low, int high, int key) {
        if (high < low)
            return -1;

        int mid = (low + high) / 2;
        if (key == arr[mid])
            return mid;

        if (key > arr[mid])
            return binarySearch(arr, mid + 1, high, key);
        else
            return binarySearch(arr, low, mid - 1, key);
    }

    static int pivotedBinarySearch(int arr[], int key) {
        int pivot = findPivot(arr, 0, arr.length - 1);
        if (pivot == -1)
            return binarySearch(arr, 0, arr.length - 1, key);

        // If we found a pivot, then first compare with pivot and
        // then search in two subarrays around pivot.
        if (arr[pivot] == key)
            return pivot;

        if (arr[0] <= key)
            return binarySearch(arr, 0, pivot - 1, key);
        else
            return binarySearch(arr, pivot + 1, arr.length - 1, key);
    }

    public static void main(String args[]) {
        int arr[] = {5, 6, 7, 8, 9, 10, 1, 2, 3};
        int key = 3;
        System.out.println("Index of the element is: " +
                pivotedBinarySearch(arr, key));
    }
}
