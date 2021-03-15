import edu.princeton.cs.algs4.MinPQ;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Comparator;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * The MinPriorityQueue class represents a priority queue of generic keys.
 * It supports the usual insert and delete-the-minimum operations, along with
 * methods for peeking at the minimum key, testing if the priority queue is
 * empty, and iterating through the keys.
 * <p>
 * Comparable是需要比较的对象来实现接口。
 * Comparator需要定制一个比较类去实现，重写compare方法。
 */
public class MinPriorityQueue<Key> implements Iterable<Key> {
    private Key[] pq;       // store items as indices 1 to n
    private int n;          // number of items on priority queue
    private Comparator<Key> comparator;

    /**
     * Initializes an empty priority queue with the given initial capacity.
     *
     * @param initCapacity the initial capacity of this priority queue
     */
    public MinPriorityQueue(int initCapacity) {
        pq = (Key[]) new Object[initCapacity + 1];
        n = 0;
    }

    /**
     * Initializes an empty priority queue.
     */
    public MinPriorityQueue() {
        this(1);
    }

    /**
     * Initializes an empty priority queue with the given initial capacity,
     * using the given comparator.
     */
    public MinPriorityQueue(int initCapacity, Comparator<Key> comparator) {
        this.comparator = comparator;
        pq = (Key[]) new Object[initCapacity + 1];
        n = 0;
    }

    /**
     * Initializes an empty priority queue using the given copmarator.
     */
    public MinPriorityQueue(Comparator<Key> comparator) {
        this(1, comparator);
    }

    private boolean greater(int i, int j) {
        if (comparator == null)
            return ((Comparable<Key>) pq[i]).compareTo(pq[j]) > 0;
        else
            return comparator.compare(pq[i], pq[j]) > 0;
    }

    private void exch(int i, int j) {
        Key swap = pq[i];
        pq[i] = pq[j];
        pq[j] = swap;
    }

    /**
     * Specifically, we represent complete binary trees sequentially within
     * an array by putting the node in level order, with the root at position
     * 1, its children at position 2 and 3, and so on.
     * <p>
     * In a heap, the parent of the node in position k is in position ⎣k /2⎦ and,
     * conversely, the two children of the node in position k are in positions
     * 2k and 2k+1.
     */

    // 向上移动
    // Bottom-up reheapify (swim)
    private void swim(int k) {
        while (k > 1 && greater(k / 2, k)) {
            exch(k / 2, k);
            k = k / 2;
        }
    }

    // 向下移动
    // Top-down reheapify (sink)
    private void sink(int k) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && greater(j, j + 1)) j++;
            if (!greater(k, j)) break;
            exch(k, j);
            k = j;
        }
    }

    public void insert(Key x) {
        if (n == pq.length - 1)
            resize(2 * pq.length);

        // Add x, and percolate it up to maintain heap invariant.
        pq[++n] = x;
        swim(n);
        assert isMinHeap();
    }

    /**
     * Removes and returns a smallest key on this priority queue.
     *
     * @return a smallest key on this priority queue
     * @throws NoSuchElementException if this priority queue is empty
     */
    public Key delMin() {
        if (isEmpty())
            throw new NoSuchElementException("Priority queue underflow");
        Key min = pq[1];
        exch(1, n--);
        sink(1);
        pq[n + 1] = null; // to avoid loitering and help with garbage collection
        assert isMinHeap();
        return min;
    }

    public boolean isEmpty() {
        return n == 0;
    }

    private boolean isMinHeap() {
        for (int i = 1; i <= n; i++) {
            if (pq[i] == null) return false;
        }
        for (int i = n + 1; i < pq.length; i++) {
            if (pq[i] != null)
                return false;
        }
        if (pq[0] != null)
            return false;
        return isMinHeapOrdered(1);
    }

    // Is subtree of pq[1..n] rooted at k a min heap?
    private boolean isMinHeapOrdered(int k) {
        if (k > n) return true;
        int left = 2 * k;
        int right = 2 * k + 1;
        if (left <= n && greater(k, left)) return false;
        if (right <= n && greater(k, right)) return false;
        return isMinHeapOrdered(left) && isMinHeapOrdered(right);
    }

    // Resize the underlying array to have the given capacity.
    private void resize(int capacity) {
        assert capacity > n;
        Key[] temp = (Key[]) new Object[capacity];
        for (int i = 1; i <= n; i++) {
            temp[i] = pq[i];
        }
        pq = temp;
    }

    /**
     * Return the number of keys on this priority queue.
     *
     * @return the number of keys on this priority queue
     */
    public int size() {
        return n;
    }

    @Override
    public Iterator<Key> iterator() {
        return new HeapIterator();
    }

    private class HeapIterator implements Iterator<Key> {
        // Create a new pq.
        private MinPQ<Key> copy;

        // Add all items to copy of heap.
        // Takes linear time since already in heap order so on kyes move
        public HeapIterator() {
            if (comparator == null)
                copy = new MinPQ<>(size());
            else
                copy = new MinPQ<>(size(), comparator);
            for (int i = 1; i <= n; i++)
                copy.insert(pq[i]);
        }

        @Override
        public boolean hasNext() {
            return !copy.isEmpty();
        }

        @Override
        public Key next() {
            if (!hasNext())
                throw new NoSuchElementException();
            return copy.delMin();
        }
    }

    public static void main(String[] args) {
        MinPriorityQueue<String> pq = new MinPriorityQueue<>();
        while (!StdIn.isEmpty()) {
            String item = StdIn.readString();
            if (!item.equals("-"))
                pq.insert(item);
            else if (!pq.isEmpty())
                StdOut.print(pq.delMin() + " ");
        }
        StdOut.println("(" + pq.size() + " left on pq");
    }
}
