package backtracking;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * 八皇后问题
 *
 * 在8x8格的国际象棋上摆放八个皇后，使其不能相互攻击，即任意两个皇后都不能处于同一行、同一列
 * 或同一斜线上，问一共有多少种摆法？
 *
 * 递归回溯的方法
 *
 * 当我们选择了第一个皇后的位置之后，与其处于同行同列同斜线的位置便都无法被选择，第二个皇后
 * 只能放在未被第一个皇后所辐射的位置上，接着放置第三个皇后，同样不能放在被前两个皇后
 * 辐射到的位置上。
 *
 * 若此时已经没有未被辐射的位置能够选择，也就意味着这种摆法是不可行的，需要回退到上一步，给
 * 第二个皇后重新选择一个未被第一个皇后辐射的位置，再来看是否有第三个皇后可以摆放的位置。
 */
public class EightQueues {
    static char[][] board;      //  表示棋盘
    // 存放结果
    // List<String>表示一种棋盘解法
    static List<List<String>> res = new ArrayList<>();

    // Only need to check these cells: north or northwest or northeast of board[i][j].
    public static boolean isValid(char[][] board, int i, int j) {
        // north
        for (int k = 1; k <= i - 1; k++) {
            if (board[i - k][j] == 'Q') {
                return false;
            }
        }
        // northeast
        for (int k = 1; k <= i - 1 && k <= j - 1; k++) {
            if (board[i - k][j - k] == 'Q') {
                return false;
            }
        }
        // northwest
        for (int k = 1; k <= i - 1 && k <= board.length - 1 - j; k++) {
            if (board[i - k][j + k] == 'Q') {
                return false;
            }
        }
        return true;
    }

    public static List<String> construct(char[][] board) {
        List<String> list = new ArrayList<>();
        for (int i = 1; i < board.length; i++) {
            StringBuilder sb = new StringBuilder();
            for (int j = 1; j < board.length; j++) {
                sb.append(board[i][j]);
            }
            list.add(sb.toString());
        }
        return list;
    }

    // Find next valid position for Q by rows.
    public static void backtrack(int i) {
        if (i == board.length) {
            res.add(construct(board));
            return;
        }

        // In each row, find valid positions.
        for (int j = 1; j < board.length; j++) {
            if (isValid(board, i, j)) {
                board[i][j] = 'Q';
                backtrack(i + 1); // by rows
                board[i][j] = '.';
            }
        }
    }

    public static List<List<String>> solveNQueens(int n) {
        board = new char[n + 1][n + 1];
        for (int i = 0; i < board.length; i++) {
            for (int j = 0; j < board.length; j++) {
                board[i][j] = '.';
            }
        }
        backtrack(1);
        return res;
    }

    public static void main(String[] args) {
        int n = 8;
        solveNQueens(n);
        System.out.println("When n = " + n + ", there are " + res.size() + " solutions:\n");
        for (List<String> list : res) {
            for (String row : list) {
                for (int i = 0; i < row.length(); i++) {
                    System.out.print(row.charAt(i) + " ");
                }
                System.out.println();
            }
            System.out.println();
        }
    }
}
