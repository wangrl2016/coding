import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * A generic stack, implemented using a singly linked list. Each stack element is of type Item.
 *
 * The LinkedListStack class represents a list-in-first-out (LIFO) stack of generic items.
 *
 * It supports the usual push and pop operations, along with methods for peeking at the top item,
 * testing if the stack is empty, and iterating through the items in LIFO order.
 *
 * This implementation uses a singly linked list with a non-static nested class for
 * linked-list nodes. The push, pop, peek, size and isEmpty operations all take constant time
 * in the worst case.
 */

public class LinkedListStack<Item> implements Iterable<Item> {

    private Node<Item> first;       // top of stack
    private int n;                  // size of the stack

    // Helper linked list class.
    private static class Node<Item> {
        private Item item;
        private Node<Item> next;
    }

    // 非静态内部类表示
    // private class StackNode {
    //    private Item item;
    //    private StackNode next;
    // }

    // 检查Stack是否合法
    // Check internal invariants.
    private boolean check() {
        // Check a few properties of instance variable 'first'.
        if (n < 0)
            return false;
        if (n == 0) {
            if (first != null)
                return false;
        } else if (n == 1) {
            if (first == null)
                return false;
            if (first.next != null)
                return false;
        } else {
            if (first == null)
                return false;
            if (first.next == null)
                return false;
        }

        // Check internal consistency of instance variable n.
        int numberOfNodes = 0;
        for (Node x = first; x != null && numberOfNodes <= n; x = x.next) {
            numberOfNodes++;
        }
        return numberOfNodes == n;
    }

    /**
     * Initializes an empty stack.
     */
    public LinkedListStack() {
        first = null;
        n = 0;
        assert check();
    }

    /**
     * Is this stack empty?
     *
     * @return true if this stack is empty; false otherwise
     */
    public boolean isEmpty() {
        return first == null;
    }

    /**
     * Returns the number of items in the stack.
     *
     * @return the number of items in the stack
     */
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
        if (isEmpty())
            throw new NoSuchElementException("Stack underflow");
        Item item = first.item;     // save item to return
        first = first.next;     // delete first node
        n--;
        assert check();
        return item;            // return the saved item
    }

    /**
     * Returns (but does not remove) the item most recently added to this stack.
     *
     * @return the item most recently added to this stack
     */
    public Item peek() {
        if (isEmpty())
            throw new NoSuchElementException("Stack underflow");
        return first.item;
    }

    /**
     * Return a string representation of this stack.
     *
     * @return the sequence of items in the stack in LIFO order, separated by spaces
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (Item item : this)
            sb.append(item).append(" ");
        return sb.toString();
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
        if (args.length < 1)
            System.exit(1);
        LinkedListStack<String> stack = new LinkedListStack<>();
        String[] ss = args[0].split(" ");
        for (String item : ss)
            if (!item.equals("-"))
                stack.push(item);
            else if (!stack.isEmpty())
                System.out.print(stack.pop() + " ");

        System.out.println("\nTop element '" + stack.peek() + "'");
        for (String item : stack)
            System.out.print(item + " ");
        System.out.println("(" + stack.size() + " left on stack)");
    }
}
