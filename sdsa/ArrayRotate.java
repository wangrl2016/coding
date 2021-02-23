public class ArrayRotate {
    // 方法一
    public void rotateUsingTempArray(int[] arr, int d) {
        assert d < arr.length;

        // 1. 将前面d个元素存入到临时数组中
        int[] temp = new int[d];
        for (int i = 0; i < d; i++)
            temp[i] = arr[i];

        // 2. 将剩下的元素进行平移
        for (int i = 0; i < arr.length - d; i++)
            arr[i] = arr[i + d];

        // 3. 将临时数组复制到数组中
        for (int i = 0; i < d; i++)
            arr[arr.length - d + i] = temp[i];
    }

    // Left rotate arr[] of size n by d.
    private void leftRotateByOne(int arr[]) {
        int temp = arr[0];
        for (int i = 0; i < arr.length - 1; i++)
            arr[i] = arr[i + 1];
        arr[arr.length - 1] = temp;
    }

    // 方法二
    public void leftRotate(int arr[], int d) {
        for (int i = 0; i < d; i++)
            leftRotateByOne(arr);
    }

    public static void main(String[] args) {
        ArrayRotate rotate = new ArrayRotate();
        int arr[] = {1, 2, 3, 4, 5, 6, 7};

        rotate.rotateUsingTempArray(arr, 2);
        for (int i = 0; i < arr.length; i++)
            System.out.print(arr[i] + " ");

        System.out.println();

        rotate.leftRotate(arr, 2);
        for (int i = 0; i < arr.length; i++)
            System.out.print(arr[i] + " ");
    }
}
