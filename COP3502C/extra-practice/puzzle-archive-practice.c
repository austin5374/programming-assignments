/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 64

/* ----- structs ----- */
typedef struct {
    char *playerName;
    int  totalScore;
} Player;

typedef struct {
    char    *puzzleType;   /* points into puzzleTypes; do NOT malloc here */
    int      puzzleNo;
    Player **players;      /* size = playerCount */
    int     *scores;       /* size = playerCount */
    int      playerCount;
} Puzzle;

typedef struct {
    Puzzle *puzzles;       /* size = puzzleCount */
    int     puzzleCount;
} Archive;

/* ----- small helper ----- */
static char *readWord(void) {
    char buf[NAME_MAX];
    if (scanf("%63s", buf) != 1) {
        fprintf(stderr, "input error\n");
        exit(1);
    }
    char *s = malloc(strlen(buf) + 1);
    if (!s) { fprintf(stderr, "alloc failed\n"); exit(1); }
    strcpy(s, buf);
    return s;
}

/* ----- prototypes (simple) ----- */
char**  readPuzzleTypes(int *countOut);
Player* createPlayer(char *name);
Player** readPlayerPool(int *playerCount);
Archive* createArchive(int puzzleCount);

char*   getPuzzleTypePtr(char **puzzleTypes, int count, char *type);
Player* getPlayerPtrByName(Player **playerPool, int playerCount, char *playerName);

void    printBestScorer(Puzzle *puzzle);
void    printMaxTotalScorer(Archive *archive);

void    freePlayerPool(Player **pool, int count);
void    freeArchive(Archive *archive);

/* ----- implementations ----- */

char **readPuzzleTypes(int *countOut) {
    int n = 0;
    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "input error reading puzzle type count\n");
        exit(1);
    }

    char **types = malloc(n * sizeof *types);
    if (!types) { fprintf(stderr, "alloc failed\n"); exit(1); }

    for (int i = 0; i < n; i++) {
        types[i] = readWord();
    }
    *countOut = n;
    return types;
}

Player* createPlayer(char *name) {
    Player *p = malloc(sizeof *p);
    if (!p) { fprintf(stderr, "alloc failed\n"); exit(1); }

    p->playerName = malloc(strlen(name) + 1);
    if (!p->playerName) { fprintf(stderr, "alloc failed\n"); exit(1); }

    strcpy(p->playerName, name);
    p->totalScore = 0;
    return p;
}

Player** readPlayerPool(int *playerCount) {
    int n = 0;
    if (scanf("%d", &n) != 1 || n < 0) {
        fprintf(stderr, "input error reading player count\n");
        exit(1);
    }

    Player **pool = malloc(n * sizeof *pool);
    if (!pool) { fprintf(stderr, "alloc failed\n"); exit(1); }

    for (int i = 0; i < n; i++) {
        char *nameTmp = readWord();
        pool[i] = createPlayer(nameTmp);
        free(nameTmp);
    }

    *playerCount = n;
    return pool;
}

Archive* createArchive(int puzzleCount) {
    Archive *a = malloc(sizeof *a);
    if (!a) { fprintf(stderr, "alloc failed\n"); exit(1); }
    a->puzzleCount = puzzleCount;
    a->puzzles = calloc(puzzleCount, sizeof *a->puzzles); /* zeroed */
    if (!a->puzzles) { fprintf(stderr, "alloc failed\n"); exit(1); }
    return a;
}

char* getPuzzleTypePtr(char **puzzleTypes, int count, char *type) {
    for (int i = 0; i < count; i++) {
        if (strcmp(puzzleTypes[i], type) == 0) return puzzleTypes[i];
    }
    /* type should always exist according to the input rules */
    fprintf(stderr, "unknown puzzle type: %s\n", type);
    exit(1);
}

Player* getPlayerPtrByName(Player **playerPool, int playerCount, char *playerName) {
    for (int i = 0; i < playerCount; i++) {
        if (strcmp(playerPool[i]->playerName, playerName) == 0) return playerPool[i];
    }
    /* name should always exist according to the input rules */
    fprintf(stderr, "unknown player: %s\n", playerName);
    exit(1);
}

void printBestScorer(Puzzle *p) {
    if (p->playerCount == 0) {
        printf("%s#%d No player yet for this puzzle\n", p->puzzleType, p->puzzleNo);
        return;
    }
    /* pick highest score; tie = earlier index wins */
    int bestIdx = 0;
    int best = p->scores[0];
    for (int j = 1; j < p->playerCount; j++) {
        int sc = p->scores[j];
        if (sc > best) {
            best = sc;
            bestIdx = j;
        }
    }
    printf("%s#%d %s %d\n",
           p->puzzleType, p->puzzleNo,
           p->players[bestIdx]->playerName, best);
}

