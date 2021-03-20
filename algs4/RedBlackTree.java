/**
 * 平衡查找树
 *
 * 2-3查找树
 *
 * 一棵2-3查找树或为一棵空树，或由以下组成
 *
 * 2-节点，含有一个键(及其对应的值)和两条链接，左链接指向的2-3树中的键都小于
 * 该节点，右链接指向的2-3树中的键都大于该节点。
 *
 * 3-节点，含有两个键(及其对应的值)和三条链接，左链接指向的2-3树中的键都小于
 * 该节点，中链接指向的2-3树中的键都位于该节点的两个键之间，右链接指向的2-3树
 * 中的键都大于该节点。
 *
 * 查找
 *
 * To determine whether a key in the tree, we compare it against the
 * keys at the root. If it is equal to any of them, we have a search hit;
 * otherwise, we follow the link from the root to the subtree corresponding
 * to the interval of key values that could contain the search key. If
 * that link is null, we have a search miss; otherwise we recursively
 * search in that subtree.
 *
 * 节点的插入分为以下几种情况
 *
 * 1. Insert into a 2-node
 * 2. Insert into a tree consisting of a single 3-node
 * 3. Insert into a 3-node whose parent is a 2-node
 * 4. Insert into a 3-node whose parent is a 3-node
 *
 * ４-节点的几种情况
 *
 * The 4-node may be the root; it may be the left child or the right child of
 * a 2-node; or it may be the left child, middle child, or right child of a
 * 3-node.
 *
 * 
 */
public class RedBlackTree {
}
