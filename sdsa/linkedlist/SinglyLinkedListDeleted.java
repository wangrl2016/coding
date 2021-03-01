package linkedlist;

public class SinglyLinkedListDeleted {
    Node head;  // head of list

    class Node {
        int data;
        Node next;

        Node(int d) {
            data = d;
            next = null;
        }
    }

    void deleteNode(int key) {
        // Store head node.
        Node temp = head, prev = null;

        // If head node itself holds the key to be deleted.
        if (temp != null && temp.data == key) {
            head = temp.next;
            return;
        }

        // Search for the key to be deleted, keep track of
        // the previous node as we need to change.
        while (temp != null && temp.data != key) {
            prev = temp;
            temp = temp.next;
        }

        // If key was not present in linked list.
        if (temp == null)
            return;

        // Unlink the node from linked list.
        prev.next = temp.next;
    }

    // 链表递归删除输入存在的元素
    public Node deleteNode(Node head, int val) {
        if (head == null)
            return null;

        head.next = deleteNode(head.next, val);
        return head.data == val ? head.next : head;
    }

    public void push(int data) {
        Node node = new Node(data);
        node.next = head;
        head = node;
    }

    public static void main(String[] args) {
        SinglyLinkedListDeleted list = new SinglyLinkedListDeleted();

        list.push(8);
        list.push(9);
        list.push(7);
        list.push(6);

        Node current = list.head;
        while (current != null) {
            System.out.print(current.data + " ");
            current = current.next;
        }
        System.out.println();

        list.deleteNode(9);

        list.head = list.deleteNode(list.head, 7);

        current = list.head;
        while (current != null) {
            System.out.print(current.data + " ");
            current = current.next;
        }
    }
}
