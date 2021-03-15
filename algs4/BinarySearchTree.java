import java.util.NoSuchElementException;

/**
 * Definition: A binary search tree (BST) is a binary tree where each node has a
 * Comparable key (and an associated value) and satisfies the restriction that the key
 * in any node is larger than the key in all nodes in that node's left subtree and smaller
 * than the keys in all nodes in the node's right subtree.
 * <p>
 * The BinarySearchTree class represent an ordered symbol table of generic
 * key-value pairs.
 * It supports the usual put, get, contains, delete, size, and is-empty methods.
 */
public class BinarySearchTree<Key extends Comparable<Key>, Value> {
    private Node root;

    private class Node {
        private Key key;            // sorted by key
        private Value val;          // associated data
        private Node left, right;   // left and right subtrees
        // 包含本身，只有root节点size为1。
        private int size;           // number of nodes in subtree

        public Node(Key key, Value val, int size) {
            this.key = key;
            this.val = val;
            this.size = size;
        }
    }

    /**
     * Initializes an empty symbol tree.
     */
    public BinarySearchTree() {
    }

    /**
     * Returns true if this symbol table is empty.
     */
    public boolean isEmpty() {
        return size() == 0;
    }

    public int size() {
        return size(root);
    }

    private int size(Node x) {
        if (x == null) return 0;
        else return x.size;
    }

    /**
     * Does this symbol table contain the given key?
     */
    public boolean contains(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to contains() is null");
        return get(key) != null;
    }


    public Value get(Key key) {
        return get(root, key);
    }

    /**
     * Return the value associated with the given key.
     */
    private Value get(Node x, Key key) {
        if (key == null)
            throw new IllegalArgumentException("Calls get() with a null key");
        if (x == null)
            return null;
        int cmp = key.compareTo(x.key);
        if (cmp < 0)
            return get(x.left, key);
        else if (cmp > 0)
            return get(x.right, key);
        else
            return x.val;
    }

    /**
     * Inserts the specified key-value pair into the symbol table, overwriting
     * the old with the new value if the symbol table already contains the specified key.
     * Deletes the specified key (and its associated value) from this symbol table
     * if the specified value is null.
     */
    public void put(Key key, Value val) {
        if (key == null)
            throw new IllegalArgumentException("Calls put() with a null key");
        if (val == null) {
            delete(key);
            return;
        }
        root = put(root, key, val);
        assert check();
    }

    private Node put(Node x, Key key, Value val) {
        if (x == null)
            return new Node(key, val, 1);
        int cmp = key.compareTo(x.key);
        if (cmp < 0)
            x.left = put(x.left, key, val);
        else if (cmp > 0)
            x.right = put(x.right, key, val);
        else
            x.val = val;
        x.size = 1 + size(x.left) + size(x.right);
        return x;
    }

    /**
     * Removes the smallest key and associated value from the symbol table.
     */
    public void deleteMin() {
        if (isEmpty())
            throw new NoSuchElementException("Symbol table underflow");
        root = deleteMin(root);
        assert check();
    }

    private Node deleteMin(Node x) {
        // 用右节点填补根节点
        if (x.left == null)
            return x.right;
        x.left = deleteMin(x.left);
        x.size = size(x.left) + size(x.right) + 1;
        return x;
    }

    public void deleteMax() {
        if (isEmpty())
            throw new NoSuchElementException("Symbol table underflow");
        root = deleteMax(root);
        assert check();
    }

    private Node deleteMax(Node x) {
        if (x.right == null) return x.left;
        x.right = deleteMax(x.right);
        x.size = size(x.left) + size(x.right) + 1;
        return x;
    }

    /**
     * Remove the specified key and its associated value from this symbol table.
     * (if the key is in this symbol table).
     */
    public void delete(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Calls delete() with a null key");
        root = delete(root, key);
        assert check();
    }

    private Node delete(Node x, Key key) {
        if (x == null)
            return null;
        int cmp = key.compareTo(x.key);
        if (cmp < 0)
            x.left = delete(x.left, key);
        else if (cmp > 0)
            x.right = delete(x.right, key);
        else {
            if (x.right == null)
                return x.left;
            if (x.left == null)
                return x.right;
            Node temp = x;
            x = min(temp.right);
            x.right = deleteMin(temp.right);
            x.left = temp.left;
        }
        x.size = size(x.left) + size(x.right) + 1;
        return x;
    }

    /**
     * Returns the smallest key in the symbol table.
     */
    public Key min() {
        if (isEmpty())
            throw new NoSuchElementException("Calls min() with empty symbol table");
        return min(root).key;
    }

    private Node min(Node x) {
        if (x.left == null)
            return x;
        else
            return min(x.left);
    }

    /**
     * Returns the largest key in the symbol table.
     */
    public Key max() {
        if (isEmpty())
            throw new NoSuchElementException("Calls max() with empty symbol table");
        return max(root).key;
    }

    private Node max(Node x) {
        if (x.right == null)
            return x;
        else
            return max(x.right);
    }

    /**
     * Returns the largest key in the symbol table less than or equal to key.
     */
    public Key floor(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to floor() is null");
        if (isEmpty())
            throw new NoSuchElementException("Calls floor() with empty symbol table");
        Node x = floor(root, key);
        if (x == null)
            throw new NoSuchElementException("Argument to floor() is too small");
        else
            return x.key;
    }

    private Node floor(Node x, Key key) {
        if (x == null)
            return null;
        int cmp = key.compareTo(x.key);
        if (cmp == 0) return x;
        if (cmp < 0) return floor(x.left, key);
        Node t = floor(x.right, key);
        if (t != null)
            return t;
        else
            return x;
    }

    


    public static void main(String[] args) {

    }
}
