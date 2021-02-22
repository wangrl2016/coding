import edu.princeton.cs.algs4.StdOut;

/**
 * 2.4.3 Provide priority-queue implementations that support insert and remove the
 * maximum, one for each of the following underlying data structures: unordered array,
 * ordered array, unordered linked list, and linked list. Give a table of the worst-case
 * bounds for each operation for each of your four implementations.
 */
public class PriorityElementary {

    private class PriorityQueueOrderedArray<Key extends Comparable<Key>> {

        private Key[] priorityQueue;
        private int size = 0;

        PriorityQueueOrderedArray(int size) {
            priorityQueue = (Key[]) new Comparable[size];
        }

        public boolean isEmpty() {
            return size == 0;
        }

        public int size() {
            return size;
        }

        // O(N)
        public void insert(Key key) {
            if (size == priorityQueue.length)
                throw new RuntimeException("Priority queue is full");

            int i = size - 1;

            // insert sort
            while (i >= 0 && less(key, priorityQueue[i])) {
                priorityQueue[i + 1] = priorityQueue[i];
                i--;
            }

            priorityQueue[i + 1] = key;
            size++;
        }

        // O(1)
        public Key removeMax() {
            if (isEmpty())
                throw new RuntimeException("Priority queue underflow");

            Key maxValue = priorityQueue[size - 1];
            priorityQueue[size - 1] = null;
            size--;
            return maxValue;
        }
    }

    private class PriorityQueueUnorderedArray<Key extends Comparable<Key>> {
        private Key[] priorityQueue;
        private int size = 0;

        PriorityQueueUnorderedArray(int size) {
            priorityQueue = (Key[]) new Comparable[size];
        }

        public boolean isEmpty() {
            return size == 0;
        }

        public int size() {
            return size;
        }

        // O(1)
        public void insert(Key key) {
            if (size == priorityQueue.length)
                throw new RuntimeException("Priority queue is full");
            priorityQueue[size] = key;
            size++;
        }

        // O(N)
        public Key removeMax() {
            if (isEmpty())
                throw new RuntimeException("Priority queue underflow");

            int maxValueIndex = 0;

            for (int i = 1; i < size; i++)
                if (less(priorityQueue[maxValueIndex], priorityQueue[i]))
                    maxValueIndex = i;

            exchange(priorityQueue, maxValueIndex, size - 1);

            Key maxValue = priorityQueue[size - 1];
            priorityQueue[size - 1] = null;
            size--;
            return maxValue;
        }
    }

    private class PriorityQueueOrderedLinkedList<Key extends Comparable<Key>> {
        // 双向链表
        private class Node {
            Key key;
            Node previous;
            Node next;
        }

        // 链表的大小
        private int size = 0;

        public boolean isEmpty() {
            return size == 0;
        }

        public int size() {
            return size;
        }

        // 最小值
        private Node first;
        // 最大值
        private Node last;

        // O(N)
        // 有顺序地插入
        public void insert(Key key) {
            // 0 or 1 elements in the list.
            if (size == 0 || less(key, first.key)) {
                Node oldFirst = first;

                first = new Node();
                first.key = key;
                first.next = oldFirst;

                if (oldFirst != null) {
                    oldFirst.previous = first;
                }

                if (first.next == null) {
                    last = first;
                }
            } else {
                Node current = first;

                // 寻找插入的位置
                while (current.next != null && less(current.next.key, key))
                    current = current.next;

                Node newNode = new Node();
                newNode.key = key;
                newNode.next = current.next;
                current.next = newNode;
                newNode.previous = current;

                if (newNode.next == null) {
                    last = newNode;
                } else {
                    newNode.next.previous = newNode;
                }
            }
            size++;
        }

        // O(1)
        public Key removeMax() {
            if (isEmpty())
                throw new RuntimeException("Priority queue underflow");

            Key maxValue = last.key;

            last = last.previous;

            if (last != null) {
                last.next = null;
            }

            size--;
            return maxValue;
        }
    }

    // Exchange a[i] and a[j].
    private static void exchange(Object[] a, int i, int j) {
        Object swap = a[i];
        a[i] = a[j];
        a[j] = swap;
    }

    // Is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }

    public static void main(String[] args) {
        Integer[] a = {2, 10, 4, 1, 5};

        // ordered array
        PriorityQueueOrderedArray<Integer> orderedArray =
                new PriorityElementary().new PriorityQueueOrderedArray<>(5);

        for (Integer integer : a)
            orderedArray.insert(integer);

        while (!orderedArray.isEmpty())
            StdOut.print(orderedArray.removeMax() + " ");

        StdOut.println();

        // unordered array
        PriorityQueueUnorderedArray<Integer> unorderedArray =
                new PriorityElementary().new PriorityQueueUnorderedArray<>(5);

        for (Integer integer : a)
            unorderedArray.insert(integer);

        while (!unorderedArray.isEmpty())
            StdOut.print(unorderedArray.removeMax() + " ");

        StdOut.println();

        // ordered linked list
        PriorityQueueOrderedLinkedList<Integer> orderedLinkedList =
                new PriorityElementary().new PriorityQueueOrderedLinkedList<>();

        for (Integer integer: a)
            orderedLinkedList.insert(integer);

        while (!orderedLinkedList.isEmpty())
            StdOut.print(orderedLinkedList.removeMax() + " ");

        StdOut.println();

        // unordered linked list


    }
}
