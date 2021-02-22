public class ArrayRotate {
    // Left rotate arr[] of size n by d.
    void leftRotateByOne(int arr[]) {
        int temp = arr[0];
        for (int i = 0; i < arr.length - 1; i++)
            arr[i] = arr[i + 1];
        arr[arr.length - 1] = temp;
    }

    void leftRotate(int arr[], int d) {
        for (int i = 0; i < d; i++)
            leftRotateByOne(arr);
    }

    public static void main(String[] args) {
        ArrayRotate rotate = new ArrayRotate();
        int arr[] = {1, 2, 3, 4, 5, 6, 7};
        rotate.leftRotate(arr, 2);
        for (int i = 0; i < arr.length; i++)
            System.out.print(arr[i] + " ");
    }
}
