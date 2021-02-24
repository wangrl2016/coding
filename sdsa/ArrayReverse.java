public class ArrayReverse {

    // Reverse arr[] from index start to end.
    static void reverseArray(int arr[], int start, int end) {
        int temp;
        while (start < end) {
            temp = arr[start];
            arr[start] = arr[end];
            arr[end] = temp;
            start++;
            end--;
        }
    }

    static void leftRotate(int arr[], int d) {
        if (d == 0)
            return;
        // In case the rotating factor is greater than array length.
        d = d % arr.length;
        reverseArray(arr, 0, d - 1);
        reverseArray(arr, d, arr.length - 1);
        reverseArray(arr, 0, arr.length - 1);
    }

    public static void main(String[] args) {
        int arr[] = {1, 2, 3, 4, 5, 6, 7};
        leftRotate(arr, 2);
        for (int e : arr)
            System.out.print(e + " ");
    }
}
