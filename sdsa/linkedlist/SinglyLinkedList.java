package linkedlist;

public class SinglyLinkedList {
    Node head;  // head of the list
    static class Node {
        int data;
        Node next;

        Node(int d) {
            data = d;
            next = null;
        }
    }

    public static void main(String[] args) {
        SinglyLinkedList list = new SinglyLinkedList();

        /**
         * Three nodes have been allocated dynamically.
         * We have references to these three blocks as head,
         * second and third.
         *
         *          list.head        second              third
         *              |                |                  |
         *              |                |                  |
         *          +----+------+     +----+------+     +----+------+
         *          | 1  | null |     | 2  | null |     |  3 | null |
         *          +----+------+     +----+------+     +----+------+
         */
        list.head = new Node(1);
        Node second = new Node(2);
        Node third = new Node(3);

        /**
         * Now next of the first Node refers to the second. So they
         * both are linked.
         *
         *          list.head        second              third
         *             |                |                  |
         *             |                |                  |
         *         +----+------+     +----+------+     +----+------+
         *         | 1  |  o-------->| 2  | null |     |  3 | null |
         *         +----+------+     +----+------+     +----+------+
         */
        list.head.next = second;

        /**
         * Now next of the second Node refers to third. So all three nodes are linked.
         *
         *          list.head        second              third
         *             |                |                  |
         *             |                |                  |
         *         +----+------+     +----+------+     +----+------+
         *         | 1  |  o-------->| 2  |  o-------->|  3 | null |
         *         +----+------+     +----+------+     +----+------+
         */
        second.next = third;

        Node current = list.head;

        while (current != null) {
            System.out.print(current.data + " ");
            current = current.next;
        }
    }
}
