package linkedlist;

public class DoubleListDeleted {
    Node head;

    class Node {
        int data;
        Node prev;
        Node next;

        Node(int d) {
            data = d;
        }
    }

    // Adding a node at the front of the list.
    public void push(int data) {
        Node newNode = new Node(data);

        newNode.next = head;
        newNode.prev = null;

        if (head != null)
            head.prev = newNode;

        head = newNode;
    }

    void deleteNode(Node del) {
        if (head == null || del == null)
            return;

        // If node to be deleted is head node.
        if (head == del) {
            head = del.next;
        }

        // Change next only if node the be deleted is not the last node.
        if (del.next != null) {
            del.next.prev = del.prev;
        }

        // Change prev only if node to be delete is not the first node.
        if (del.prev != null) {
            del.prev.next = del.next;
        }
    }

    public static void main(String[] args) {
        DoubleListDeleted list = new DoubleListDeleted();
        // 10 -> 8 -> 4 -> 2 -> null
        list.push(2);
        list.push(4);
        list.push(8);
        list.push(10);

        // 8 -> 4 -> 2 -> null
        list.deleteNode(list.head);

        // 8 -> 2 -> null
        list.deleteNode(list.head.next);

        Node node = list.head;
        while (node != null) {
            System.out.print(node.data + " ");
            node = node.next;
        }
    }
}
