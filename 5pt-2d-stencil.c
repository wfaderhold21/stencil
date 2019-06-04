#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#define M 8

static inline struct timespec mydifftime(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if((end.tv_nsec-start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

int main(int argc, char ** argv) {
    double ** a, ** b;
    int i = 0;
    int j = 0, k = 0;
    struct timespec time1;
    struct timespec time2;
    struct timespec result;

    a = (double **) malloc(sizeof(double *) * M);
    b = (double **) malloc(sizeof(double *) * M);
    for (j = 0; j < M; j++) {
        a[j] = (double *) malloc(sizeof(double) * M);
        b[j] = (double *) malloc(sizeof(double) * M);
        
        memset(a[j], 0, sizeof(double) * M);
        memset(b[j], 0, sizeof(double) * M);
    }


    // init
    /*for (j = 1; j < M - 1; j++) {
        for (k = 1; k < M - 1; k++) {
            a[j][k] = (rand()*100)%10;
        }
    }*/
//    for (j = 0; j < M; j++) {
//        a[0][j] = 1;
//    }
    for (j = 0; j < M; j++) {
        a[j][0] = 1;
    }
    for (j = 0; j < M; j++) {
        a[j][M-1] = 1;
    } 
//    for (j = 0; j < M; j++) {
//        a[M - 1][j] = 1;
//    } 
//    a[M - 1][M - 1] = M - 1;
    //b[M - 1][M - 1] = M - 1;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

#ifdef DEBUG
    printf("[debug]\n");
    for (j = 0; j < M; j++) {
        for (k = 0; k < M; k++) {
            printf("%g ", a[j][k]);
        }
        printf("\n");
    }
    printf("\n\n");
#endif /* DEBUG */

    for (i = 0; i < 10; i++) {
//        int conv = 0;
        for (j = 1; j < M - 1; j++) {
            for (k = 1; k < M - 1; k++) {
                b[j][k] = 0.2 * 
                    (a[j - 1][k] + a[j][k] + a[j + 1][k] + a[j][k + 1] + a[j][k - 1]);
            }
        }
        
        for (j = 1; j < M - 1; j++) {
            for (k = 1; k < M - 1; k++) {
                a[j][k] = b[j][k];
            }
        }
        #ifdef DEBUG
        printf("[debug]\n");
        for (j = 0; j < M; j++) {
            for (k = 0; k < M; k++) {
                printf("%5.5g ", a[j][k]);
            }
            printf("\n");
        }
        printf("\n\n");
        #endif /* DEBUG */ 
    } 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);                                
    result = mydifftime(time1, time2);
    printf("timing: %lu.%.0f sec\n", result.tv_sec, (double)(result.tv_nsec / 1000000.0));
//    printf("timing sec: %lu, nsec %lu, millisec %f\n", result.tv_sec, result.tv_nsec, (double)(result.tv_nsec / 1000000.0));

    return 0;
}
