#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include "cycles.c"

#define M 1000

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
    cycles_t * tstamp;
    struct report_options report = {};

    tstamp = (cycles_t *) malloc(sizeof(cycles_t) * 2);

    a = (double **) malloc(sizeof(double *) * M);
    b = (double **) malloc(sizeof(double *) * M);
    for (j = 0; j < M; j++) {
        a[j] = (double *) malloc(sizeof(double) * M);
        b[j] = (double *) malloc(sizeof(double) * M);
        
        memset(a[j], 0, sizeof(double) * M);
        memset(b[j], 0, sizeof(double) * M);
    }


    // init
    for (j = 0; j < M; j++) {
        a[j][0] = 1;
    }
    for (j = 0; j < M; j++) {
        a[j][M-1] = 1;
    } 


    tstamp[0] = get_cycles();
    for (i = 0; i < 10; i++) {
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
    } 
    tstamp[1] = get_cycles();
    printf("Timing results:\n");
    print_report(&report, 1, tstamp);

    return 0;
}
