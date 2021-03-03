package linkedlist;

public class DoubleLinkedList {
    Node head;

    class Node {
        int data;
        Node prev;
        Node next;

        Node(int d) {
            data = d;
        }
    }

    // Add a node at the front of the list.
    public void addFront(int data) {
        Node newNode = new Node(data);

        // Make next of new node as head and previous as null.
        newNode.next = head;
        newNode.prev = null;

        // Change prev of head node to new node.
        if (head != null)
            head.prev = newNode;

        // Move the head to point to the new node.
        head = newNode;
    }

    // Given a node as prevNode, insert a new node after the given node.
    public void insertAfter(Node prevNode, int data) {
        assert prevNode != null;

        Node newNode = new Node(data);

        // Make next of new node as next of prevNode.
        newNode.next = prevNode.next;

        // Make the next of prevNode as newNode.
        prevNode.next = newNode;

        // Make prevNode as previous of newNode.
        newNode.prev = prevNode;

        // Change previous of new node's next node.
        if (newNode.next != null)
            newNode.next.prev = newNode;
    }

    // Add a node at the end of the list.
    void append(int data) {
        Node newNode = new Node(data);
        newNode.next = null;

        if (head == null) {
            newNode.prev = null;
            head = newNode;
            return;
        }

        Node last = head;
        // Traverse till the last node.
        while (last.next != null)
            last = last.next;

        // Change the next of last node.
        last.next = newNode;

        // Make last node as previous of new node.
        newNode.prev = last;
    }

    public static void main(String[] args) {
        DoubleLinkedList list = new DoubleLinkedList();

        // 6 -> null
        list.append(6);

        // 7 -> 6 -> null
        list.addFront(7);

        // 1 -> 7 -> 6 -> null
        list.addFront(1);

        // 1 -> 7 -> 8 -> 6 -> null
        list.insertAfter(list.head.next, 8);

        Node node = list.head;
        while (node != null) {
            System.out.print(node.data + " ");
            node = node.next;
        }
    }
}
