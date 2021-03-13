package heap;

import java.util.ArrayList;
import java.util.List;

public class MinHeap implements HeapInterface {
    private final List<HeapNode> minHeap;

    public MinHeap(List<HeapNode> list) {
        minHeap = new ArrayList<>();
        for (HeapNode node : list) {
            if (node != null)
                insertNode(node);
            else
                System.out.println("Null node");
        }
    }

    // 删除根节点
    private HeapNode extractMin() {
        HeapNode result = minHeap.get(0);
        deleteNode(0);
        return result;
    }

    @Override
    public HeapNode getNode() throws EmptyHeapException {
        try {
            return extractMin();
        } catch (Exception e) {
            throw new EmptyHeapException("Heap is empty");
        }
    }

    // Get the element at a given index.
    public HeapNode getNode(int index) {
        if ((index < 0) || (index >= minHeap.size()))
            throw new IndexOutOfBoundsException("Index out of heap range");
        return minHeap.get(index);
    }

    // Get the key of the element at a given index.
    private double getNodeKey(int index) {
        return minHeap.get(index).getKey();
    }

    // Swaps two element in the heap.
    private void swap(int index1, int index2) {
        HeapNode tempNode = minHeap.get(index1);
        minHeap.set(index1, minHeap.get(index2));
        minHeap.set(index2, tempNode);
    }

    // Toggle an element up to its right place as long as
    // its key is lower than its parent's.
    private void toggleUp(int index) {
        double key = minHeap.get(index).getKey();
        while (getNodeKey((int) Math.floor(index / 2.0)) > key) {
            swap(index, (int) Math.floor(index / 2.0));
            index = (int) Math.floor(index / 2.0);
        }
    }

    @Override
    public void insertNode(HeapNode node) {

    }

    @Override
    public void deleteNode(int index) {
        if (minHeap.isEmpty()) {
            try {
                throw new EmptyHeapException("Attempt to delete an element from an empty heap");
            } catch (EmptyHeapException e) {
                e.printStackTrace();
            }
        }

        if ((index >= minHeap.size()) || (index < 0))
            throw new IndexOutOfBoundsException("Index out of heap range");

        
    }

    public static void main(String[] args) {


    }
}
