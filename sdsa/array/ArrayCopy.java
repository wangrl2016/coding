package array;

public class ArrayCopy {
    public static void main(String[] args) {
        int singleArray[] = {1, 2, 3};
        int cloneSingleArray[] = singleArray.clone();

        System.out.println(singleArray == cloneSingleArray);

        int multiArray[][] = {{1, 2, 3}, {4, 5}};
        int cloneMultiArray[][] = multiArray.clone();

        System.out.println(multiArray == cloneMultiArray);
        System.out.println(multiArray[0] == cloneMultiArray[0]);
        System.out.println(multiArray[1] == cloneMultiArray[1]);
    }
}
