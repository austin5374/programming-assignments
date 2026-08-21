/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
*/

// C program to read N fixed-length words,
// count letter frequencies across all words,
// then print each word with its score.

#include <stdio.h>

#define MAX 1000       // max number of words
#define LEN 5          // word length
#define BUF (LEN + 1)  // storage per word
#define ABC 26         // alphabet size

// prototypes (before main)
int  readWords(char words[][BUF]);
void countLetters(char words[][BUF], int n, int freq[]);
int  scoreWord(char word[], int freq[]);

int main(void)
{
    char words[MAX][BUF];
    int n = readWords(words);

    int freq[ABC];
    int i;
    for (i = 0; i < ABC; i++) freq[i] = 0;

    countLetters(words, n, freq);

    for (i = 0; i < n; i++) {
        int s = scoreWord(words[i], freq);
        printf("%s %d\n", words[i], s);
    }

    return 0;
}

// read number of words and then the words (5 letters each)
int readWords(char words[][BUF])
{
    int n, i;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0) n = 0;
    if (n > MAX) n = MAX;

    for (i = 0; i < n; i++) {
        scanf("%5s", words[i]);
        words[i][LEN] = '\0';
    }
    return n;
}

// count a..z across all words
void countLetters(char words[][BUF], int n, int freq[])
{
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < LEN; j++) {
            k = words[i][j] - 'a';
            if (k >= 0 && k < ABC) {
                freq[k] = freq[k] + 1;
            }
        }
    }
}

// add up frequencies for one word
int scoreWord(char word[], int freq[])
{
    int j, k, score = 0;
    for (j = 0; j < LEN; j++) {
        k = word[j] - 'a';
        if (k >= 0 && k < ABC) {
            score = score + freq[k];
        }
    }
    return score;
}
