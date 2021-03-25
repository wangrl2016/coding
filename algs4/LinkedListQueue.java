import edu.princeton.cs.algs4.StdDraw;
import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.awt.*;
import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * A generic queue, implemented using a singly linked list.
 *
 * The LinkedListQueue class represents a first-in-first-out (FIFO) queue of generic items.
 *
 * It supports the usual enqueue and dequeue operations, along with methods for peeking
 * at the first item, testing if the queue is empty, and iterating through the items
 * in FIFO order.
 *
 * This implementation uses a singly linked list with a non-static nested class for linked-list
 * nodes. The enqueue, dequeue, peek, size and isEmpty operations all take constant time in the
 * worst case.
 */
public class LinkedListQueue<Item> implements Iterable<Item> {

    private Node first;         // beginning of queue
    private Node last;          // end of queue
    private int n;              // number of elements on queue

    // Helper linked list class.
    private class Node {
        private Item item;
        private Node next;
    }

    // Check internal invariants.
    private boolean check() {
        if (n < 0) {
            return false;
        } else if (n == 0) {
            if (first != null)
                return false;
            if (last != null)
                return false;
        } else if (n == 1) {
            if (first == null || last == null)
                return false;
            if (first != last)
                return false;
            if (first.next != null)
                return false;
        } else {
            if (first == null || last == null)
                return false;
            if (first == last)
                return false;
            if (first.next == null)
                return false;
            if (last.next != null)
                return false;

            // Check internal consistency of instance variable n.
            int numberOfNodes = 0;
            for (Node x = first; x != null && numberOfNodes <= n; x = x.next) {
                numberOfNodes++;
            }
            if (numberOfNodes != n)
                return false;

            // Check internal consistency of instance variable last.
            Node lastNode = first;
            while (lastNode.next != null) {
                lastNode = lastNode.next;
            }
            if (last != lastNode) return false;
        }
        return true;
    }

    /**
     * Initializes an empty queue.
     */
    public LinkedListQueue() {
        first = null;
        last = null;
        n = 0;
    }

    /**
     * Is this queue empty?
     *
     * @return true if this queue is empty; false otherwise
     */
    public boolean isEmpty() {
        return first == null;
    }

    /**
     * Return the number of items in this queue.
     *
     * @return the number of items in this queue
     */
    public int size() {
        return n;
    }

    /**
     * Returns the item least recently added to this queue.
     *
     * @return the item least recently added to this queue
     */
    public Item peek() {
        if (isEmpty())
            throw new NoSuchElementException("Queue underflow");
        return first.item;
    }

    /**
     * Adds the item to this queue.
     *
     * @param item the item to add
     */
    public void enqueue(Item item) {
        Node old = last;
        last = new Node();
        last.item = item;
        last.next = null;
        if (isEmpty())
            first = last;
        else
            old.next = last;
        n++;
        assert check();
    }

    /**
     * Remove and returns the item on this queue that was least recently added.
     *
     * @return the item on this queue that was least recently added
     */
    public Item dequeue() {
        if (isEmpty()) throw new NoSuchElementException("Queue underflow");
        Item item = first.item;
        first = first.next;
        n--;
        if (isEmpty())
            last = null;     // to avoid loitering
        assert check();
        return item;
    }

    /**
     * Returns a string representation of this queue.
     *
     * @return the sequence of items in FIFO order, separated by spaces
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (Item item : this)
            sb.append(item).append(" ");
        return sb.toString();
    }

    /**
     * Returns an iterator that iterates over the items in this queue in FIFO order.
     *
     * @return an iterator that iterates over the items in this queue in FIFO order
     */
    @Override
    public Iterator<Item> iterator() {
        return new LinkedIterator();
    }

    // An iterator, doesn't implement remove() since it's optional.
    private class LinkedIterator implements Iterator<Item> {
        private Node current = first;

        @Override
        public boolean hasNext() {
            return current != null;
        }

        @Override
        public Item next() {
            if (!hasNext())
                throw new NoSuchElementException();
            Item item = current.item;
            current = current.next;
            return item;
        }
    }

    public static void main(String[] args) {
        if (args.length < 1)
            System.exit(1);
        LinkedListQueue<String> queue = new LinkedListQueue<>();
        String[] ss = args[0].split(" ");
        for (String item : ss) {
            if (!item.equals("-"))
                queue.enqueue(item);
            else if (!queue.isEmpty())
                System.out.print(queue.dequeue() + " ");
        }

        System.out.println("\nFirst element '" + queue.peek() + "'");
        queue.forEach((node) -> StdOut.print(node + " "));
        StdOut.println("(" + queue.size() + " left on queue)");
    }
}
