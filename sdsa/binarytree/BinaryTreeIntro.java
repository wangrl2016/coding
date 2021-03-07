package binarytree;

public class BinaryTreeIntro {
    // Root of binary tree.
    Node root;

    BinaryTreeIntro(int key) {
        root = new Node(key);
    }

    BinaryTreeIntro() {
        root = null;
    }

    static class Node {
        int key;
        Node left, right;

        public Node(int item) {
            key = item;
            left = right = null;
        }
    }

    public static void main(String[] args) {
        BinaryTreeIntro tree = new BinaryTreeIntro();

        /**
         *     1
         *   /   \
         * null  null
         */
        tree.root = new Node(1);

        /**
         *                1
         *             /     \
         *           2        3
         *         /   \     /  \
         *       null null null null
         */
        tree.root.left = new Node(2);
        tree.root.right = new Node(3);
    }
}
