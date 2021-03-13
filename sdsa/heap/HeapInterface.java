package heap;

/**
 * Interface common to heap data structures.
 *
 * Heaps are tree-like structures that allow storing node in a specific way. Each node
 * corresponds to an element and has one parent node (except for the root) and at most two
 * children nodes. Every node contains a key, and those keys indicate hwo the tree shall
 * be build. For a min-heap, the key of a node shall be greater than or equal to its parent's
 * and lower than or equal to its children's (the opposite rule applies to a max-heap).
 *
 * All heap-related operations (inserting or deleting an node, extracting the min or max) are
 * performed in o(log n) time.
 */
public interface HeapInterface {
    /**
     * @return the top node in the heap, the one with lowest key for min-heap or with he highest
     *          key for max-heap
     * @throws EmptyHeapException if heap is empty
     */
    HeapNode getNode() throws EmptyHeapException;

    /**
     * Inserts an element in the heap. Adds it to the end and toggle it until
     * it finds its right position.
     *
     * @param node an instance of the HeapNode class
     */
    void insertNode(HeapNode node);

    /**
     * Delete an element in the heap.
     *
     * @param index int containing the position in the heap of the element to be deleted
     */
    void deleteNode(int index);
}
