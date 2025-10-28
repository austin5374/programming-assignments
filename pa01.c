/*============================================================================
| Assignment: pa01 - Calculate the checksum of an input file given:
| -> the name of the input file,
| -> the checksum size of either 8, 16, or 32 bits
| Author: Austin
| Language: c
| To Compile: gcc -o pa01 pa01.c
| To Execute: ./pa01 inputFilename.txt checksumSize
| where inputFilename.txt is the input file
| and checksumSize is either 8, 16, or 32
| Note: All input files are simple 8 bit ASCII text files
| All compile and run commands have been tested on Eustis
| Class: CIS3360 - Security in Computing - Spring 2025
| Instructor: McAlpin
| Due Date: 8/21/25
+===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char**argv){
    FILE* f = NULL;               /* file ptr */
    unsigned char* B = NULL;      /* buffer  */
    int CAP = 0;                  /* capacity */
    int N = 0;                    /* length */
    int bits = 0;                 /* checksum size */
    int col = 0;                  /* echo column counter */
    int pad = 0;                  /* how many 'X' */
    int i,j;                      /* loops */
    int x;                        /* fgetc temp (must be int) */
    unsigned long sum = 0UL;      /* running sum */
    char *fname = NULL;           /* alias */

    if (argc!=3){
        fprintf(stderr,"usage: %s inputFilename.txt checksumSize\n", argv[0]);
        return 1;
    }
    bits = atoi(argv[2]);
    if (!(bits==8 || bits==16 || bits==32)){
        fprintf(stderr,"Valid checksum sizes are 8, 16, or 32\n");
        return 1;
    }

    fname = argv[1];
    f = fopen(fname,"rb"); /* text files but rb ok */
    if(!f){ perror("fopen"); return 1; }

    /* read whole file.   */
    while( (x=fgetc(f)) != EOF ){
        if (CAP==N){
            int NEWC = (CAP==0? 256 : CAP*2);
            unsigned char *T = (unsigned char*)realloc(B,(size_t)NEWC);
            if(!T){
                fprintf(stderr,"alloc failed\n");
                fclose(f); free(B);
                return 1;
            }
            B = T; CAP = NEWC;
        }
        B[N++] = (unsigned char)x;
    }
    fclose(f);
    printf("\n");

    /* echo with wrap at exactly 80 columns */
    for(i=0;i<N;i++){
        unsigned char c=B[i];
        putchar((int)c);
        if(c=='\n'){ col=0; }
        else{
            col++;
            if(col==80){ putchar('\n'); col=0; }
        }
    }

    // figure padding for 16/32 only */
    pad = 0;
    if(bits==16){
        if( (N%2)!=0 ) pad = 1;
    } else if(bits==32){
        int r = (N%4);
        if(r!=0) pad = 4-r;
    }

    // echo padding 'X' chars (they count toward 80-col wrap)
    for(j=0;j<pad;j++){
        putchar('X');
        col++;
        if(col==80){ putchar('\n'); col=0; }
    }

    // ensure echo ends with newline if mid-line */
    if(col!=0){ putchar('\n'); col=0; }

    /* EXTRA blank line rule:
       - always for 8-bit
       - for 16/32 only when NO padding was added (pad==0) */
    if (bits==8 || pad==0){
        putchar('\n');
    }

    /* compute checksum */
    if(bits==8){
        sum = 0UL;
        for(i=0;i<N;i++){ sum += (unsigned long)B[i]; }
        for(j=0;j<pad;j++){ sum += (unsigned long)('X'); }
        sum &= 0xFFUL;
        printf("%2d bit checksum is %8lx for all %4d chars\n", 8, sum, N+pad);

    } else if(bits==16){
        sum = 0UL;
        {
            int total = N + pad;
            int k=0;
            while(k<total){
                unsigned int b0 = (k   < N)? (unsigned int)B[k]   : (unsigned int)('X');
                unsigned int b1 = (k+1 < N)? (unsigned int)B[k+1] : (unsigned int)('X');
                unsigned long w = (((unsigned long)b0)<<8) | ((unsigned long)b1); /* big-endian */
                sum += w;
                k += 2;
            }
        }
        sum &= 0xFFFFUL;
        printf("%2d bit checksum is %8lx for all %4d chars\n", 16, sum, N+pad);

    } else { /* 32 */
        sum = 0UL;
        {
            int total = N + pad;
            int t=0;
            while(t<total){
                unsigned int b0 = (t   < N)? (unsigned int)B[t]   : (unsigned int)('X');
                unsigned int b1 = (t+1 < N)? (unsigned int)B[t+1] : (unsigned int)('X');
                unsigned int b2 = (t+2 < N)? (unsigned int)B[t+2] : (unsigned int)('X');
                unsigned int b3 = (t+3 < N)? (unsigned int)B[t+3] : (unsigned int)('X');
                unsigned long w =
                    (((unsigned long)b0)<<24) |
                    (((unsigned long)b1)<<16) |
                    (((unsigned long)b2)<<8 ) |
                    ((unsigned long)b3); /* big-endian */
                sum += w;
                t += 4;
            }
        }
        sum &= 0xFFFFFFFFUL;
        printf("%2d bit checksum is %8lx for all %4d chars\n", 32, sum, N+pad);
    }

    free(B);
    return 0;
}

/*=============================================================================
| I [Austin Vodrazka] ([5524103]) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any other person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/