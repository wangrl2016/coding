import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * A generic bag or multiset, implemented using a singly linked list.
 */
public class LinkedListBag<Item> implements Iterable<Item> {
    private Node first; // beginning of bag
    private int n;      // number of elements in bag

    // Helper linked list class.
    private class Node {
        private Item item;
        private Node next;
    }

    /**
     * Initializes an empty bag.
     */
    public LinkedListBag() {
        first = null;
        n = 0;
    }

    /**
     * Is this bag empty?
     *
     * @return true if this bag is empty; false otherwise
     */
    public boolean isEmpty() {
        return first == null;
    }

    /**
     * Returns the number of items in this bag.
     *
     * @return the number of items in this bag
     */
    public int size() {
        return n;
    }

    /**
     * Adds the item to this bag.
     *
     * @param item the item to add to this bag
     */
    public void add(Item item) {
        Node old = first;
        first = new Node();
        first.item = item;
        first.next = old;
        n++;
    }

    /**
     * @return an iterator that iterates over the items in the bag
     */
    @Override
    public Iterator<Item> iterator() {
        return new LinkedListIterator();
    }

    // An iterator over a linked list
    private class LinkedListIterator implements Iterator<Item> {
        Node current = first;

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
        LinkedListBag<String> bag = new LinkedListBag<>();
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
