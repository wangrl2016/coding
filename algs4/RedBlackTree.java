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
 * 平衡一棵树的初衷是为了消除最坏情况。
 *
 * 红黑二叉查找树
 *
 * 红黑二叉查找树背后的基本思想是用标准的二叉查找树(完全由2-节点构成)和一些额外信息(替换3-节点)来
 * 表示2-3树。
 *
 * 我们将树中的链接分为两种类型：红链接将两个2-节点连接起来构成一个3-节点，黑链接则是2-3树中的普通
 * 链接。确切地说，我们将3-节点表示为由一条左斜的红色链接(两个2-节点其中之一是另一个的左子节点)相连
 * 的两个2-节点。
 *
 * 红黑树中的颜色表示
 *
 * For convenience, since each node is pointed to by precisely one link (from its
 * parent), we encode the color of links in nodes, by adding a boolean instance
 * variable color to our Node data type, which is true if the link from the parent
 * is red and false if it is black. By convention, null links are black.
 */
public class RedBlackTree {
}
