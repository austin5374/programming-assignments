/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
*/

#include <stdio.h>
#include <stdlib.h>

#define PRIMELIMIT 110000
#define NUMPRIMES  10000

int main() {
    int i, j;
    int *sieve = (int *)malloc((PRIMELIMIT + 1) * sizeof(int));
    for (i = 0; i <= PRIMELIMIT; i++) sieve[i] = 1;
    sieve[0] = 0;
    sieve[1] = 0;
    for (i = 2; i * i <= PRIMELIMIT; i++) {
        if (sieve[i]) {
            for (j = i * i; j <= PRIMELIMIT; j += i) {
                sieve[j] = 0;
            }
        }
    }
    long long *primesum = (long long *)calloc(NUMPRIMES, sizeof(long long));
    i = 2;
    j = 0;
    while (j < NUMPRIMES && i <= PRIMELIMIT) {
        if (sieve[i]) {
            primesum[j] = i;
            j++;
        }
        i++;
    }
    free(sieve);
    for (i = 1; i < NUMPRIMES; i++) {
        primesum[i] = primesum[i] + primesum[i - 1];
    }
    int numCases, n;
    scanf("%d", &numCases);
    for (i = 0; i < numCases; i++) {
        scanf("%d", &n);
        printf("%lld\n", primesum[n - 1]);
    }
    free(primesum);
    return 0;
}
