import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdOut;

import java.awt.*;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Queue implementation with a resizing array.
 *
 * The ResizingArrayQueue class represents a first-in-first-out (FIFO) queue of generic items.
 * It supports the usual enqueue and dequeue operations, along with methods for peeking at the
 * first item, testing if the queue is empty, and iterating through the item in FIFO order.
 *
 * This implementation uses a resizing array, which double the underlying array when it is full
 * and halves the underlying array when it is one-quarter full.
 *
 * The enqueue and dequeue operations take constant amortized time.
 * The size, peek and isEmpty operations takes constant time in the worst case.
 */
public class ResizingArrayQueue<Item> implements Iterable<Item> {
    // Initial capacity of underlying resizing array.
    private static final int INIT_CAPACITY = 8;

    private Item[] q;       // queue elements
    private int n;          // number of elements on queue
    private int first;      // index of first element of queue
    private int last;       // index of next available slot

    /**
     * Initializes an empty queue.
     */
    @SuppressWarnings("unchecked")
    public ResizingArrayQueue() {
        q = (Item[]) new Object[INIT_CAPACITY];
        n = 0;
        first = 0;
        last = 0;
    }

    /**
     * Is this queue empty?
     *
     * @return true if this queue is empty; false otherwise
     */
    public boolean isEmpty() {
        return n == 0;
    }

    /**
     * Returns the number of items in this queue.
     *
     * @return the number of items in this queue
     */
    public int size() {
        return n;
    }

    // Resize the underlying array.
    @SuppressWarnings("unchecked")
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

    /**
     * Removes and returns the item on this queue that was least recently added.
     *
     * @return the item on this queue that was least recently added
     */
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

    /**
     * Returns the item least recently added to this queue.
     *
     * @return the item least recently added to this queue
     */
    public Item peek() {
        if (isEmpty())
            throw new NoSuchElementException("Queue underflow");
        return q[first];
    }

    /**
     * Returns an iterator that iterates over the times in this queue in FIFO order.
     *
     * @return an iterator that iterates over the items in this queue in FIFO order
     */
    @Override
    public Iterator<Item> iterator() {
        return new ArrayIterator();
    }

    // An iterator, doesn't implement remove() since it's optional.
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

    /**
     * 绘制队列，同时将大小、首尾的位置进行标记。
     *
     * @param row 绘制的行数
     */
    public void draw(int row) {
        StdDraw.text(1 / 15.0, row / 30.0, String.valueOf(q.length));
        StdDraw.text(2 / 15.0, row / 30.0, String.valueOf(first));
        StdDraw.text(3 / 15.0, row / 30.0, String.valueOf(last));

        for (int i = 0; i < n; i++)
            StdDraw.text((i + 4) / 15.0, row / 30.0, String.valueOf(q[(first + i) % q.length]));
    }

    public static void main(String[] args) {
        if (args.length < 1)
            System.exit(1);
        StdDraw.enableDoubleBuffering();
        StdDraw.setPenColor(Color.BLACK);
        ResizingArrayQueue<String> queue = new ResizingArrayQueue<>();
        String[] ss = args[0].split(" ");
        StdDraw.text(1 / 15.0, 1 / 30.0, "S");
        StdDraw.text(2 / 15.0, 1 / 30.0, "F");
        StdDraw.text(3 / 15.0, 1 / 30.0, "L");
        int row = 2;
        for (String item : ss) {
            if (!item.equals("-"))
                queue.enqueue(item);
            else if (!queue.isEmpty())
                System.out.print(queue.dequeue() + " ");
            queue.draw(row++);
        }

        System.out.println("\nFirst element '" + queue.peek() + "'");
        queue.forEach((node) -> StdOut.print(node + " "));
        StdOut.println("(" + queue.size() + " left on queue)");

        StdDraw.show();
        StdDraw.save("out/resizing-array-queue.png");
        System.exit(0);
    }
}
