package linkedlist;

public class IsCircularList {
    static class Node {
        int data;
        Node next;
    }

    static boolean isCircular(Node head) {
        if (head == null)
            return true;

        // Next of head.
        Node node = head.next;
        while (node != null && node != head)
            node = node.next;

        // If loop stopped because of circular condition.
        return (node == head);
    }

    static Node newNode(int data) {
        Node temp = new Node();
        temp.data = data;
        temp.next = null;
        return temp;
    }

    public static void main(String args[]) {
        Node head = newNode(1);
        head.next = newNode(2);
        System.out.println(isCircular(head) ? "Yes" : "No");

        head.next.next = head;
        System.out.println(isCircular(head) ? "Yes" : "No");
    }
}
