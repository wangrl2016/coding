package binarytree;

/**
 * 参考文章
 * <p>
 * https://www.softwaretestinghelp.com/binary-search-tree-in-java/
 */

public class BinarySearchTree {
    static class Node {
        int key;
        Node left, right;

        public Node(int item) {
            key = item;
            left = right = null;
        }
    }

    // 根节点
    Node root;

    static Node insert(Node root, int key) {
        // Tree is empty.
        if (root == null) {
            root = new Node(key);
            return root;
        }

        // Traverse the tree.
        if (key < root.key)
            // Insert in the left subtree.
            root.left = insert(root.left, key);
        else if (key > root.key)
            // Insert in the right subtree.
            root.right = insert(root.right, key);

        return root;
    }

}