void printMaxTotalScorer(Archive *archive) {
    /* build first-seen list across the whole archive */
    int cap = 16;
    int seenCount = 0;
    Player **seen = malloc(cap * sizeof *seen);
    if (!seen) { fprintf(stderr, "alloc failed\n"); exit(1); }

    for (int i = 0; i < archive->puzzleCount; i++) {
        Puzzle *p = &archive->puzzles[i];
        for (int j = 0; j < p->playerCount; j++) {
            Player *pp = p->players[j];
            int found = 0;
            for (int k = 0; k < seenCount; k++) {
                if (seen[k] == pp) { found = 1; break; }
            }
            if (!found) {
                if (seenCount == cap) {
                    cap *= 2;
                    Player **tmp = realloc(seen, cap * sizeof *tmp);
                    if (!tmp) { fprintf(stderr, "alloc failed\n"); exit(1); }
                    seen = tmp;
                }
                seen[seenCount++] = pp;
            }
        }
    }

    if (seenCount == 0) {
        printf("No players in archive\n");
        free(seen);
        return;
    }

    /* pick max total; tie = earlier first-seen wins */
    int bestIdx = 0;
    int bestTotal = seen[0]->totalScore;
    for (int i = 1; i < seenCount; i++) {
        int t = seen[i]->totalScore;
        if (t > bestTotal) {
            bestTotal = t;
            bestIdx = i;
        }
    }

    printf("Top player: %s with total score %d\n",
           seen[bestIdx]->playerName, bestTotal);
    free(seen);
}

void freePlayerPool(Player **pool, int count) {
    for (int i = 0; i < count; i++) {
        free(pool[i]->playerName);
        free(pool[i]);
    }
    free(pool);
}

void freeArchive(Archive *archive) {
    for (int i = 0; i < archive->puzzleCount; i++) {
        free(archive->puzzles[i].players);
        free(archive->puzzles[i].scores);
    }
    free(archive->puzzles);
    free(archive);
}

/* ----- main ----- */
int main(void) {
    int puzzleTypeCount = 0;
    char **puzzleTypes = readPuzzleTypes(&puzzleTypeCount);

    int playerCount = 0;
    Player **playerPool = readPlayerPool(&playerCount);

    int z = 0;
    if (scanf("%d", &z) != 1 || z < 0) {
        fprintf(stderr, "input error reading puzzle count\n");
        exit(1);
    }
    Archive *archive = createArchive(z);

    /* read puzzles */
    for (int i = 0; i < z; i++) {
        char *typeTmp = readWord();
        int id = 0, c = 0;
        if (scanf("%d %d", &id, &c) != 2 || c < 0) {
            fprintf(stderr, "input error reading puzzle header\n");
            exit(1);
        }

        Puzzle *pz = &archive->puzzles[i];
        pz->puzzleType = getPuzzleTypePtr(puzzleTypes, puzzleTypeCount, typeTmp);
        free(typeTmp);
        pz->puzzleNo = id;
        pz->playerCount = c;

        if (c > 0) {
            pz->players = malloc(c * sizeof *pz->players);
            pz->scores  = malloc(c * sizeof *pz->scores);
            if (!pz->players || !pz->scores) { fprintf(stderr, "alloc failed\n"); exit(1); }
        }

        for (int j = 0; j < c; j++) {
            char *nameTmp = readWord();
            int sc = 0;
            if (scanf("%d", &sc) != 1) {
                fprintf(stderr, "input error reading score\n");
                exit(1);
            }
            Player *pp = getPlayerPtrByName(playerPool, playerCount, nameTmp);
            free(nameTmp);

            pz->players[j] = pp;
            pz->scores[j]  = sc;
            pp->totalScore += sc;  /* add to total while loading */
        }
    }

    /* queries */
    int q = 0;
    if (scanf("%d", &q) != 1) { fprintf(stderr, "input error reading q\n"); exit(1); }
    for (int t = 0; t < q; t++) {
        int qt = 0;
        if (scanf("%d", &qt) != 1) { fprintf(stderr, "input error reading query type\n"); exit(1); }
        if (qt == 1) {
            printMaxTotalScorer(archive);
        } else if (qt == 2) {
            printf("Top scorer per puzzle:\n");
            for (int i = 0; i < archive->puzzleCount; i++) {
                printBestScorer(&archive->puzzles[i]);
            }
        }
    }

    /* free at the end (puzzleType strings last, since puzzles point to them) */
    freeArchive(archive);
    freePlayerPool(playerPool, playerCount);
    for (int i = 0; i < puzzleTypeCount; i++) free(puzzleTypes[i]);
    free(puzzleTypes);
    return 0;
}
