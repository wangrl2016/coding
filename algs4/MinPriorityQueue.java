import java.util.Comparator;
import java.util.Iterator;

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
        if (comparator == null) {
            return ((Comparable<Key>) pq[i]).compareTo(pq[j]) > 0;
        } else
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

    }

    // 向下移动
    private void sink(int k) {

    }


    @Override
    public Iterator<Key> iterator() {
        return null;
    }
}
