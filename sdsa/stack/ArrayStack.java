package stack;

public class ArrayStack {
    static final int MAX = 1000;
    int top;
    int a[] = new int[MAX];

    boolean isEmpty() {
        return (top < 0);
    }

    ArrayStack() {
        top = -1;
    }

    boolean push(int x) {
        if (top >= (MAX - 1)) {
            System.out.println("Stack overflow");
            return false;
        } else {
            a[++top] = x;
            return true;
        }
    }

    int pop() {
        if (top < 0) {
            System.out.println("Stack underflow");
            return 0;
        } else {
            return a[top--];
        }
    }

    int peek() {
        if (top < 0) {
            System.out.println("Stack underflow");
            return 0;
        } else {
            return a[top];
        }
    }

    public static void main(String[] args) {
        ArrayStack stack = new ArrayStack();
        stack.push(10);
        stack.push(20);
        System.out.println(stack.pop() + " popped from stack");
    }
}
