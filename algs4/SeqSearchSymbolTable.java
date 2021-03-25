import edu.princeton.cs.algs4.Queue;

/**
 * Symbol table implementation with sequential search in an unordered linked list
 * of key-value pairs.
 *
 * The SeqSearchSymbolTable class represents an (unordered) symbol table of generic
 * key-value pairs.
 *
 * 提供的操作
 *
 * It supports the usual put, get, contains, delete, size and isEmpty methods. It
 * also provides a keys method for iterating over all of the keys.
 *
 * A symbol table implements the associative array abstraction: when associating a
 * value with a key that is already in the symbol table, the convention is to replace
 * the old value with the new value.
 * The class also uses the convention that values cannot be null. Setting the value
 * associated with a key to null is equivalent to deleting the key from the symbol table.
 *
 * It relies on the equals method to test whether two keys are equal. It does not call
 * either the compareTo() or hashCode() method.
 *
 * 时间消耗
 *
 * This implementation uses a singly linked list and sequential search.
 * The put and delete operations take O(n).
 * The get and contains operations take O(n) time in the worse case.
 * The size and isEmpty operations take O(1) time.
 */
public class SeqSearchSymbolTable<Key, Value> {
    private int n;      // number of key-value pairs
    private Node first; // the linked list of key-value pairs

    // A helper linked list data type.
    private class Node {
        private Key key;
        private Value val;
        private Node next;

        public Node(Key key, Value val, Node next) {
            this.key = key;
            this.val = val;
            this.next = next;
        }
    }

    /**
     * Initializes an empty symbol table.
     */
    SeqSearchSymbolTable() {
    }

    /**
     * Return the number of key-value pairs in this symbol table.
     *
     * @return the number of key-value pairs in this symbol table
     */
    public int size() {
        return n;
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
     * Returns true if this symbol table contains the specified key.
     *
     * @param key the key
     * @return true if this symbol table contains key; false otherwise
     */
    public boolean contains(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to contain() is null");
        return get(key) != null;
    }

    /**
     * Returns the value associated with the given key in this symbol table.
     *
     * @param key the key
     * @return the value associated with the given key if the key is in the
     * symbol table and null if the key is not in the symbol table
     */
    public Value get(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to get() is null");
        for (Node x = first; x != null; x = x.next) {
            if (key.equals(x.key))
                return x.val;
        }
        return null;
    }

    /**
     * Inserts the specified key-value pair into the symbol table, overwriting the old
     * value with the new value if the symbol table already contains the specified key.
     * Deletes the specified key (and its associated value) from this symbol table
     * if the specified value is null.
     *
     * @param key the key
     * @param val the value
     */
    public void put(Key key, Value val) {
        if (key == null)
            throw new IllegalArgumentException("First argument to put() is null");
        if (val == null) {
            delete(key);
            return;
        }

        for (Node x = first; x != null; x = x.next) {
            if (key.equals(x.key)) {
                x.val = val;
                return;
            }
        }
        first = new Node(key, val, first);
        n++;
    }

    /**
     * Removes the specified key and its associated value from this symbol table
     * (if the key is in this symbol table).
     *
     * @param key the key
     */
    public void delete(Key key) {
        if (key == null)
            throw new IllegalArgumentException("Argument to delete is null");
        first = delete(first, key);
    }

    // Delete key in linked list beginning at Node x.
    // Warning: function call stack too large if table is large.
    private Node delete(Node x, Key key) {
        if (x == null)
            return null;
        if (key.equals(x.key)) {
            n--;
            return x.next;
        }
        x.next = delete(x.next, key);
        return x;
    }

    /**
     * Returns all keys in the symbol table as an Iterable.
     * To iterate over all of the keys in the symbol table name st,
     * use the foreach notation: for(Key key : st.keys()).
     */
    public Iterable<Key> keys() {
        Queue<Key> queue = new Queue<>();
        for (Node x = first; x != null; x = x.next)
            queue.enqueue(x.key);
        return queue;
    }

    public static void main(String[] args) {
        String[] ss = args[0].split(" ");
        SeqSearchSymbolTable<String, Integer> st = new SeqSearchSymbolTable<>();
        for (int i = 0; i < ss.length; i++) {
            st.put(ss[i], i);
        }
        for (String s : st.keys())
            System.out.println(s + " " + st.get(s));
        System.out.println("Symbol table " + ((st.contains("that") ? "" : "not "))
                + "contains " + "'that'");
    }
}
