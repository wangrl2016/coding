import edu.princeton.cs.algs4.Queue;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.NoSuchElementException;

/**
 * Binary Search Trees
 *
 * 定义
 *
 * A binary search tree (BST) is a binary tree where each node has a Comparable key (and an associated value)
 * and satisfies the restriction that the key in any node is larger than the keys in all nodes in that node's
 * left subtree and smaller than the keys in all nodes in that node's right subtree.
 *
 *
 * Definition: A binary search tree (BST) is a binary tree where each node has a
 * Comparable key (and an associated value) and satisfies the restriction that the key
 * in any node is larger than the key in all nodes in that node's left subtree and smaller
 * than the keys in all nodes in the node's right subtree.
 *
 * The BinarySearchTree class represent an ordered symbol table of generic
 * key-value pairs.
 * It supports the usual put, get, contains, delete, size, and is-empty methods.
 *
 * 实现
 * We define a private nested class to define nodes in BSTs, just as we did for linked lists.
 * Each node contains a key, a value, a left link, a right link, and a node count(子节点的个数+自身).
 *
 * 求大小(Size)
 *
 * size(x) = size(x.left) + size(s.right) +1
 *
 * 查找(Search, Get)
 *
 * A recursive algorithm to search for a key in a BST follows immediately from the recursive
 * structure: if the tree is empty, we have a search miss; if the search key is equal to the
 * key at the root, we have a search hit. Otherwise, we search (recursively) in the appropriate
 * subtree, moving left if the search key is smaller, right if it is larger.
 *
 * 插入(Insert, Put)
 *
 * If the tree is empty, we return a new node containing the key and value; if the search key is
 * less than the key at the root, we set the left link to the result of inserting the key into
 * the left subtree; otherwise, we set the right link to the result of inserting the key into the
 * right subtree.
 */
public class BinarySearchTree<Key extends Comparable<Key>, Value> {
    private Node root;              // root of BST

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
     *
     * @return true if this symbol table is empty; false otherwise
     */
    public boolean isEmpty() {
        return size() == 0;
    }

    /**
     * Returns the number of key-value pairs in this symbol table.
     *
     * @return the number of key-value pairs in this symbol table
     */
    public int size() {
        return size(root);
    }

    // Return number of key-value pairs in BST rooted at x.
    private int size(Node x) {
        if (x == null) return 0;
        else return x.size;
    }

    /**
     * Does this symbol table contain the given key?
     *
     * @param key the key
     * @return true if this symbol table contains key and false otherwise
     */
    public boolean contains(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to contains() is null");
        return get(key) != null;
    }

    /**
     * Returns the value associated with the given key.
     *
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     * and null if the key is not in the symbol table
     */
    public Value get(Key key) {
        return get(root, key);
    }

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
     *
     * @param key the key
     * @param val the value
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

    /**
     * Removes the largest key and associated value from the symbol table.
     */
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
     *
     * @return the smallest key in the symbol table
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
     *
     * @return the largest key in the symbol table
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
     *
     * @param key the key
     * @return the largest key in the symbol table less than or equal to key
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

    public Key floor2(Key key) {
        Key x = floor2(root, key, null);
        if (x == null)
            throw new NoSuchElementException("Argument to floor() is too small");
        else
            return x;
    }

    private Key floor2(Node x, Key key, Key best) {
        if (x == null)
            return best;
        int cmp = key.compareTo(x.key);
        if (cmp < 0)
            return floor2(x.left, key, best);
        else if (cmp > 0)
            return floor2(x.right, key, x.key);
        else
            return x.key;
    }

    /**
     * Returns the smallest key in the symbol table greater than or equal to key.
     *
     * @param key the key
     * @return the smallest key in the symbol table greater than or equal to key
     */
    public Key ceiling(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to ceiling() is null");
        if (isEmpty())
            throw new NoSuchElementException("Calls ceiling() with empty symbol table");
        Node x = ceiling(root, key);
        if (x == null)
            throw new NoSuchElementException("Argument to floor() is too large");
        else
            return x.key;
    }

    private Node ceiling(Node x, Key key) {
        if (x == null)
            return null;
        int cmp = key.compareTo(x.key);
        if (cmp == 0)
            return x;
        if (cmp < 0) {
            Node t = ceiling(x.left, key);
            if (t != null)
                return t;
            else
                return x;
        }
        return ceiling(x.right, key);
    }

    /**
     * Return the key in the symbol table of given rank.
     * This key has the property that there are rank keys in
     * the symbol table that are smaller. In other words, this key is the
     * rank+1st smallest key in the symbol table.
     *
     * @param rank the order statistic
     * @return the key in the symbol table of given rank
     */
    public Key select(int rank) {
        if (rank < 0 || rank > size())
            throw new IllegalArgumentException("Argument to select() is invalid: " + rank);
        return select(root, rank);
    }

    // Return key in BST rooted at x of given rank
    // Precondition:rank is in legal range.
    private Key select(Node x, int rank) {
        if (x == null)
            return null;
        int leftSize = size(x.left);
        if (leftSize > rank)
            return select(x.left, rank);
        else if (leftSize < rank)
            return select(x.right, rank - leftSize - 1);
        else
            return x.key;
    }

