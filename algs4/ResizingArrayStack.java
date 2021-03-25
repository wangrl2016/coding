import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Stack implementation with a resizing array.
 *
 * The ResizingArrayStack class represents a last-in-first-out (LIFO) stack of generic items.
 *
 * It supports the usual push and pop operations, along with methods for peeking at the
 * top item, testing if the stack is empty, and iterating through the items in LIFO order.
 *
 * This implementation uses a resizing array, which double the underlying array
 * when it is full and halves the underlying array when it is one-quarter full.
 *
 * The push and pop operations take constant amortized time.
 * The size, peek, and isEmpty operations takes constants time in the worse case.
 */
public class ResizingArrayStack<Item> implements Iterable<Item> {

    // Initial capacity of underlying resizing array.
    private static final int INIT_CAPACITY = 8;

    private Item[] a;   // array of items
    private int n;      // number of elements on stack

    /**
     * Initializes an empty stack.
     */
    @SuppressWarnings("unchecked")
    public ResizingArrayStack() {
        a = (Item[]) new Object[INIT_CAPACITY];
        n = 0;
    }

    /**
     * Is this stack empty?
     *
     * @return true if this stack is empty; false otherwise
     */
    public boolean isEmpty() {
        return n == 0;
    }

    public int size() {
        return n;
    }

    // Resize the underlying array holding the elements.
    @SuppressWarnings("unchecked")
    private void resize(int capacity) {
        assert capacity >= n;

        Item[] copy = (Item[]) new Object[capacity];
        if (n >= 0) System.arraycopy(a, 0, copy, 0, n);
        a = copy;
    }

    /**
     * Adds the item to this stack.
     *
     * @param item the item to add
     */
    public void push(Item item) {
        // n指向下一个空位置
        if (n == a.length)
            resize(2 * a.length);   // double size of array if necessary
        a[n++] = item;  // add item
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
        // Shrink size of array if necessary.
        if (n > 0 && n == a.length / 4)
            resize(a.length / 2);
        return item;
    }

    /**
     * Return (but does not remove) the item most recently added to this stack.
     *
     * @return the item most recently added to this stack.
     */
    public Item peek() {
        if (isEmpty())
            throw new NoSuchElementException("Stack underflow");
        return a[n - 1];
    }

    /**
     * Returns an iterator to this stack that iterates through the items in LIFO order.
     *
     * @return an iterator to this stack that iterates through the items in LIFO order
     */
    @Override
    public Iterator<Item> iterator() {
        return new ReverseArrayIterator();
    }

    // 倒序遍历
    // An iterator, doesn't implement remove()since it's optional.
    private class ReverseArrayIterator implements Iterator<Item> {
        // 指向最近加入的元素位置
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
        if (args.length < 1)
            System.exit(1);
        ResizingArrayStack<String> stack = new ResizingArrayStack<>();
        String[] ss = args[0].split(" ");
        for (String item : ss)
            if (!item.equals("-"))
                stack.push(item);
            else if (!stack.isEmpty())
                System.out.print(stack.pop() + " ");

        System.out.println("\nTop element '" + stack.peek() + "'");
        stack.forEach((item) -> System.out.print(item + " "));
        System.out.println("(" + stack.size() + " left on stack)");
    }
}
