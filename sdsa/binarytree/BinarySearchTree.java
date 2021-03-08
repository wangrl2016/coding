package binarytree;

/**
 * 参考文章
 * <p>
 * https://www.softwaretestinghelp.com/binary-search-tree-in-java/
 */

public class BinarySearchTree {
    class Node {
        int key;
        Node left, right;

        public Node(int item) {
            key = item;
            left = right = null;
        }
    }

    // 根节点
    Node root;
}
