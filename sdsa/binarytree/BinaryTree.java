package binarytree;

/**
 * A binary tree is a data structure in which an element has two successors(children). The left
 * child is usually smaller than the parent, and the right child is usually bigger.
 */
public class BinaryTree {
    class Node {
        public int data;
        public Node left;
        public Node right;
        public Node parent;

        public Node(int value) {
            data = value;
            left = null;
            right = null;
            parent = null;
        }
    }

    // The root of the binary tree.
    private Node root;


    public BinaryTree() {
        root = null;
    }

    /**
     * Method to find a node with a certain value.
     *
     * @param key Value being looked for
     * @return The node if it finds it, otherwise returns the parent
     */
    public Node find(int key) {
        Node current = root;
        while (current != null) {
            if (key < current.data) {
                if (current.left == null)
                    // The key is not exist, returns the parent.
                    return current;
                current = current.left;
            } else if (key > current.data) {
                if (current.right == null)
                    return current;
                current = current.right;
            } else {
                // If you find the value return it.
                return current;
            }
        }
        return null;
    }

    /**
     * Inserts certain value into the binary tree.
     *
     * @param value Value to be inserted
     */
    public void put(int value) {
        Node newNode = new Node(value);
        if (root == null)
            root = newNode;
        else {
            // This will return the soon to be parent of the value you're inserting.
            Node parent = find(value);

            // This if/else assigns the new node to be either the left or right child of the parent.
            if (value < parent.data) {
                parent.left = newNode;
                parent.left.parent = parent;
            } else {
                parent.right = newNode;
                parent.right.parent = parent;
            }
        }
    }

    /**
     * Print leftChild - root - rightChild
     *
     * @param localRoot The local root of the binary tree
     */
    public void inOrder(Node localRoot) {
        if (localRoot != null) {
            inOrder(localRoot.left);
            System.out.print(localRoot.data + " ");
            inOrder(localRoot.right);
        }
    }

    /**
     * Print root - leftChild - rightChild
     *
     * @param localRoot The local root of the binary tree
     */
    public void preOrder(Node localRoot) {
        if (localRoot != null) {
            System.out.print(localRoot.data + " ");
            preOrder(localRoot.left);
            preOrder(localRoot.right);
        }
    }

    /**
     * Prints rightChild - leftChild - root
     *
     * @param localRoot The local root of the binary tree
     */
    public void postOrder(Node localRoot) {
        if (localRoot != null) {
            postOrder(localRoot.left);
            postOrder(localRoot.right);
            System.out.print(localRoot.data + " ");
        }
    }

    public static void main(String[] args) {
        BinaryTree tree = new BinaryTree();
        for (int i = 0; i < 10; i++) {
            tree.put(i);
        }
        tree.preOrder(tree.root);
        System.out.println();
        tree.inOrder(tree.root);
        System.out.println();
        tree.postOrder(tree.root);
    }
}
