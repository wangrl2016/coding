import edu.princeton.cs.algs4.StdIn;
import edu.princeton.cs.algs4.StdOut;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * This implementation uses a resizing array, which double the underlying array
 * when it is full and halves the underlying array when it is one-quarter full.
 *
 * Input: to be or not to - be - - that - - - is
 */
public class StackArray<Item> implements Iterable<Item> {

    // Initial capacity of underlying resizing array.
    private static final int INIT_CAPACITY = 8;

    private Item[] a;   // array of items
    private int n;      // number of elements on stack

    public StackArray() {
        a = (Item[]) new Object[INIT_CAPACITY];
        n = 0;
    }

    public boolean isEmpty() {
        return n == 0;
    }

    public int size() {
        return n;
    }

    // Resize the underlying array holding the elements.
    private void resize(int capacity) {
        assert capacity >= n;

        Item[] copy = (Item[]) new Object[capacity];
        for (int i = 0; i < n; i++) {
            copy[i] = a[i];
        }
        a = copy;
    }

    /**
     * Adds the item to this stack.
     *
     * @param item the item to add
     */
    public void push(Item item) {
        if (n == a.length) resize(2 * a.length);
        a[n++] = item;
    }

    /**
     * Removes and returns the item most recently added to this stack.
     *
     * @return the item most recently added
     */
    public Item pop() {
        if (isEmpty())
            throw new NoSuchElementException("Stack underflow");
        Item item = a[n - 1];
        a[n - 1] = null;
        n--;
        if (n > 0 && n == a.length / 4)
            resize(a.length / 2);
        return item;
    }

    public Item peek() {
        if (isEmpty())
            throw new NoSuchElementException("Stack underflow");
        return a[n - 1];
    }


    @Override
    public Iterator<Item> iterator() {
        return new ReverseArrayIterator();
    }

    private class ReverseArrayIterator implements Iterator<Item> {
        private int i;

        public ReverseArrayIterator() {
            i = n - 1;
        }

        @Override
        public boolean hasNext() {
            return i >= 0;
        }

        @Override
        public Item next() {
            if (!hasNext())
                throw new NoSuchElementException();
            return a[i--];
        }
    }

    public static void main(String[] args) {
        StackArray<String> stack = new StackArray<>();
        while (!StdIn.isEmpty()) {
            String item = StdIn.readString();
            if (!item.equals("-"))
                stack.push(item);
            else if (!stack.isEmpty())
                StdOut.print(stack.pop() + " ");
        }
        for (String item : stack)
            StdOut.print(item + " ");
        StdOut.println("(" + stack.size() + " left on stack)");
    }
}
