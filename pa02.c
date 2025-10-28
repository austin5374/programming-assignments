/*============================================================================
| Assignment: pa02 - Encrypting a plaintext file using the Hill cipher
|
| Author: Austin
| Language: C
| To Compile: gcc -o pa02 pa02.c
| To Execute: ./pa02 kX.txt pX.txt
|   where kX.txt is the key file
|   and  pX.txt is the plaintext file
| Note:
|   - Program must run on Eustis (Linux)
|   - All input files are simple 8-bit ASCII
|
| Class: CIS3360 - Security in Computing - Fall 2025
| Instructor: McAlpin
| Due Date: October 19th 2025
+===========================================================================*/




#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_N 9
#define MAX_TEXT 10000

//prototypes
void print_usage(const char *prog);
int  read_key_matrix(FILE *kf, int key[MAX_N][MAX_N], int *n_out);
void print_matrix(const int key[MAX_N][MAX_N], int n);
int  read_plaintext(FILE *pf, char buf[MAX_TEXT], int *len_out);
void print_80cols(const char *s, int len);
void pad_plaintext(char buf[MAX_TEXT], int *len_inout, int n);
int  mod26(int x);
void encrypt(const int key[MAX_N][MAX_N], int n, const char *plain, int len, char *cipher);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *keyfile = argv[1];
    const char *plainfile = argv[2];

    FILE *kf = fopen(keyfile, "r");
    if (kf == NULL) {
        fprintf(stderr, "Error: could not open key file '%s'\n", keyfile);
        return EXIT_FAILURE;
    }

    FILE *pf = fopen(plainfile, "r");
    if (pf == NULL) {
        fprintf(stderr, "Error: could not open plaintext file '%s'\n", plainfile);
        fclose(kf);
        return EXIT_FAILURE;
    }

    int n = 0;
    int key[MAX_N][MAX_N];
    if (read_key_matrix(kf, key, &n) != 0) {
        fclose(pf);
        fclose(kf);
        return EXIT_FAILURE;
    }
    fclose(kf);

    //matrix and read and pad plaintext */
    print_matrix(key, n);
    char plain[MAX_TEXT];
    int plen = 0;
    if (read_plaintext(pf, plain, &plen) != 0) {
        fclose(pf);
        return EXIT_FAILURE;
    }
    fclose(pf);

    pad_plaintext(plain, &plen, n);

    //Plaintext&ciphertext section
    printf("\nPlaintext:\n");
    print_80cols(plain, plen);
    char cipher[MAX_TEXT];
    encrypt(key, n, plain, plen, cipher);

    printf("\nCiphertext:\n");
    print_80cols(cipher, plen);

    return EXIT_SUCCESS;
}

void print_usage(const char *prog) {
    fprintf(stderr, "Usage: %s <keyfile> <plaintextfile>\n", prog);
}

//reads n then n*n ints//
int read_key_matrix(FILE *kf, int key[MAX_N][MAX_N], int *n_out) {
    int n;
    if (fscanf(kf, "%d", &n) != 1) {
        fprintf(stderr, "Error: key file missing matrix size on first line\n");
        return -1;
    }
    if (n < 2 || n > 9) {
        fprintf(stderr, "Error: matrix size %d is out of range (must be 2..9)\n", n);
        return -1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(kf, "%d", &key[i][j]) != 1) {
                fprintf(stderr, "Error: key file ended early or bad data at row %d col %d\n", i + 1, j + 1);
                return -1;
            }
        }
    }
    *n_out = n;
    return 0;
}

/* print with %4d, one newline before title, none extra after block */
void print_matrix(const int key[MAX_N][MAX_N], int n) {
    printf("\nKey matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", key[i][j]);
        }
        printf("\n");
    }
}

int read_plaintext(FILE *pf, char buf[MAX_TEXT], int *len_out) {
    int c, k = 0;
    while ((c = fgetc(pf)) != EOF) {
        if (isalpha((unsigned char)c)) {
            char lc = (char)tolower((unsigned char)c);
            if (k < MAX_TEXT - 1) {
                buf[k++] = lc;
            } else {
                fprintf(stderr, "Error: plaintext exceeds buffer capacity (%d)\n", MAX_TEXT - 1);
                break;
            }
        }
    }
    buf[k] = '\0';
    *len_out = k;
    return 0;
}

//prints strings w up to 80 chars ending with '\n'
void print_80cols(const char *s, int len) {
    for (int i = 0; i < len; i += 80) {
        int chunk = (len - i >= 80) ? 80 : (len - i);
        if (chunk > 0) {
            fwrite(s + i, 1, (size_t)chunk, stdout);
            fputc('\n', stdout);
        }
    }
}

//pad with x
void pad_plaintext(char buf[MAX_TEXT], int *len_inout, int n) {
    int len = *len_inout;
    int r = len % n;
    int pad = (r == 0) ? 0 : (n - r);
    if (len + pad >= MAX_TEXT) {
        pad = (MAX_TEXT - 1) - len;
        if (pad < 0) pad = 0;
    }
    for (int i = 0; i < pad; i++) buf[len + i] = 'x';
    len += pad;
    buf[len] = '\0';
    *len_inout = len;
}

int mod26(int x) {
    int m = x % 26;
    if (m < 0) m += 26;
    return m;
}

//encryption
void encrypt(const int key[MAX_N][MAX_N], int n, const char *plain, int len, char *cipher) {
    for (int i = 0; i < len; i += n) {
        for (int r = 0; r < n; r++) {
            int sum = 0;
            for (int c = 0; c < n; c++) {
                int p = plain[i + c] - 'a';
                sum += key[r][c] * p;
            }
            cipher[i + r] = (char)('a' + mod26(sum));
        }
    }
    cipher[len] = '\0';
}

/*=============================================================================
| I Austin Vodrazka (5524103) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/