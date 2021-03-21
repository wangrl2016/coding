package graph;

import java.util.ArrayList;

/**
 * 参考链接
 * <p>
 * https://www.geeksforgeeks.org/graph-and-its-representations/
 * <p>
 * 定义
 * <p>
 * A graph is a data structure that consists of the following two components:
 * 1. A finite set of vertices also called as nodes.
 * 2. A finite set of ordered pair of the form (u, v) called as edge. The pair
 * is ordered because (u, v) is not the same as (v, u) in case of a directed
 * graph(di-graph).
 * <p>
 * 使用相邻矩阵表示图
 * <p>
 * Adjacency matrix is a 2D array of size V x V where V is the number of vertices
 * in a graph. Let the 2D array be adj[][], a slot adj[i][j] = 1 indicates that
 * there is an edge from vertex i to vertex j.
 * <p>
 * 使用相邻链表表示矩阵
 * <p>
 * The size of the array is equal to the number of vertices. Let the array be an
 * array[]. An entry array[i] represents the list of vertices adjacent to the ith
 * vertex.
 */

public class GraphRepresent {

    // A utility function to add ane edge in an undirected graph.
    static void addEdge(ArrayList<ArrayList<Integer>> adj, int u, int v) {
        adj.get(u).add(v);
        adj.get(v).add(u);
    }

    static void printGraph(ArrayList<ArrayList<Integer>> adj) {
        for (int i = 0; i < adj.size(); i++) {
            System.out.println("\nAdjacency list of vertex " + i);
            System.out.print("head");
            for (int j = 0; j < adj.get(i).size(); j++) {
                System.out.print(" -> " + adj.get(i).get(j));
            }
            System.out.println();
        }
    }

    public static void main(String[] args) {
        // Create a graph with 5 vertices.
        int v = 5;
        ArrayList<ArrayList<Integer>> adj
                = new ArrayList<>(v);

        for (int i = 0; i < v; i++)
            adj.add(new ArrayList<>());

        // Add edges one by one
        addEdge(adj, 0, 1);
        addEdge(adj, 0, 4);
        addEdge(adj, 1, 2);
        addEdge(adj, 1, 3);
        addEdge(adj, 1, 4);
        addEdge(adj, 2, 3);
        addEdge(adj, 3, 4);

        printGraph(adj);
    }
}
