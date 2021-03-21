package matrix;

/**
 * Given a matrix, clockwise rotate elements in it.
 * <p>
 * For 4x4 matrix
 * <p>
 * Input
 * 1    2    3    4
 * 5    6    7    8
 * 9    10   11   12
 * 13   14   15   16
 * <p>
 * Output
 * 5    1    2    3
 * 9    10   6    4
 * 13   11   7    8
 * 14   15   16   12
 * <p>
 * 算法实现
 * <p>
 * 1. Move elements of top row.
 * 2. Move elements of last column.
 * 3. Move elements of bottom row.
 * 4. Move elements of first column.
 */

public class MatrixRotate {

    static int ROW = 4;
    static int COL = 4;

    // A function to rotate a matrix
    // mat[][] of size row x col.
    // Initially, m = row and n = col.
    static void rotateMatrix(int m, int n, int mat[][]) {
        int row = 0, col = 0;
        int prev, curr;

        // row - starting row index
        // m - ending row index
        // col - starting column index
        // n - ending column index
        // i - iterator

        while (row < m && col < n) {
            if (row + 1 == m || col + 1 == n)
                break;

            // Store the first element of next row, this element
            // will replace first element of current row.
            prev = mat[row + 1][col];

            // Move elements of first row from the remaining rows.
            for (int i = col; i < n; i++) {
                curr = mat[row][i];
                mat[row][i] = prev;
                prev = curr;
            }
            row++;

            // Move elements of last column from the remaining columns.
            for (int i = row; i < m; i++) {
                curr = mat[i][n - 1];
                mat[i][n - 1] = prev;
                prev = curr;
            }
            n--;

            // Move elements of last row from the remaining rows.
            if (row < m) {
                for (int i = n - 1; i >= col; i--) {
                    curr = mat[m - 1][i];
                    mat[m - 1][i] = prev;
                    prev = curr;
                }
            }
            m--;

            // Move element of first column from the remaining rows.
            if (col < n) {
                for (int i = m - 1; i >= row; i--) {
                    curr = mat[i][col];
                    mat[i][col] = prev;
                    prev = curr;
                }
            }
            col++;
        }

        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                System.out.print(mat[i][j] + " ");
            }
            System.out.print("\n");
        }
    }

    public static void main(String[] args) {
        int a[][] = {
                {1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12},
                {13, 14, 15, 16}};

        rotateMatrix(ROW, COL, a);
    }
}
