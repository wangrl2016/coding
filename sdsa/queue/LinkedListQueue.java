package queue;

public class LinkedListQueue {
    Node front, rear;

    static class Node {
        int key;
        Node next;

        public Node(int key) {
            this.key = key;
            this.next = null;
        }
    }

    public LinkedListQueue() {
        this.front = this.rear = null;
    }

    void enqueue(int key) {
        Node temp = new Node(key);

        // If queue is empty, then new node is front and rear both.
        if (this.rear == null) {
            this.front = this.rear = temp;
            return;
        }

        // Add the new node at the end of queue and change rear.
        this.rear.next = temp;
        this.rear = temp;
    }

    // Method to remove an key front queue.
    void dequeue() {
        // If queue is empty, return null.
        if (this.front == null)
            return;

        // Store previous front and mve front one node ahead.
        Node temp = this.front;
        this.front = this.front.next;

        // If front becomes null, then change rear also as null.
        if (this.front == null)
            this.rear = null;
    }

    int front() {
        if (front != null) {
            return front.key;
        } else {
            return Integer.MIN_VALUE;
        }
    }

    int rear() {
        if (rear != null) {
            return rear.key;
        } else {
            return Integer.MIN_VALUE;
        }
    }


    public static void main(String[] args) {
        LinkedListQueue queue = new LinkedListQueue();
        queue.enqueue(10);
        queue.enqueue(20);
        queue.dequeue();
        queue.enqueue(30);
        System.out.println("Queue front " + queue.front());
        System.out.println("Queue rear " + queue.rear());
    }
}
