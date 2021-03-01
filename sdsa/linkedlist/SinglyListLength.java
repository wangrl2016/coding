package linkedlist;

public class SinglyListLength {
    Node head;  // head of list

    class Node {
        int data;
        Node next;

        Node(int d) {
            data = d;
            next = null;
        }
    }

    // Returns count of nodes in linked list.
    // 迭代
    public int getCount() {
        Node temp = head;
        int count = 0;
        while (temp != null) {
            count++;
            temp = temp.next;
        }
        return count;
    }

    // 递归
    public int getCountRecursive(Node node) {
        if (node == null)
            return 0;

        return 1 + getCountRecursive(node.next);
    }

    public void push(int data) {
        Node node = new Node(data);
        node.next = head;
        head = node;
    }

    public static void main(String[] args) {
        SinglyListLength list = new SinglyListLength();
        for (int i = 0; i < 5; i++)
            list.push(i);
        System.out.println("Count using iterative is "
                + list.getCount());
        System.out.println("Count using recursive is "
                + list.getCountRecursive(list.head));
    }
}
