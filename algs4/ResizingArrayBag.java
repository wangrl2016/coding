import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Bag implementation with a resizing array.
 *
 * The ResizingArrayBag class represents a bag (or multiset) of generic items.
 * It supports insertion and iterating over the items in arbitrary order.
 *
 * This implementation uses a resizing array.
 *
 * The add operation takes constant amortized time; the isEmpty and size operations
 * takes constant time. Iteration takes time proportional to the number of items.
 */
public class ResizingArrayBag<Item> implements Iterable<Item> {
    // Initial capacity of underlying resizing array.
    private static final int INIT_CAPACITY = 8;

    private Item[] a;       // array of items
    private int n;          // number of elements on bag

    /**
     * Initializes an empty bag.
     */
    @SuppressWarnings("unchecked")
    public ResizingArrayBag() {
        a = (Item[]) new Object[INIT_CAPACITY];
        n = 0;
    }

    /**
     * Is this bag emtpy?
     *
     * @return true if this bag is empty; false otherwise
     */
    public boolean isEmpty() {
        return n == 0;
    }

    /**
     * Return the number of items in this bag.
     *
     * @return the number of items in this bag
     */
    public int size() {
        return n;
    }

    // Resize the underlying array holding the elements.
    @SuppressWarnings("unchecked")
    public void resize(int capacity) {
        assert capacity >= n;
        Item[] copy = (Item[]) new Object[capacity];
        if (n >= 0)
            System.arraycopy(a, 0, copy, 0, n);
        a = copy;
    }

    /**
     * Adds the item to this bag.
     *
     * @param item the item to add to this bag
     */
    public void add(Item item) {
        if (n == a.length) resize(2 * a.length);
        a[n++] = item;
    }

    /**
     * Return an iterator that iterates over the items in the bag in arbitrary order.
     *
     * @return an iterator that iterates over the items in the bag in arbitrary order
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
            return a[i++];
        }
    }

    public static void main(String[] args) {
        ResizingArrayBag<String> bag = new ResizingArrayBag<>();
        String[] ss = args[0].split(" ");
        for (String s : ss)
            bag.add(s);

        System.out.println("Bag is " + (bag.isEmpty() ? "" : "not") + " empty");
        System.out.println("Bag size " + bag.size());
        for (String s : bag)
            System.out.print(s + " ");
        System.out.println();
    }
}
