package linkedlist;

public class CircularLinkedList {
    static class Node {
        int data;
        Node next;
    }

    // Function to insert a node at the beginning of a
    // circular linked list.
    static Node push(Node head, int data) {
        Node newNode = new Node();
        newNode.data = data;
        newNode.next = head;

        // If linked list is not null
        // then set the next of last node.
        // 最后一个元素指向最开始的元素
        Node temp = head;
        if (head != null) {
            while (temp.next != head)
                temp = temp.next;
            temp.next = newNode;
        } else {
            // 指向自己
            newNode.next = newNode;
        }
        head = newNode;
        return head;
    }

    public static void main(String[] args) {

        Node head = push(null, 12);
        head = push(head, 56);
        head = push(head, 2);
        head = push(head, 11);

        Node temp = head;
        if (head != null) {
            do {
                System.out.print(temp.data + " ");
                temp = temp.next;
            } while (temp != head);
        }
    }
}
