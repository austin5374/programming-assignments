/*
Summer 26
COP 3503C Assignment 4
This program is written by: Austin Vodrazka */

import java.util.*;
import java.io.*;

public class Main {

    public static void main(String[] args) throws IOException {
        // scanner is too slow for a 1000x1000 grid, using bufferedreader
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        StringTokenizer st = new StringTokenizer(br.readLine());
        int r = Integer.parseInt(st.nextToken());
        int c = Integer.parseInt(st.nextToken());

        char[][] grid = new char[r][];
        for (int i = 0; i < r; i++) {
            String line = br.readLine();
            // the test files have windows line endings so chop the \r or it messes up the last col
            if (line.length() > 0 && line.charAt(line.length()-1) == '\r') {
                line = line.substring(0, line.length()-1);
            }
            grid[i] = line.toCharArray();
        }

        // same idea as elevator trouble, a cell is just one int (row*c + col)
        // so the queue and dist array only deal with ints not some coord object
        int start = -1;
        int end = -1;

        // 26 buckets, one per capital letter. each holds the encoded ids of that letter
        List<Integer>[] letters = new List[26];
        for (int i = 0; i < 26; i++) letters[i] = new ArrayList<Integer>();

        for (int i = 0; i < r; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                char ch = grid[i][j];
                if (ch == '*') {
                    start = i*c + j;
                } else if (ch == '$') {
                    end = i*c + j;
                } else if (ch >= 'A' && ch <= 'Z') {
                    letters[ch-'A'].add(i*c + j);
                }
            }
        }

        // dist is also my visited check, -1 means i havnt been there yet
        int[] dist = new int[r*c];
        Arrays.fill(dist, -1);

        // keeps track of letter groups i alredy blew up so i dont do it again.
        // this is the whole trick, if a letter has like 300k cells and you expand it
        // every single time you pop one its O(k^2) and dies on the time limit
        boolean[] letterDone = new boolean[26];

        Queue<Integer> q = new ArrayDeque<Integer>();
        dist[start] = 0;
        q.add(start);

        while (q.size() > 0) {
            int cur = q.poll();

            if (cur == end) break; // got out

            int row = cur / c;
            int col = cur % c;
            char ch = grid[row][col];

            // teleport part. if im standing on a letter and that groups not done yet,
            // throw all the same letter cells in the queue one time then mark it done
            if (ch >= 'A' && ch <= 'Z' && !letterDone[ch-'A']) {
                letterDone[ch-'A'] = true;
                for (int id : letters[ch-'A']) {
                    if (dist[id] == -1) {
                        dist[id] = dist[cur] + 1;
                        q.add(id);
                    }
                }
            }

            // in elevator trouble there was only up/down so 2 ifs. here its 4 directions
            // so just 4 of the same block, didnt bother with a loop

            // up
            if (row-1 >= 0 && grid[row-1][col] != '!') {
                int nxt = (row-1)*c + col;
                if (dist[nxt] == -1) {
                    dist[nxt] = dist[cur] + 1;
                    q.add(nxt);
                }
            }

            // down
            if (row+1 < r && grid[row+1][col] != '!') {
                int nxt = (row+1)*c + col;
                if (dist[nxt] == -1) {
                    dist[nxt] = dist[cur] + 1;
                    q.add(nxt);
                }
            }

            // left
            if (col-1 >= 0 && grid[row][col-1] != '!') {
                int nxt = row*c + (col-1);
                if (dist[nxt] == -1) {
                    dist[nxt] = dist[cur]+1;
                    q.add(nxt);
                }
            }

            // right
            if (col+1 < c && grid[row][col+1] != '!') {
                int nxt = row*c + (col+1);
                if (dist[nxt] == -1) {
                    dist[nxt] = dist[cur]+1;
                    q.add(nxt);
                }
            }
        }

        // if end is still -1 he never made it out.
        // note the test files want -1 printed here not "Call 911" even tho the pdf says that
        System.out.println(dist[end]);
    }
}
