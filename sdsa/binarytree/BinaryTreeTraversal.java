package binarytree;

/**
 * 树的构建
 * <p>
 * https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
 * <p>
 * 参考文档
 */

public class BinaryTreeTraversal {

    Node root;      // 树的根节点

    static class Node {
        int data;
        Node left;
        Node right;

        Node(int d) {
            data = d;
            left = right = null;
        }
    }

    BinaryTreeTraversal() {
        root = null;
    }

    public Node insertLevelOrder(int[] arr, Node root, int i) {
        // Base case for recursion.
        if (i < arr.length) {
            
        }
    }

    public static void main(String[] args) {
        BinaryTreeTraversal tree = new BinaryTreeTraversal();
        /**
         *    1
         *  /   \
         * 2    3
         */
        tree.root = new Node(1);
        tree.root.left = new Node(2);
        tree.root.right = new Node(3);


    }
}
