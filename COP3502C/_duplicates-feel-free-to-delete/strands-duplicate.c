/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
Assignment: PA3
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//golabal constants
//direction codes
//input uses -1 for NULL
static const int DR[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
static const int DC[] = { -1,  0,  1, -1, 1, -1, 0, 1 };
#define NULLDIR (-1)


// type of nodee
typedef struct dllnode {
    char ch;
    struct dllnode *prev;
    struct dllnode *next;
} dllnode;


// prototypes
dllnode*** allocGrid(int r, int c);
void       freeGridAndNodes(dllnode*** grid, int r, int c);

void        linkNextFromDirs(dllnode*** grid, int r, int c, const int *dirs);
void        linkPrevFromDirs(dllnode*** grid, int r, int c, const int *dirs);

static dllnode* findHead(dllnode *cur);
static dllnode* findTail(dllnode *cur);

void doPrint(dllnode*** grid, int y, int x);
void doReverse(dllnode*** grid, int y, int x);
void doConcat(dllnode*** grid, int y1, int x1, int y2, int x2);




//implementations

dllnode*** allocGrid(int r, int c) {
    dllnode*** g = malloc(sizeof *g * r);
    if (!g) { fprintf(stderr, "alloc failed\n"); exit(1); }
    for (int i = 0; i < r; i++) {
        g[i] = malloc(sizeof *g[i] * c);
        if (!g[i]) { fprintf(stderr, "alloc failed\n"); exit(1); }
        for (int j = 0; j < c; j++) g[i][j] = NULL;
    }
    return g;
}

void freeGridAndNodes(dllnode*** grid, int r, int c) {
    if (!grid) return;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) free(grid[i][j]);
        free(grid[i]);
    }
    free(grid);
}

void linkNextFromDirs(dllnode*** grid, int r, int c, const int *dirs) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int d = dirs[i*c + j];
            if (d != NULLDIR) {
                int ni = i + DR[d], nj = j + DC[d];
                grid[i][j]->next = grid[ni][nj];
            }
        }
    }
}

void linkPrevFromDirs(dllnode*** grid, int r, int c, const int *dirs) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            int d = dirs[i*c + j];
            if (d != NULLDIR) {
                int pi = i + DR[d], pj = j + DC[d];
                grid[i][j]->prev = grid[pi][pj];
            }
        }
    }
}










//utilities//
static dllnode* findHead(dllnode *cur) {
    while (cur && cur->prev) cur = cur->prev;
    return cur;
}
static dllnode* findTail(dllnode *cur) {
    while (cur && cur->next) cur = cur->next;
    return cur;
}

//t = 1 prints the word
void doPrint(dllnode*** grid, int y, int x) {
    dllnode *p = findHead(grid[y][x]);
    for (; p; p = p->next) putchar(p->ch);
    putchar('\n');
}

//t = 2 reverses the word in-place
void doReverse(dllnode*** grid, int y, int x) {
    dllnode *head = findHead(grid[y][x]);
    for (dllnode *p = head; p; p = p->prev) {
        dllnode *tmp = p->next;
        p->next = p->prev;
        p->prev = tmp;

    }
}

//t = 3: concat word1 (ending at y1,x1) + word2 (starting at y2,x2)
void doConcat(dllnode*** grid, int y1, int x1, int y2, int x2) {
    dllnode *tail1 = findTail(grid[y1][x1]);
    dllnode *head2 = findHead(grid[y2][x2]);
    tail1->next = head2;
    head2->prev = tail1;
}








///MAIN///

int main(void) {
    int r = 0, c = 0;
    if (scanf("%d %d", &r, &c) != 2 || r < 2 || c < 2) {
        fprintf(stderr, "input error reading r,c\n");
        return 1;
    }

    dllnode*** grid = allocGrid(r, c);

//read letters and make nodes
    char *row = malloc((size_t)c + 1);
    if (!row) { fprintf(stderr, "alloc failed\n"); exit(1); }

    for (int i = 0; i < r; i++) {
        if (scanf("%s", row) != 1 || (int)strlen(row) != c) {
            fprintf(stderr, "input error reading row of letters\n");
            exit(1);
        }
        for (int j = 0; j < c; j++) {
            dllnode *n = malloc(sizeof *n);
            if (!n) { fprintf(stderr, "alloc failed\n"); exit(1); }
            n->ch = row[j];
            n->prev = n->next = NULL;
            grid[i][j] = n;
        }
    }
    free(row);

  
  

    int total = r * c;
    int *nextDir = malloc(sizeof *nextDir * total);
    int *prevDir = malloc(sizeof *prevDir * total);
    if (!nextDir || !prevDir) { fprintf(stderr, "alloc failed\n"); exit(1); }

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (scanf("%d", &nextDir[i*c + j]) != 1)
                { fprintf(stderr, "input error\n"); exit(1); }

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (scanf("%d", &prevDir[i*c + j]) != 1)
                { fprintf(stderr, "input error\n"); exit(1); }

//pointers
    linkNextFromDirs(grid, r, c, nextDir);
    linkPrevFromDirs(grid, r, c, prevDir);
    free(nextDir);
    free(prevDir);

//queries
    int q = 0;
    if (scanf("%d", &q) != 1) { fprintf(stderr, "input error reading q\n"); exit(1); }
    for (int t = 0; t < q; t++) {
        int type = 0;
        if (scanf("%d", &type) != 1) { fprintf(stderr, "input error\n"); exit(1); }
        if (type == 1 || type == 2) {
            int y, x;
            if (scanf("%d %d", &y, &x) != 2) { fprintf(stderr, "input error\n"); exit(1); }
            if (type == 1) doPrint(grid, y, x);
            else           doReverse(grid, y, x);
        } else if (type == 3) {
            int y1,x1,y2,x2;
            if (scanf("%d %d %d %d", &y1,&x1,&y2,&x2) != 4)
                { fprintf(stderr, "input error\n"); exit(1); }
            doConcat(grid, y1, x1, y2, x2);
        }
    }

//clean
    freeGridAndNodes(grid, r, c);
    return 0;
}
