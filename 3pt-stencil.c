#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#define M 10

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
    int j = 0;
    struct timespec time1;
    struct timespec time2;
    struct timespec result;

    a = (double *) malloc(sizeof(double) * M);
    b = (double *) malloc(sizeof(double) * M);
    memset(a, 0, sizeof(double) * M);
    memset(b, 0, sizeof(double) * M);


    a[M - 1] = M - 1;
    b[M - 1] = M - 1;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

#ifdef DEBUG
    printf("[debug]\n");
    for (j = 0; j < M; j++) {
        printf("%g ", a[j]);
    }
    printf("\n");
#endif /* DEBUG */

    for (;;i++) {
        for (j = 1; j < M - 1; j++) {
            b[j] = 0.333 * (a[j - 1] + a[j] + a[j + 1]);
        }
        
        // local update
        if (memcmp(a, b, sizeof(double) * M) == 0) {
            printf("convergence met at step %d\n", i);
            break;
        }
        memcpy(&a[0], &b[0], sizeof(double) * M);

        #ifdef DEBUG
        for (j = 0; j < M; j++) {
            printf("%g ", a[j]);
        }
        printf("\n");
        #endif /* DEBUG */ 
    } 
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);                                
    result = mydifftime(time1, time2);
    printf("timing: %lu.%.0f sec\n", result.tv_sec, (double)(result.tv_nsec / 1000000.0));
//    printf("timing sec: %lu, nsec %lu, millisec %f\n", result.tv_sec, result.tv_nsec, (double)(result.tv_nsec / 1000000.0));

    return 0;
}
