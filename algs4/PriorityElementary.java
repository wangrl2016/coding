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

    // Is v < w ?
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }

    public static void main(String[] args) {
        Integer[] a = {2, 10, 4, 1};

        // ordered array
        PriorityQueueOrderedArray<Integer> orderedArray =
                new PriorityElementary().new PriorityQueueOrderedArray<>(5);

        for (Integer integer : a)
            orderedArray.insert(integer);

        while (!orderedArray.isEmpty())
            StdOut.print(orderedArray.removeMax() + " ");
    }
}
