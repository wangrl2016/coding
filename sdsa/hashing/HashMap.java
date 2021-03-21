package hashing;

public class HashMap {
    private int hashSize;
    private LinkedList[] buckets;

    public HashMap(int hashSize) {
        buckets = new LinkedList[hashSize];
        for (int i = 0; i < hashSize; i++) {
            buckets[i] = new LinkedList();
        }
        this.hashSize = hashSize;
    }

    // 求余获取hash值
    public int hashing(int key) {
        int hash = key % hashSize;
        if (hash < 0)
            hash += hashSize;
        return hash;
    }

    public void insertHash(int key) {
        int hash = hashing(key);
        buckets[hash].insert(key);
    }

    public void deleteHash(int key) {
        int hash = hashing(key);
        buckets[hash].delete(key);
    }

    public void displayHashTable() {
        for (int i = 0; i < hashSize; i++) {
            System.out.printf("Bucket %d: ", i);
            System.out.println(buckets[i].display());
        }
    }

    // 创建单链表
    public static class LinkedList {
        private Node first;

        public LinkedList() {
            first = null;
        }

        public void insert(int key) {
            if (isEmpty()) {
                first = new Node(key);
                return;
            }

            Node temp = findEndRecur(first);
            temp.setNext(new Node(key));
        }

        public boolean isEmpty() {
            return first == null;
        }

        public Node findEnd(Node node) {
            if (isEmpty())
                return null;
            Node temp = node;
            while (temp.getNext() != null)
                temp = temp.getNext();
            return temp;
        }

        public Node findEndRecur(Node node) {
            if (node == null)
                return null;
            else if (node.getNext() == null) {
                return node;
            } else {
                return findEndRecur(node.getNext());
            }
        }

        private Node findKey(int key) {
            if (!isEmpty()) {
                return findKey(first, key);
            } else {
                return null;
            }
        }

        private Node findKey(Node n, int key) {
            if (n.getKey() == key)
                return n;
            else if (n.getNext() == null) {
                return null;
            } else {
                return findKey(n.getNext(), key);
            }
        }

        private void delete(int key) {
            if (!isEmpty()) {
                if (first.getKey() == key) {
                    first = null;
                } else {
                    delete(first, key);
                }
            }
        }

        private void delete(Node n, int key) {
            if (n.getNext().getKey() == key) {
                if (n.getNext().getNext() == null) {
                    n.setNext(null);
                } else {
                    n.setNext(n.getNext().getNext());
                }
            }
        }

        public String display() {
            return display(first);
        }

        private String display(Node n) {
            if (n == null) {
                return "null";
            } else {
                return n.getKey() + "->" + display(n.getNext());
            }
        }
    }

    public static class Node {
        private Node next;
        private int key;

        public Node(int key) {
            next = null;
            this.key = key;
        }

        public Node getNext() {
            return next;
        }

        public int getKey() {
            return key;
        }

        public void setNext(Node next) {
            this.next = next;
        }
    }

    public static void main(String[] args) {
        HashMap hash = new HashMap(10);
        for (int i = 0; i < 100; i++) {
            hash.insertHash(i);
        }
        hash.displayHashTable();
    }
}
