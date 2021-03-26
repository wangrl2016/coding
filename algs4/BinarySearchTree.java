import edu.princeton.cs.algs4.Queue;

import java.util.NoSuchElementException;
import java.util.Objects;

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
 *
 * 分析
 *
 * The running times of algorithms on binary search trees depend on the shapes of the trees,
 * which, in turn, depend on  the order in which keys are inserted. In the best case, a tree
 * with N nodes could be perfectly balanced, with ~lgN nodes between the root and each null
 * link. In worse case there could be N nodes on the search path.
 *
 * 查找最大最小值
 *
 * If the left link of the root is null, the smallest key in a BST is the key at the root;
 * if the left link is not null, the smallest key in the BST is the smallest key in the
 * subtree rooted at the node referenced by the left link. Find the maximum key is similar,
 * moving to the right instead of to the left.
 */
public class BinarySearchTree<Key extends Comparable<Key>, Value> {
    private Node root;              // root of BST

    private class Node {
        private final Key key;            // sorted by key
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
     * In simple BSTs, the only new link is the one at the bottom, but resetting the links
     * higher up on the path is as easy as the test to avoid setting them. Also, we just
     * need to increment the node count on each node on the path, but we use more general
     * code that sets each to one plus the sum of the counts in its subtrees.
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

    /**
     * 被删除节点含有左右子树的情况
     *
     * 1. Save a link to the node to be deleted in t.
     * 2. Set x to point to its successor min(t.right).
     * 3. Set the right link of x(which is supposed to point to the BST containing
     * all the keys larger than x.key) to deleteMin(t.right), the link to the
     * BST containing all the keys that are larger than x.key after the deletion.
     * 4. Set the left link of x (which was null) to t.left (all the keys that are
     * less than both the deleted key and its successor).
     */
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
     * If a given key is less than the key at the root of a BST, then the floor
     * of key (the largest key in the BST less than or equal to key) must be in
     * the left subtree. If key is greater than the key at the root, then the
     * floor of key could be in the right subtree, but only if there is a key
     * smaller than or equal to key in the right subtree; if not, then the key
     * at the root is the floor of key.
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
            return Objects.requireNonNullElse(t, x);
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

    /**
     * Return key in BST rooted at x of given rank
     * Precondition:rank is in legal range.
     *
     * Suppose that we seek the key of rank k, if the number of keys t in the left
     * subtree is larger than k, we look (recursively) for the key of rank k in the
     * left subtree; if t is equal to k, we return the key at the root; af if t is
     * smaller than k, we look (recursively) for the key of rank k - t - 1 in the
     * right subtree.
     */
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

    /**
     * Number of keys in the subtree less than key.
     *
     * The inverse method rank() that returns the rank of a given key is similar: if the
     * given key is equal to hte key at the root, we return the number of keys t in the
     * left subtree; if the given key is less than the key at the root, we return the
     * rank of the key in the left subtree (recursively computed); and if the given key
     * is larger than the key at the root, we return t plus one (to count the key at the
     * root) plus the rank of the key in the right subtree (recursively computed).
     */
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
            return new Queue<>();
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
        int cmpLow = lo.compareTo(x.key);
        int cmpHigh = hi.compareTo(x.key);
        if (cmpLow < 0) keys(x.left, queue, lo, hi);
        if (cmpLow <= 0 && cmpHigh >= 0)
            queue.enqueue(x.key);
        if (cmpHigh > 0)
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

    // Does this binary tree satisfy symmetric order?
    // Note: this test also ensures that data structures is a binary tree since order is strict.
    private boolean isBST() {
        return isBST(root, null, null);
    }

    // Is the tree rooted at x a BST will all keys strictly between min and max.
    // (if min or max is null, treat as empty constraint)
    // Credit: Bob Dondero's elegant solution.
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

    // Check that ranks are consistent.
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
            System.out.println("Not in symmetric order");
        if (!isSizeConsistent())
            System.out.println("Subtree counts not consistent");
        if (!isRankConsistent())
            System.out.println("Ranks not consistent");
        return isBST() && isSizeConsistent() && isRankConsistent();
    }

    public static void main(String[] args) {
        BinarySearchTree<String, Integer> bst = new BinarySearchTree<>();
        String[] ss = args[0].split(" ");
        for (int i = 0; i < ss.length; i++) {
            bst.put(ss[i], i);
        }

        for (String s : bst.levelOrder())
            System.out.print("(" + s + " " + bst.get(s) + ")");

        System.out.println("\nBST height " + bst.height());
        bst.deleteMin();
        bst.deleteMax();
        System.out.println("Floor I is " + bst.floor("I"));
        System.out.println("Ceiling I is " + bst.ceiling("I"));
        System.out.println("Floor J is " + bst.floor2("J"));
    }
}
