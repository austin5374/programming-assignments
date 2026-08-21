/*
 * Author: Austin Vodrazka
 * Course: COP3502C Fall 2025
 * Assignment: Program Assigment 4
 * Date: 10/23/25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 12
#define NUMGAMES 7
#define TOTAL 6
#define BASECASESIZE 30

const char GAMES[NUMGAMES][MAXSIZE + 1] = {
    "Wordle", "Spelling Bee", "Crossword",
    "Connections", "Strands", "Letter Boxed", "Total"
};

typedef struct Player {
    char *name;
    int scores[NUMGAMES];
} Player;

//required prototypes
int compare(Player *p1, Player *p2, int key);
void quickSort(Player **list, int n, int key);
void quickSortRec(Player **list, int low, int high, int key);

//helper prototypes
void insertionSortSegment(Player **list, int low, int high, int key);
Player *createPlayer(const char *name, int w, int sb, int cr, int co, int st, int lb);
void freePlayers(Player **list, int n);
void swapPtr(Player **a, Player **b);

/* compare: higher score comes first; break ties by name A toZ */
int compare(Player *p1, Player *p2, int key) {
    int s1 = p1->scores[key];
    int s2 = p2->scores[key];

    if (s1 > s2) return -1;
    if (s1 < s2) return  1;

    int c = strcmp(p1->name, p2->name);
    if (c < 0) return -1;
    if (c > 0) return  1;
    return 0;
}

//sort for small subarrays
void insertionSortSegment(Player **list, int low, int high, int key) {
    int i, j;
    for (i = low + 1; i <= high; i++) {
        Player *cur = list[i];
        j = i - 1;
        while (j >= low && compare(list[j], cur, key) > 0) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = cur;
    }
}

void swapPtr(Player **a, Player **b) {
    Player *t = *a;
    *a = *b;
    *b = t;
}

void quickSort(Player **list, int n, int key) {
    if (list == NULL || n <= 1) return;
    quickSortRec(list, 0, n - 1, key);
}

//quick sort with median-of-three pivot
void quickSortRec(Player **list, int low, int high, int key) {
    int size = high - low + 1;
    if (size <= BASECASESIZE) {
        insertionSortSegment(list, low, high, key);
        return;
    }

    int mid = low + (high - low) / 2;


    if (compare(list[mid], list[low], key) < 0) swapPtr(&list[low], &list[mid]);
    if (compare(list[high], list[low], key) < 0) swapPtr(&list[low], &list[high]);
    if (compare(list[high], list[mid], key) < 0) swapPtr(&list[mid], &list[high]);

    Player *pivot = list[mid];

    //Hoare
    int i = low - 1;
    int j = high + 1;
    for (;;) {
        do { i++; } while (compare(list[i], pivot, key) < 0);
        do { j--; } while (compare(list[j], pivot, key) > 0);
        if (i >= j) break;
        swapPtr(&list[i], &list[j]);
    }

    quickSortRec(list, low, j, key);
    quickSortRec(list, j + 1, high, key);
}

//create a Player on the heap */
Player *createPlayer(const char *name, int w, int sb, int cr, int co, int st, int lb) {
    Player *p = (Player *)malloc(sizeof(Player));
    if (p == NULL) return NULL;

    p->name = (char *)malloc(strlen(name) + 1);
    if (p->name == NULL) {
        free(p);
        return NULL;
    }
    strcpy(p->name, name);

    p->scores[0] = w;
    p->scores[1] = sb;
    p->scores[2] = cr;
    p->scores[3] = co;
    p->scores[4] = st;
    p->scores[5] = lb;
    p->scores[TOTAL] = w + sb + cr + co + st + lb;

    return p;
}

//free list and players
void freePlayers(Player **list, int n) {
    if (list == NULL) return;
    for (int i = 0; i < n; i++) {
        if (list[i] != NULL) {
            free(list[i]->name);
            free(list[i]);
        }
    }
    free(list);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        return 0;
    }

    Player **list = (Player **)malloc(n * sizeof(Player *));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed for list.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        char nameBuf[MAXSIZE + 1];
        int w, sb, cr, co, st, lb;

        if (scanf("%12s %d %d %d %d %d %d",
                  nameBuf, &w, &sb, &cr, &co, &st, &lb) != 7) {
            freePlayers(list, i);
            return 0;
        }

        list[i] = createPlayer(nameBuf, w, sb, cr, co, st, lb);
        if (list[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for player.\n");
            freePlayers(list, i);
            return 1;
        }
    }

    int key;
    if (scanf("%d", &key) != 1) {
        freePlayers(list, n);
        return 0;
    }

    quickSort(list, n, key);

    printf("%s Ranklist\n", GAMES[key]);
    for (int i = 0; i < n; i++) {
        printf("%d. %-15s %d\n", i + 1, list[i]->name, list[i]->scores[key]);
    }

    freePlayers(list, n);
    return 0;
}
