import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

public class SymbolTableSeqSearch<Key, Value> {
    private int n;      // number of key-value pairs
    private Node first; // the linked list of key-value pairs

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

    SymbolTableSeqSearch() {
    }

    public int size() {
        return n;
    }

    /**
     * Returns true if this symbol table is empty.
     */
    public boolean isEmpty() {
        return size() == 0;
    }

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
     * @return IllegalArgumentException if key is null
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
     * To iterate over all of the keys in the symbol table.
     */
    public Iterable<Key> keys() {
        Queue<Key> queue = new Queue<>();
        for (Node x = first; x != null; x = x.next)
            queue.enqueue(x.key);
        return queue;
    }

    public static void main(String[] args) {
        SymbolTableSeqSearch<String, Integer> symbolTable = new SymbolTableSeqSearch<>();
        for (int i = 0; !StdIn.isEmpty(); i++) {
            String key = StdIn.readString();
            symbolTable.put(key, i);
        }
        for (String s : symbolTable.keys())
            StdOut.println(s + "    " + symbolTable.get(s));
    }
}
