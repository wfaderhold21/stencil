#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

//#define M 8
static const int M = 10;
static const int nr_blocks = 1024;

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

__global__ void compute(const float * a, float * b)
{
    int i = blockIdx.x;
    int j;    

    for (j = 0; j < M; j++) {
        if ((i + j * nr_blocks) > 0 && (i + j * nr_blocks) < M) {
            b[i + j * nr_blocks] = 0.2 * (a[M+((i+j*nr_blocks)-1)] + a[M+(i+j*nr_blocks)] + a[M+((i+j*nr_blocks)+1)] + a[(i+j*nr_blocks)] + a[2*M+(i+j*nr_blocks)]);
        }
    } 
}

int main(int argc, char ** argv) {
    float ** a, ** b, * c;
    float * c_a, * c_b;
    int i = 0;
    int j = 0, k = 0;
    struct timespec time1;
    struct timespec time2;
    struct timespec result;

    a = (float **) malloc(sizeof(float *) * M);
    b = (float **) malloc(sizeof(float *) * M);
    cudaMalloc((void **)&c_a, sizeof(float) * M * M);
    cudaMalloc((void **)&c_b, sizeof(float) * M * M);
    for (j = 0; j < M; j++) {
        a[j] = (float *) malloc(sizeof(float) * M);
        b[j] = (float *) malloc(sizeof(float) * M);
        
        memset(a[j], 0, sizeof(float) * M);
        memset(b[j], 0, sizeof(float) * M);
    }

    for (j = 0; j < M; j++) {
        a[j][0] = 1;
    }
    for (j = 0; j < M; j++) {
        a[j][M-1] = 1;
    } 
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

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

    for (i = 0;i < 10;i++) {
#ifdef DEBUG
        printf("Iter: %d\n", i);
        fflush(stdout);
#endif
        for (j = 1; j < M - 1; j++) {
            cudaMemcpy(c_a, a[j - 1], M * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(&c_a[M], a[j], M * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(&c_a[2*M], a[j+1], M * sizeof(float), cudaMemcpyHostToDevice);
            cudaMemcpy(c_b, b[j], M * sizeof(float), cudaMemcpyHostToDevice);
            
            compute<<<nr_blocks, 1>>>(c_a, c_b);
            cudaMemcpy(b[j], c_b, M * sizeof(float), cudaMemcpyDeviceToHost);
        }
        
        //printf("[debug] updating a with b\n");
        for (j = 1; j < M - 1; j++) {
            for (k = 1; k < M - 1; k++) {
                a[j][k] = b[j][k];
            }
        }
        #ifdef DEBUG
        printf("[debug output of b]\n");
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
    printf("timing: %lu.%.0f sec\n", result.tv_sec, (float)(result.tv_nsec / 1000000.0));

    free(a);
    free(b);
    cudaFree(c_a);
    cudaFree(c_b);

    return 0;
}
