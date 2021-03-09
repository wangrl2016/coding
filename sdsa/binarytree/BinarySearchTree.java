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

    BinarySearchTree() {
        root = null;
    }

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

    boolean search(int key) {
        return search(root, key) != null;
    }

    static Node search(Node root, int key) {
        // Base cases: root is null or key is present at root.
        if (root == null || root.key == key)
            return root;

        if (key > root.key)
            return search(root.right, key);
        else
            return search(root.left, key);
    }

    void delete(int key) {
        root = delete(root, key);
    }

    static int minValue(Node root) {
        int min = root.key;
        while (root.left != null) {
            min = root.left.key;
            root = root.left;
        }
        return min;
    }

    static Node delete(Node root, int key) {
        if (root == null)
            return null;

        // Traverse the tree.
        if (key < root.key)
            root.left = delete(root.left, key); // traverse left subtree
        else if (key > root.key)
            root.right = delete(root.right, key);
        else {
            // Node contains only one child
            if (root.left == null)
                return root.right;
            else if (root.right == null)
                return root.left;

            // Node has two children.
            // 获取右子树的最小值
            root.key = minValue(root.right);
            // 删除右子树叶节点的最小值
            root.right = delete(root.right, root.key);
        }
        return root;
    }

    // left - root - right traverse the BST.
    static void inorder(Node root) {
        if (root != null) {
            inorder(root.left);
            System.out.print(root.key + " ");
            inorder(root.right);
        }
    }

    public static void main(String[] args) {
        BinarySearchTree tree = new BinarySearchTree();
        int[] arr = {45, 10, 7, 12, 90, 50};
        /**
         *        45
         *      /    \
         *    10      90
         *   /  \     /
         *  7   12   50
         */
        for (int a : arr)
            tree.root = insert(tree.root, a);
        inorder(tree.root);

        boolean found = tree.search(50);
        System.out.println("\nKey 50 found in BST: " + found);

        tree.delete(45);
        inorder(tree.root);
    }
}