    /**
     * Return the number of keys in the symbol table strictly less than key.
     *
     * @param key the key
     * @return the number of keys in the symbol table strictly less than key
     */
    public int rank(Key key) {
        if (key == null)
            throw new IllegalArgumentException("argument to rank() is null");
        return rank(key, root);
    }

    // Number of keys in the subtree less than key.
    private int rank(Key key, Node x) {
        if (x == null)
            return 0;
        int cmp = key.compareTo(x.key);
        if (cmp < 0)
            return rank(key, x.left);
        else if (cmp > 0)
            return 1 + size(x.left) + rank(key, x.right);
        else
            return size(x.left);
    }

    /**
     * Return all keys in the symbol table as an Iterable.
     * To iterate over all of the keys in the symbol table named st
     * use the foreach notation: for (Key key: st.keys()).
     *
     * @return all keys in the symbol table
     */
    public Iterable<Key> keys() {
        if (isEmpty())
            return new Queue<Key>();
        return keys(min(), max());
    }

    /**
     * Returns all keys in the symbol table in the given range, as Iterable.
     *
     * @param lo minimum endpoint
     * @param hi hi maximum endpoint
     * @return all keys in the symbol table between lo (inclusive) and hi (inclusive)
     */
    public Iterable<Key> keys(Key lo, Key hi) {
        if (lo == null)
            throw new IllegalArgumentException("First argument to keys() is null");
        if (hi == null)
            throw new IllegalArgumentException("Second argument to keys() is null");
        Queue<Key> queue = new Queue<>();
        keys(root, queue, lo, hi);
        return queue;
    }

    private void keys(Node x, Queue<Key> queue, Key lo, Key hi) {
        if (x == null)
            return;
        int cmplo = lo.compareTo(x.key);
        int cmphi = hi.compareTo(x.key);
        if (cmplo < 0) keys(x.left, queue, lo, hi);
        if (cmplo <= 0 && cmphi >= 0)
            queue.enqueue(x.key);
        if (cmphi > 0)
            keys(x.right, queue, lo, hi);
    }

    /**
     * Returns the number of keys in the symbol table in the given range.
     *
     * @param lo minimum endpoint
     * @param hi maximum endpoint
     * @return the number of keys in the symbol table between lo (inclusive) and hi (inclusive)
     */
    public int size(Key lo, Key hi) {
        if (lo == null)
            throw new IllegalArgumentException("First argument to size() is null");
        if (hi == null)
            throw new IllegalArgumentException("Second argument to size() is null");
        if (lo.compareTo(hi) > 0) return 0;
        if (contains(hi))
            return rank(hi) - rank(lo) + 1;
        else
            return rank(hi) - rank(lo);
    }

    /**
     * Returns the height of the BST (for debugging).
     */
    public int height() {
        return height(root);
    }

    private int height(Node x) {
        if (x == null)
            return -1;
        return 1 + Math.max(height(x.left), height(x.right));
    }

    /**
     * 广度优先遍历
     *
     * Returns the keys in the BST in level order.
     *
     * @return the keys in the BST in level order traversal
     */
    public Iterable<Key> levelOrder() {
        Queue<Key> keys = new Queue<>();
        Queue<Node> nodes = new Queue<>();
        nodes.enqueue(root);

        while (!nodes.isEmpty()) {
            Node x = nodes.dequeue();
            if (x == null)
                continue;
            keys.enqueue(x.key);
            nodes.enqueue(x.left);
            nodes.enqueue(x.right);
        }
        return keys;
    }

    private boolean isBST() {
        return isBST(root, null, null);
    }

    private boolean isBST(Node x, Key min, Key max) {
        if (x == null)
            return true;
        if (min != null && x.key.compareTo(min) <= 0) return false;
        if (max != null && x.key.compareTo(max) >= 0) return false;
        return isBST(x.left, min, x.key) && isBST(x.right, x.key, max);
    }

    // Are the size fields correct?
    private boolean isSizeConsistent() {
        return isSizeConsistent(root);
    }

    private boolean isSizeConsistent(Node x) {
        if (x == null)
            return true;
        if (x.size != size(x.left) + size(x.right) + 1)
            return false;
        return isSizeConsistent(x.left) && isSizeConsistent(x.right);
    }

    private boolean isRankConsistent() {
        for (int i = 0; i < size(); i++)
            if (i != rank(select(i)))
                return false;
        for (Key key : keys())
            if (key.compareTo(select(rank(key))) != 0)
                return false;
        return true;
    }

    private boolean check() {
        if (!isBST())
            StdOut.println("Not in symmetric order");
        if (!isSizeConsistent())
            StdOut.println("Subtree counts not consistent");
        if (!isRankConsistent())
            StdOut.println("Ranks not consistent");
        return isBST() && isSizeConsistent() && isRankConsistent();
    }

    public static void main(String[] args) {
        BinarySearchTree<String, Integer> tree = new BinarySearchTree<>();
        for (int i = 0; !StdIn.isEmpty(); i++) {
            String key = StdIn.readString();
            tree.put(key, i);
        }

        for (String s : tree.levelOrder())
            StdOut.println(s + " " + tree.get(s));

        StdOut.println("----");

        for (String s : tree.keys())
            StdOut.println(s + " " + tree.get(s));
    }
}
