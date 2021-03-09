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

    /**
     * 利用的规则
     * If parent node is at index i in the array then the left child
     * of that node is at index (2*i + 1) and right child is at index
     * (2*i + 2) in the array.
     */
    public Node insertLevelOrder(int[] arr, Node root, int i) {
        // Base case for recursion.
        if (i < arr.length) {
            Node temp = new Node(arr[i]);
            root = temp;

            // Insert left child.
            root.left = insertLevelOrder(arr, root.left, 2 * i + 1);

            // Insert right child.
            root.right = insertLevelOrder(arr, root.right, 2 * i + 2);
        }
        return root;
    }

    public static void preOrder(Node root) {
        if (root == null)
            return;
        System.out.print(root.data + " ");
        preOrder(root.left);
        preOrder(root.right);
    }

    public static void inOrder(Node root) {
        if (root == null)
            return;
        inOrder(root.left);
        System.out.print(root.data + " ");
        inOrder(root.right);
    }

    public static void postOrder(Node root) {
        if (root == null)
            return;
        postOrder(root.left);
        postOrder(root.right);
        System.out.print(root.data + " ");
    }

    // 广度优先算法
    public static void levelOrder(Node root) {

    }

    // 深度优先
    public static void deepOrder(Node root) {

    }

    public static void main(String[] args) {
        BinaryTreeTraversal tree = new BinaryTreeTraversal();

        int[] arr = new int[20];
        for (int i = 0; i < arr.length; i++)
            arr[i] = i + 1;
        tree.root = tree.insertLevelOrder(arr, tree.root, 0);

    }
}
