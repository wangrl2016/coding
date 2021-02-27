package array;

import java.util.Collection;

public class ArrayInJava {
    // Both are valid declarations.
    int intArrayAfter[];
    int[] intArrayBefore;

    byte byteArray[];
    short shortArray[];
    boolean booleanArray[];
    long longArray[];
    float floatArray[];
    double doubleArray[];
    char charArray[];

    private class MyClass {
    }

    MyClass myClassArray[];

    Object[] ao;        // array of Object
    Collection[] ca;

    int intArraySep[];  // declaring array

    int[] intArrayConn = new int[20];   // combining both statements in one

    public ArrayInJava() {
        intArraySep = new int[20];  // allocating memory to array
    }

    public static void sum(int[] arr) {
        // Getting sum of array values.
        int sum = 0;
        for (int i = 0; i < arr.length; i++)
            sum += arr[i];
        System.out.println("Sum of array values: " + sum);
    }

    public static int[] returnArrayMethod() {
        return new int[]{1, 2, 3};
    }

    public static void main(String[] args) {
        int a[] = {3, 1, 2, 5, 4};
        // Passing array to method sum.
        sum(a);

        int[] arr;
        arr = new int[2];

        arr[0] = 10;
        arr[1] = 20;

        for (int i = 0; i < arr.length; i++)
            System.out.println("Element at index " + i + ": " + arr[i]);

        int[][] multiArray = {{2, 7, 9}, {3, 6, 1}, {7, 4, 2}};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                System.out.print(multiArray[i][j] + " ");
            System.out.println();
        }
    }
}
