package linkedlist;
public class SinglyLinkedListInserted {
    Node head;

    class Node {
        int data;
        Node next;

        Node(int d) {
            data = d;
            next = null;
        }
    }

    /**
     * Insert a new Node at front of the list. This method is
     * defined inside LinkedList class shown above.
     */
    public void insertFront(int newData) {
        // 1. Allocate the Node and put in the data.
        Node newNode = new Node(newData);

        // 2. Make next of new Node as head.
        newNode.next = head;

        // 3. Move the head to point to new Node.
        head = newNode;
    }

    public void insertAfter(Node prevNode, int newData) {
        // 1. Check if the given Node is null.
        if (prevNode == null) {
            System.out.println("The given previous node cannot be null");
            return;
        }

        // 2. Allocate the Node and put in the data.
        Node newNode = new Node(newData);

        // 3. Make next of new Node as next of prevNode.
        newNode.next = prevNode.next;

        // 4. Make next of prevNode as newNode
        prevNode.next = newNode;
    }

    public void insertEnd(int newData) {
        Node newNode = new Node(newData);
        if (head == null) {
            head = new Node(newData);
            return;
        }

        newNode.next = null;

        Node last = head;
        while (last.next != null)
            last.next = last.next;

        last.next = newNode;
        return;
    }

    public static void main(String[] args) {
        SinglyLinkedListInserted list = new SinglyLinkedListInserted();

        // 6 -> null
        list.insertEnd(6);

        // 7 -> 6 -> null
        list.insertFront(7);

        // 1 -> 7 -> 6 -> null
        list.insertFront(1);

        // 1 -> 7 -> 8 -> 6 -> null
        list.insertAfter(list.head.next, 8);

        Node node = list.head;
        while (node != null) {
            System.out.print(node.data + " ");
            node = node.next;
        }
    }
}
