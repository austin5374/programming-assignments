/*
Summer 26
COP 3503C Assignment 3
This program is written by: Austin Vodrazka */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;

public class Main {

    // disjoint set with union by rank + path compression
    // also tracks size of each set at the root so we can do the 2ab thing
    static int[] parent;
    static int[] rank;
    static int[] size;

    static void makeSets(int n) {
        parent = new int[n + 1]; // 1 indexed, ignore slot 0
        rank = new int[n + 1];
        size = new int[n + 1];
        for (int i = 1; i <= n; i++) {
            parent[i] = i; // everyone is their own parent at the start
            rank[i] = 0;
            size[i] = 1; // each set is just one node for now
        }
    }

    // iterative find so we dont blow the stack on big inputs (n up to 1e5)
    static int find(int x) {
        int root = x;
        while (parent[root] != root) {
            root = parent[root];
        }
        // second pass to flatten the path (path compression)
        while (parent[x] != root) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    }

    // returns the change in connectivity (2*a*b) if we actually merged
    // returns 0 if they were alredy in the same set
    static long union(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) {
            return 0; // already connected, nothing changes
        }

        // make sure ra is the taller tree (union by rank)
        if (rank[ra] < rank[rb]) {
            int tmp = ra;
            ra = rb;
            rb = tmp;
        }

        long sa = size[ra];
        long sb = size[rb];

        parent[rb] = ra; // hang the smaller under the bigger
        size[ra] += size[rb];
        if (rank[ra] == rank[rb]) {
            rank[ra]++; // only grows when both were equal height
        }

        // new component is size (a+b), old were a and b
        // (a+b)^2 - a^2 - b^2 = 2ab, thats how much connectivity goes up
        return 2 * sa * sb;
    }

    public static void main(String[] args) throws Exception {
        // StreamTokenizer is way faster than Scanner for this much input
        StreamTokenizer in = new StreamTokenizer(new BufferedReader(new InputStreamReader(System.in)));

        in.nextToken(); int n = (int) in.nval;
        in.nextToken(); int m = (int) in.nval;
        in.nextToken(); int d = (int) in.nval;

        // store all edges. eu[i]/ev[i] is connection number i (1 indexed)
        int[] eu = new int[m + 1];
        int[] ev = new int[m + 1];
        for (int i = 1; i <= m; i++) {
            in.nextToken(); eu[i] = (int) in.nval;
            in.nextToken(); ev[i] = (int) in.nval;
        }

        // the order we destroy connections in
        int[] destroyOrder = new int[d];
        boolean[] destroyed = new boolean[m + 1];
        for (int i = 0; i < d; i++) {
            in.nextToken();
            int idx = (int) in.nval;
            destroyOrder[i] = idx;
            destroyed[idx] = true; // mark so we skip it on the first pass
        }

        makeSets(n);

        // connectivity = sum of size^2. starts as n (every node is its own size 1 set)
        long conn = n;

        // first add every edge that never gets destroyed
        // this gives us the final state (after all d destroys happened)
        for (int i = 1; i <= m; i++) {
            if (!destroyed[i]) {
                conn += union(eu[i], ev[i]);
            }
        }

        long[] results = new long[d + 1];
        results[d] = conn; // last line of output = fully destroyed state

        // now add the destroyed edges back in REVERSE order
        // cant remove edges from a dsu, so we build up instead of tearing down
        for (int step = d - 1; step >= 0; step--) {
            int idx = destroyOrder[step];
            conn += union(eu[idx], ev[idx]);
            results[step] = conn;
        }

        // print forward. StringBuilder so we dont flush a million times
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i <= d; i++) {
            sb.append(results[i]).append('\n');
        }
        System.out.print(sb);
    }
}
