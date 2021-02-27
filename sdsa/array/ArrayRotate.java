package array;

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

    int gcd(int a, int b) {
        if (b == 0)
            return a;
        else
            return gcd(b, a % b);
    }

    // 方法三
    public void jugglingRotate(int arr[], int d) {
        // To handle if d >= n
        d = d % arr.length;

        int gcd = gcd(arr.length, d);

        int temp;   // 存放临时数据
        int j;
        for (int i = 0; i < gcd; i++) {
            // Move i-th values of blocks.
            temp = arr[i];
            j = i;

            while (true) {
                int k = j + d;  // 移动数据的位置
                if (k >= arr.length)
                    k = k - arr.length; // 超过数组边界
                if (k == i)
                    break;  // 循环到开始部分
                arr[j] = arr[k];
                j = k;
            }
            arr[j] = temp;
        }
    }

    public static void main(String[] args) {
        ArrayRotate rotate = new ArrayRotate();

        int a1[] = {1, 2, 3, 4, 5, 6, 7};
        rotate.rotateUsingTempArray(a1, 2);
        for (int i = 0; i < a1.length; i++)
            System.out.print(a1[i] + " ");

        System.out.println();

        int a2[] = {1, 2, 3, 4, 5, 6, 7};
        rotate.leftRotate(a2, 2);
        for (int i = 0; i < a2.length; i++)
            System.out.print(a2[i] + " ");

        System.out.println();

        int a3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        rotate.jugglingRotate(a3, 3);
        for (int i : a3)
            System.out.print(i + " ");
    }
}
