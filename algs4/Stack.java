import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * A generic stack, implemented using a singly linked list.
 * Each stack element is of type Item.
 * <p>
 * Input: to be or not to - be - - that - - - is
 */

public class Stack<Item> implements Iterable<Item> {

    private Node<Item> first;       // top of stack
    private int n;      // size of the stack

    // Helper linked list class.
    private static class Node<Item> {
        private Item item;
        private Node<Item> next;
    }

    public Stack() {
        first = null;
        n = 0;
    }

    public boolean isEmpty() {
        return first == null;
    }

    public int size() {
        return n;
    }

    /**
     * Adds the item to this stack.
     *
     * @param item the item to add
     */
    public void push(Item item) {
        Node<Item> old = first;
        first = new Node<>();
        first.item = item;
        first.next = old;
        n++;
    }

    /**
     * Removes and returns the item most recently added to this stack.
     *
     * @return the item most recently added
     */
    public Item pop() {
        if (isEmpty()) throw new NoSuchElementException("Stack underflow");
        Item item = first.item;     // save item to return
        first = first.next;     // delete first node
        n--;
        return item;            // return the saved item
    }


    /**
     * Returns an iterator to this stack that iterates through the items in LIFO order.
     *
     * @return an iterator to this stack that iterates through the items in LIFO order
     */
    @Override
    public Iterator<Item> iterator() {
        return new LinkedIterator(first);
    }

    // An iterator, doesn't implement remove() since it's optional.
    private class LinkedIterator implements Iterator<Item> {
        private Node<Item> current;

        public LinkedIterator(Node<Item> first) {
            current = first;
        }

        @Override
        public boolean hasNext() {
            return current != null;
        }

        @Override
        public Item next() {
            if (!hasNext()) throw new NoSuchElementException();
            Item item = current.item;
            current = current.next;
            return item;
        }
    }

    public static void main(String[] args) {
        Stack<String> stack = new Stack<>();
        while (!StdIn.isEmpty()) {
            String item = StdIn.readString();
            if (!item.equals("-"))
                stack.push(item);
            else if (!stack.isEmpty()) {
                StdOut.print(stack.pop() + " ");
            }
        }

        stack.forEach((node) -> StdOut.print(node + " "));

        StdOut.println("(" + stack.size() + " left on stack)");
    }
}
