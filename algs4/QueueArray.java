import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * The class represents a first-in-first-out (FIFO) queue of generic items.
 * It supports the usual enqueue and dequeue operations, along with methods
 * for peeking at the first item, testing if the queue is empty, and iterating
 * through the item in FIFO order.
 * <p>
 * Input: to be or not to - be - - that - - - is
 */
public class QueueArray<Item> implements Iterable<Item> {
    // Initial capacity of underlying resizing array.
    private static final int INIT_CAPACITY = 8;

    private Item[] q;       // queue elements
    private int n;          // number of elements on queue
    private int first;      // index of first element of queue
    private int last;       // index of next available slot

    public QueueArray() {
        q = (Item[]) new Object[INIT_CAPACITY];
        n = 0;
        first = 0;
        last = 0;
    }

    public boolean isEmpty() {
        return n == 0;
    }

    public int size() {
        return n;
    }

    // Resize the underlying array.
    private void resize(int capacity) {
        assert capacity >= n;
        Item[] copy = (Item[]) new Object[capacity];
        for (int i = 0; i < n; i++) {
            copy[i] = q[(first + i) % q.length];
        }
        q = copy;
        first = 0;
        last = n;
    }

    /**
     * Adds the item to this queue.
     *
     * @param item the item to add
     */
    public void enqueue(Item item) {
        // Double size of array if necessary and recopy to front of array.
        if (n == q.length) resize(2 * q.length);
        q[last++] = item;       // add item
        if (last == q.length) last = 0;     // wrap-around
        n++;
    }

    public Item dequeue() {
        if (isEmpty())
            throw new NoSuchElementException("Queue underflow");
        Item item = q[first];
        q[first] = null;
        n--;
        first++;
        if (first == q.length) first = 0;       // wrap-around
        // Shrink size of array if necessary.
        if (n > 0 && n == q.length / 4) resize(q.length / 2);
        return item;
    }

    public Item peek() {
        if (isEmpty())
            throw new NoSuchElementException("Queue underflow");
        return q[first];
    }

    @Override
    public Iterator<Item> iterator() {
        return new ArrayIterator();
    }

    private class ArrayIterator implements Iterator<Item> {
        private int i = 0;

        @Override
        public boolean hasNext() {
            return i < n;
        }

        @Override
        public Item next() {
            if (!hasNext())
                throw new NoSuchElementException();
            Item item = q[(i + first) % q.length];
            i++;
            return item;
        }
    }

    public static void main(String[] args) {
        QueueArray<String> queue = new QueueArray<>();
        while (!StdIn.isEmpty()) {
            String item = StdIn.readString();
            if (!item.equals("-"))
                queue.enqueue(item);
            else if (!queue.isEmpty())
                StdOut.print(queue.dequeue() + " ");
        }
        queue.forEach((node) -> StdOut.print(node + " "));
        StdOut.println("(" + queue.size() + " left on queue)");
    }
}
