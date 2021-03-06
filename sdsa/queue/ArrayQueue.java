package queue;

public class ArrayQueue {
    int front, rear, size;
    int capacity;
    int array[];

    public ArrayQueue(int capacity) {
        this.capacity = capacity;
        front = this.size = 0;
        rear = capacity - 1;
        array = new int[this.capacity];
    }

    // Queue is full when size becomes equal to the capacity.
    boolean isFull(ArrayQueue queue) {
        return (queue.size == queue.capacity);
    }

    // Queue is empty when size is 0.
    boolean isEmpty(ArrayQueue queue) {
        return queue.size == 0;
    }

    // Method to add an item to the queue.
    // It changes rear and size.
    void enqueue(int item) {
        if (isFull(this))
            return;

        this.rear = (this.rear + 1) % this.capacity;
        this.array[this.rear] = item;
        this.size = this.size + 1;
    }

    // Method to remove an item from queue.
    // It changes front and size.
    int dequeue() {
        if (isEmpty(this))
            return Integer.MIN_VALUE;

        int item = this.array[this.front];
        this.front = (this.front + 1) % this.capacity;
        this.size = this.size - 1;
        return item;
    }

    // Method to get front of queue.
    int front() {
        if (isEmpty(this))
            return Integer.MIN_VALUE;
        return this.array[this.front];
    }

    // Method to get rear of queue.
    int rear() {
        if (isEmpty(this))
            return Integer.MIN_VALUE;
        return this.array[this.rear];
    }

    public static void main(String[] args) {
        ArrayQueue queue = new ArrayQueue(1000);
        queue.enqueue(10);
        queue.enqueue(20);
        queue.enqueue(30);
        System.out.println(queue.dequeue() + " dequeued from queue");
        System.out.println("Front item is " + queue.front());
        System.out.println("Rear item is " + queue.rear());
    }
}
