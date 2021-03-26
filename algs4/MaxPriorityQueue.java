import java.util.Comparator;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Heap定义
 *
 * A binary tree is heap-ordered if the key in each node is larger than or equal to the keys in that node's
 * two children (if any).
 *
 * 定理
 *
 * The largest key in a heap-ordered binary tree is found at the root.
 *
 * A binary heap is a collection of keys arranged in a complete heap-ordered binary tree, represented in
 * level order in an array.
 *
 * We represent complete binary trees sequentially within an array by putting the nodes in level order,
 * with the root at position 1, its children at position 2 and 3,and so on.
 * In a heap, the parent of the node in position k is position [k/2] and, conversely, the two children
 * of the node in position k are in positions 2k and 2k+1.
 *
 * Instead of using explicit links, we can travel up and down by doing simple arithmetic on array indices:
 * to move up the tree from a[k] we set k to k/2; to move down the tree we set k to 2k or 2k + 1.
 *
 * The heap operations that we consider work by first making a simple modification that could violate
 * the heap condition, then traveling through the heap, modifying the heap as required to ensure that
 * the heap condition is satisfied everywhere. We refer to this process as reheapifying, or restoring
 * heap order.
 *
 * In a N-key priority queue, the heap algorithms require no more than 1 + lgN compares for insert and
 * no more than 2lgN compares for remove the maximum.
 */
public class MaxPriorityQueue<Key> implements Iterable<Key> {
    private Key[] pq;       // store items at indices 1 to n
    private int n;          // number of items on priority queue
    private Comparator<Key> comparator; // optional comparator

    @SuppressWarnings("unchecked")
    public MaxPriorityQueue(int initCapacity) {
        pq = (Key[]) new Object[initCapacity + 1];
        n = 0;
    }

    public MaxPriorityQueue() {
        this(1);
    }

    @SuppressWarnings("unchecked")
    public MaxPriorityQueue(int initCapacity, Comparator<Key> comparator) {
        this.comparator = comparator;
        // Represent a heap of size N in private array pq[] of length N +1, with
        // pq[0] unused and the heap in pq[1] through pq[n].
        pq = (Key[]) new Object[initCapacity + 1];
        n = 0;
    }

    public MaxPriorityQueue(Comparator<Key> comparator) {
        this(1, comparator);
    }

    /**
     * Initializes a priority queue from the array of keys.
     * Takes time proportional to the number of keys, using sink-based heap construction.
     *
     * @param keys the array of keys
     */
    @SuppressWarnings("unchecked")
    public MaxPriorityQueue(Key[] keys) {
        n = keys.length;
        pq = (Key[]) new Object[keys.length + 1];
        System.arraycopy(keys, 0, pq, 1, n);
        for (int k = n / 2; k >= 1; k--)
            sink(k);
        assert isMaxHeap();
    }

    public boolean isEmpty() {
        return n == 0;
    }

    public int size() {
        return n;
    }

    public Key max() {
        if (isEmpty())
            throw new NoSuchElementException("Priority queue underflow");
        return pq[1];
    }

    @SuppressWarnings("unchecked")
    private void resize(int capacity) {
        assert capacity > n;
        Key[] temp = (Key[]) new Object[capacity];
        if (n >= 0) System.arraycopy(pq, 1, temp, 1, n);
        pq = temp;
    }

    /**
     * We add the new key at the end of the array, increment the size of the heap, and then
     * swim up through the heap with that key to restore the heap condition.
     */
    public void insert(Key x) {
        if (n == pq.length - 1)
            resize(2 * pq.length);

        pq[++n] = x;
        swim(n);
        assert isMaxHeap();
    }

    /**
     * We take the largest key off the top, put the item from the end of the heap at the top, decrement
     * the size of the heap, and then sink down through the heap with that key to restore the heap condition.
     */
    public Key delMax() {
        if (isEmpty())
            throw new NoSuchElementException("Priority queue underflow");
        Key max = pq[1];
        exch(1, n--);
        sink(1);
        pq[n + 1] = null;
        if ((n > 0) && (n == (pq.length - 1) / 4)) resize(pq.length / 2);
        assert isMaxHeap();
        return max;
    }

    /**
     * Bottom-up reheapify (swim)
     *
     * If the heap order is violated because a node's key becomes larger than the node's parent's key, then
     * we can make progress toward fixing the violation by exchanging the node with its parent. After the
     * exchange, the node is larger than both its children, but the node may still be larger than its parent.
     * We can fix that violation in the same way, and so forth, moving up the heap until we reach a node
     * with a larger key, or the root.
     */
    private void swim(int k) {
        while (k > 1 && less(k / 2, k)) {
            exch(k, k / 2);
            k = k / 2;
        }
    }

    /**
     * Top-down reheapify (sink)
     *
     * If the heap order is violated because a node's key becomes smaller than one or both of that node's
     * children's keys, then we can make progress toward fixing the violation by exchanging the node with
     * the larger of its two children. This switch may cause a violation at the child; we fix that violation
     * in the same way, and so forth, moving down the heap until we reach a node with both children
     * smaller (or equal), or the bottom.
     */
    private void sink(int k) {
        while (2 * k <= n) {
            int j = 2 * k;
            if (j < n && less(j, j + 1)) j++;
            if (!less(k, j)) break;
            exch(k, j);
            k = j;
        }
    }

    @SuppressWarnings("unchecked")
    private boolean less(int i, int j) {
        if (comparator == null) {
            return ((Comparable<Key>) pq[i]).compareTo(pq[j]) < 0;
        } else
            return comparator.compare(pq[i], pq[j]) < 0;
    }

    private void exch(int i, int j) {
        Key swap = pq[i];
        pq[i] = pq[j];
        pq[j] = swap;
    }

    private boolean isMaxHeap() {
        for (int i = 1; i <= n; i++) {
            if (pq[i] == null)
                return false;
        }

        for (int i = n + 1; i < pq.length; i++) {
            if (pq[i] != null)
                return false;
        }

        if (pq[0] != null)
            return false;

        return isMaxHeapOrdered(1);
    }

    private boolean isMaxHeapOrdered(int k) {
        if (k > n)
            return true;
        int left = 2 * k;
        int right = 2 * k + 1;
        if (left <= n && less(k, left)) return false;
        if (right <= n && less(k, right)) return false;
        return isMaxHeapOrdered(left) && isMaxHeapOrdered(right);
    }

    @Override
    public Iterator<Key> iterator() {
        return new HeapIterator();
    }

    private class HeapIterator implements Iterator<Key> {
        // create a new pq
        private final MaxPriorityQueue<Key> copy;

        public HeapIterator() {
            if (comparator == null)
                copy = new MaxPriorityQueue<>(size());
            else
                copy = new MaxPriorityQueue<>(size(), comparator);
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
            return copy.delMax();
        }
    }

    public static void main(String[] args) {
        MaxPriorityQueue<String> pq = new MaxPriorityQueue<>();
        String[] ss = args[0].split(" ");
        System.out.print("删除元素 ");
        for (String s : ss) {
            if (!s.equals("-"))
                pq.insert(s);
            else if (!pq.isEmpty())
                System.out.print(pq.delMax() + " ");
        }
        System.out.println("\nPriority queue max key " + pq.max());
        for (String key : pq)
            System.out.print(key + " ");
        System.out.println("(" + pq.size() + " left on pq)");
    }
}
