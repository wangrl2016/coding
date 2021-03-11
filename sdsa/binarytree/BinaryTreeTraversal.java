package binarytree;

import java.util.LinkedList;

/**
 * 树的构建
 * <p>
 * https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
 * <p>
 * 后序遍历迭代版本
 * <p>
 * https://www.geeksforgeeks.org/iterative-postorder-traversal/
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

    public static void preOrderRecursion(Node root) {
        if (root == null)
            return;
        System.out.print(root.data + " ");
        preOrderRecursion(root.left);
        preOrderRecursion(root.right);
    }

    public static void preOrderIteration(Node root) {
        LinkedList<Node> stack = new LinkedList<>();
        Node node = root;
        while (node != null || !stack.isEmpty()) {
            if (node != null) {
                System.out.print(node.data + " ");
                stack.push(node);
                node = node.left;
            } else {
                Node temp = stack.pop();
                node = temp.right;
            }
        }
    }

    public static void inOrderRecursion(Node root) {
        if (root == null)
            return;
        inOrderRecursion(root.left);
        System.out.print(root.data + " ");
        inOrderRecursion(root.right);
    }

    public static void inOrderIteration(Node root) {
        LinkedList<Node> stack = new LinkedList<>();
        Node node = root;
        while (node != null || !stack.isEmpty()) {
            if (node != null) {
                stack.push(node);
                node = node.left;
            } else {
                Node temp = stack.pop();
                System.out.print(temp.data + " ");
                node = temp.right;
            }
        }
    }

    public static void postOrderRecursion(Node root) {
        if (root == null)
            return;
        postOrderRecursion(root.left);
        postOrderRecursion(root.right);
        System.out.print(root.data + " ");
    }

    // 使用两个栈
    public static void postOrderIteration(Node root) {
        LinkedList<Node> stack1 = new LinkedList<>();
        LinkedList<Node> stack2 = new LinkedList<>();

        if (root == null)
            return;

        stack1.push(root);

        while (!stack1.isEmpty()) {
            // Pop an item from stack1 and push it to stack2.
            Node temp = stack1.pop();
            stack2.push(temp);

            // Push left and right children of removed item to stack1.
            if (temp.left != null)
                stack1.push(temp.left);
            if (temp.right != null)
                stack1.push(temp.right);
        }

        // Print all elements of second stack.
        while (!stack2.isEmpty()) {
            Node temp = stack2.pop();
            System.out.print(temp.data + " ");
        }
    }

    // 广度优先算法
    public static void levelOrder(Node root) {
        if (root == null)
            return;

        LinkedList<Node> queue = new LinkedList<>();
        queue.offer(root);
        while (!queue.isEmpty()) {
            Node node = queue.poll();   // 出队列
            System.out.print(node.data + " ");
            if (node.left != null)
                queue.offer(node.left); // 入队列
            if (node.right != null)
                queue.offer(node.right); // 入队列
        }
    }

    // 深度优先算法
    public static void deepOrder(Node root) {
        if (root == null)
            return;
        LinkedList<Node> stack = new LinkedList<>();
        stack.push(root);
        while (!stack.isEmpty()) {
            Node node = stack.pop();
            System.out.print(node.data + " ");
            if (node.right != null) {
                stack.push(node.right);
            }
            if (node.left != null) {
                stack.push(node.left);
            }
        }
    }

    public static void main(String[] args) {
        BinaryTreeTraversal tree = new BinaryTreeTraversal();

        int[] arr = new int[7];
        for (int i = 0; i < arr.length; i++)
            arr[i] = i + 1;
        tree.root = tree.insertLevelOrder(arr, tree.root, 0);

        System.out.print("前序递归遍历: ");
        preOrderRecursion(tree.root);
        System.out.print("\n前序迭代遍历: ");
        preOrderIteration(tree.root);
        System.out.print("\n中序递归遍历: ");
        inOrderRecursion(tree.root);
        System.out.print("\n中序迭代遍历: ");
        inOrderIteration(tree.root);
        System.out.print("\n后序递归遍历: ");
        postOrderRecursion(tree.root);
        System.out.print("\n后序迭代遍历: ");
        postOrderIteration(tree.root);
        System.out.print("\n广度优先遍历: ");
        levelOrder(tree.root);
        System.out.print("\n深度优先遍历: ");
        deepOrder(tree.root);
    }
}
