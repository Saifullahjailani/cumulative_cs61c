
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

void fill(double *data, int size, double val) {
    // Task 1.5 TODO
    __m256d vals = _mm256_set1_pd (val);
    int next;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        next = chunks * threads;

        for(int i=id*chunks; i<(id + 1) * chunks; i+=4)
            _mm256_storeu_pd ((double*) (data+i),(__m256d) (vals));
    }
    for(int i = next; i < size; i++){
        data[i] = val;
    }
}

void abs_data(double *destination, const double *source, int size){
    __m256d zero = _mm256_set1_pd (0.0);
    __m256d neg_1 = _mm256_set1_pd (-1.0);
    int next;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        next = chunks * threads;

        for(int i=id*chunks; i<(id + 1) * chunks; i+=4){
            
            __m256d data = _mm256_loadu_pd (source+i);
            __m256d mask = _mm256_cmp_pd (data, zero, _CMP_LT_OS);
            __m256d negative = _mm256_and_pd(mask, data);
            negative = _mm256_mul_pd(negative, neg_1);
            __m256d positive = _mm256_andnot_pd(mask, data);

            _mm256_storeu_pd (destination+i,_mm256_add_pd(positive, negative));
        }
            
    }
    for(int i = next; i < size; i++){
        if(source[i] >= 0){
            destination[i] = source[i];
        }else{
            destination[i] = -source[i];
        }
    }

}

void negate(double *destination, const double *source, int size){
    __m256d neg_1 = _mm256_set1_pd (-1.0);
    int next;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        next = chunks * threads;

        for(int i=id*chunks; i<(id + 1) * chunks; i+=4){
            __m256d data = _mm256_loadu_pd (source+i);
            data = _mm256_mul_pd(data, neg_1);
            _mm256_storeu_pd (destination+i,data);
        }
            
    }
    for(int i = next; i < size; i++){
        destination[i] = -source[i];
    }
}

void add(double *destination, const double *op1, const double *op2, const int size){
    int next;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        next = chunks * threads;

        for(int i=id*chunks; i<(id + 1) * chunks; i+=4){
            __m256d data1 = _mm256_loadu_pd (op1+i);
            __m256d data2 = _mm256_loadu_pd (op2+i);
            __m256d data = _mm256_add_pd(data1, data2);
            _mm256_storeu_pd (destination+i,data);
        }
            
    }
    for(int i = next; i < size; i++){
        destination[i] = op1[i] + op2[i];
    }

}

void sub(double *destination, const double *op1, const double *op2, const int size){
    int next;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        next = chunks * threads;

        for(int i=id*chunks; i<(id + 1) * chunks; i+=4){
            __m256d data1 = _mm256_loadu_pd (op1+i);
            __m256d data2 = _mm256_loadu_pd (op2+i);
            __m256d data = _mm256_sub_pd(data1, data2);
            _mm256_storeu_pd (destination+i,data);
        }
            
    }
    for(int i = next; i < size; i++){
        destination[i] = op1[i] - op2[i];
    }

}
double dot(double* x, double* y, int size) {
    double global_sum = 0.0;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        int next = chunks * threads;
        int i;
        double local_sum = 0;
        for(i=id*chunks; i<(id + 1) * chunks; i+=4){
            __m256d data1 = _mm256_loadu_pd (x+i);
            __m256d data2 = _mm256_loadu_pd (y+i);
            __m256d d = _mm256_mul_pd(data1, data2);
            double s[4];
            _mm256_storeu_pd (s,d);
            local_sum += s[0] + s[1] + s[2] + s[3];
        }
        
        for(int i = next; i < size; i++){
            local_sum += (x[i] * y[i]);
        }
        #pragma omp critical
            global_sum += local_sum;
    }
    
    return global_sum;
}

double* transpose(double *const matr, int rows, int cols){
    double *data = malloc(sizeof(double) * rows * cols);
    int i, j;
    #pragma omp parallel for collapse(2) private(j)
    for(i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            data[j*rows + i] = matr[i * cols + j];
        }
    }
    return data;
}

double dot_vecs(double *const vec1, double *const vec2, const int size){
    int next;
    double global_sum =0;
    #pragma omp parallel
    {
        int threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int chunks = size / threads;
        next = chunks * threads;
        int i;
        __m256d sum = _mm256_set1_pd (0.0);

        for(i=id*chunks; i<(id + 1) * chunks; i+=4){
            __m256d data1 = _mm256_loadu_pd (vec1+i);
            __m256d data2 = _mm256_loadu_pd (vec2+i);
            __m256d data = _mm256_mul_pd(data1, data2);
            sum = _mm256_add_pd(data, sum);
        }
        double tmp[4];
        _mm256_storeu_pd (tmp, sum);
        for(int j = id*chunks/4*4; j < (id + 1) * chunks; j++){
            tmp[0] += (vec1[i] * vec2[j]);
        }
        #pragma omp critical
            global_sum += (tmp[0] + tmp[1] + tmp[2] + tmp[3]);
    }
    for(int i = next; i < size; i++){
        global_sum += (vec1[i] * vec2[i]);
    }
    return global_sum;
}

double multiply(double *const result, double *const m1, double *const m2,int row1, int col1, int col2){
    double *t = transpose(m2, col1, col2);
    #pragma omp parallel
    {
        #pragma omp for collapse(2)
        for(int i = 0; i < row1; i++){
            for(int j = 0; j < col1; j++){
                
            }
        }
    }

}

void print_datqa(double* d , int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%f ", d[col * i + j]);
        }
        printf("\n");
    }
}
int main(){
    double *d = malloc(sizeof(double) * 20);
    for(int i = 0; i < 20; i++){
        d[i] = i+1;
    }
    
    
    double ans = dot(d, d, 20);
    printf("%f\n", ans);
    
}

