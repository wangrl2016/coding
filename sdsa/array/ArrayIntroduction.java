public class ArrayIntroduction {

    public static void main(String[] args) {
        // A character array in Java.
        char arr1[] = {'g', 'e', 'e', 'k', 's'};

        // An Integer array in Java.
        int arr2[] = {10, 20, 30, 40, 50};

        for (char c : arr1)
            System.out.print(c + " ");

        System.out.println();

        for (int i : arr2)
            System.out.print(i + " ");
    }
}
