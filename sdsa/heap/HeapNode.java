package heap;

/**
 * Class for heap node.
 * <p>
 * A heap node contains two attributes: a key which will be used to build the tree (int or
 * double, either primitive type or object) and any kind of IMMUTABLE object the user sees fit to
 * carry any information he/she likes. Be aware that the use of a mutable object might jeopardize
 * the integrity of this information.
 */
public class HeapNode {
    private final int key;
    private final Object additionalInfo;

    public HeapNode(int key, Object info) {
        this.key = key;
        this.additionalInfo = info;
    }

    public HeapNode(int key) {
        this.key = key;
        this.additionalInfo = null;
    }

    public int getKey() {
        return key;
    }

    public Object getAdditionalInfo() {
        return additionalInfo;
    }

    @Override
    public String toString() {
        return "HeapNode{" +
                "key=" + key +
                ", additionalInfo=" + additionalInfo +
                '}';
    }
}
