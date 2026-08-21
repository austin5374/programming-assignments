/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
Assignment: PA2
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_MAX 64

static char *readWord(void) {
    char buf[NAME_MAX];
    if (scanf("%63s", buf) != 1) {
        fprintf(stderr, "input error\n");
        exit(1);
    }
    char *s = malloc(strlen(buf) + 1);
    if (!s) {
        fprintf(stderr, "alloc failed\n");
        exit(1);
    }
    strcpy(s, buf);
    return s;
}

static int cmpStr(const void *a, const void *b) {
    const char *sa = *(const char * const *)a;
    const char *sb = *(const char * const *)b;
    return strcmp(sa, sb);
}

static char **readDictionary(int *countOut) {
    int n = 0;
    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "input error reading dictionary size\n");
        exit(1);
    }

    char **dict = malloc(n * sizeof *dict);
    if (!dict) {
        fprintf(stderr, "alloc failed\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        dict[i] = readWord();
    }
    
    *countOut = n;
    return dict;
}

typedef struct {
    char **data;
    int    size;
    int    cap;
} Vec;

static void vecInit(Vec *v) {
    v->data = NULL;
    v->size = 0;
    v->cap  = 0;
}
static void vecPush(Vec *v, const char *s) {
    if (v->size == v->cap) {
        int newCap = (v->cap == 0) ? 16 : v->cap * 2;
        char **tmp = realloc(v->data, newCap * sizeof *tmp);
        if (!tmp) {
            fprintf(stderr, "alloc failed\n");
            exit(1);
        }
        v->data = tmp;
        v->cap  = newCap;
    }
    v->data[v->size] = malloc(strlen(s) + 1);
    if (!v->data[v->size]) {
        fprintf(stderr, "alloc failed\n");
        exit(1);
    }
    strcpy(v->data[v->size], s);
    v->size++;
}

static void vecFree(Vec *v) {
    for (int i = 0; i < v->size; i++) {
        free(v->data[i]);
    }
    free(v->data);
}

static void sort7(char *s) {
    for (int i = 0; i < 6; i++) {
        int min = i;
        for (int j = i + 1; j < 7; j++) {
            if (s[j] < s[min]) min = j;
        }
        if (min != i) {
            char tmp = s[i];
            s[i] = s[min];
            s[min] = tmp;
        }
    }
}

static int dictHasRec(char **dict, int lo, int hi, const char *key) {
    if (lo > hi) return 0;
    int mid = lo + (hi - lo) / 2;
    int cmp = strcmp(key, dict[mid]);
    if (cmp == 0) return 1;
    if (cmp < 0) return dictHasRec(dict, lo, mid - 1, key);
    return dictHasRec(dict, mid + 1, hi, key);
}

static int dictHas(char **dict, int n, const char *key) {
    return dictHasRec(dict, 0, n - 1, key);
}

static void genAll(char *letters, char required,
                   int used[7],
                   char *buf, int len,
                   char **dict, int dictCount,
                   Vec *out) {
    if (len >= 4) {
        int hasReq = 0;
        for (int i = 0; i < len; i++) {
            if (buf[i] == required) { hasReq = 1; break; }
        }
        if (hasReq) {
            buf[len] = '\0';
            if (dictHas(dict, dictCount, buf)) {
                vecPush(out, buf);
            }
        }
    }

    if (len == 7) return;

    for (int i = 0; i < 7; i++) {
        if (!used[i]) {
            used[i] = 1;
            buf[len] = letters[i];
            genAll(letters, required, used, buf, len + 1, dict, dictCount, out);
            used[i] = 0;
        }
    }
}

int main(void) {
    char *puzzle = readWord();
    if (strlen(puzzle) != 7) {
        fprintf(stderr, "puzzle must be 7 letters\n");
        free(puzzle);
        return 1;
    }

    char required = puzzle[3];
    sort7(puzzle);

    int dictCount = 0;
    char **dict = readDictionary(&dictCount);

    qsort(dict, dictCount, sizeof *dict, cmpStr);

    Vec results;
    vecInit(&results);

    int used[7] = {0};
    char buf[8];

    genAll(puzzle, required, used, buf, 0, dict, dictCount, &results);

    for (int i = 0; i < results.size; i++) {
        printf("%s\n", results.data[i]);
    }

    vecFree(&results);
    for (int i = 0; i < dictCount; i++) free(dict[i]);
    free(dict);
    free(puzzle);
    return 0;
}
