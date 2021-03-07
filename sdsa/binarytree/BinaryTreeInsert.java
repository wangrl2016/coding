package binarytree;

import java.util.LinkedList;
import java.util.Queue;

public class BinaryTreeInsert {
    static class Node {
        int key;
        Node left, right;

        Node(int key) {
            this.key = key;
            left = null;
            right = null;
        }
    }

    // Inorder traversal of a binary tree.
    static void inorder(Node root) {
        if (root == null)
            return;

        inorder(root.left);
        System.out.print(root.key + " ");
        inorder(root.right);
    }

    // Insert element in binary tree.
    static void insert(Node root, int key) {
        if (root == null) {
            root = new Node(key);
            return;
        }

        Queue<Node> q = new LinkedList<Node>();
        q.add(root);

        // Do level order traversal until we find
        // an empty place.
        while (!q.isEmpty()) {
            root = q.peek();
            q.remove();

            if (root.left == null) {
                root.left = new Node(key);
                break;
            } else {
                q.add(root.left);
            }

            if (root.right == null) {
                root.right = new Node(key);
                break;
            } else {
                q.add(root.right);
            }
        }
    }

    public static void main(String[] args) {
        /**
         *              10
         *            /    \
         *          11      9
         *         /      /   \
         *       7       15    8
         */
        Node root = new Node(10);
        root.left = new Node(11);
        root.left.left = new Node(7);
        root.right = new Node(9);
        root.right.left = new Node(15);
        root.right.right = new Node(8);

        System.out.print("Inorder traversal before insertion: ");
        inorder(root);

        int key = 12;
        insert(root, key);

        System.out.print("\nInorder traversal after insertion: ");
        inorder(root);
    }
}
