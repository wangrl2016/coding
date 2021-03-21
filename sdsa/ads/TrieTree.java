package ads;

/**
 * 参考链接
 * <p>
 * https://zh.wikipedia.org/wiki/Trie
 * <p>
 * Trie这个术语来自于retrieval, 又称前缀树或者字典树，是一种有序树。
 */

public class TrieTree {

    public class TrieNode {
        TrieNode[] child;
        boolean end;

        public TrieNode() {
            child = new TrieNode[26];
            end = false;
        }
    }

    private final TrieNode root;

    public TrieTree() {
        root = new TrieNode();
    }

    public void insert(String word) {
        TrieNode currentNode = root;
        for (int i = 0; i < word.length(); i++) {
            TrieNode node = currentNode.child[word.charAt(i) - 'a'];
            if (node == null) {
                node = new TrieNode();
                currentNode.child[word.charAt(i) - 'a'] = node;
            }
            currentNode = node;
        }
        currentNode.end = true;
    }

    public boolean search(String word) {
        TrieNode currentNode = root;
        for (int i = 0; i < word.length(); i++) {
            char ch = word.charAt(i);
            TrieNode node = currentNode.child[ch - 'a'];
            if (node == null)
                return false;
            currentNode = node;
        }
        return currentNode.end;
    }

    public boolean delete(String word) {
        TrieNode currentNode = root;
        for (int i = 0; i < word.length(); i++) {
            char ch = word.charAt(i);
            TrieNode node = currentNode.child[ch - 'a'];
            if (node == null)
                return false;
            currentNode = node;
        }
        if (currentNode.end == true) {
            currentNode.end = false;
            return true;
        }
        return false;
    }

    // Regex to check if word contains only a-z character.
    public static boolean isValid(String word) {
        return word.matches("^[a-z]+$");
    }

    public static void main(String[] args) {
        TrieTree tree = new TrieTree();

        tree.insert("hello");
        tree.insert("world");

        System.out.println("world in Trie? " +
                (tree.search("world") ? "yes" : "no"));
    }
}
