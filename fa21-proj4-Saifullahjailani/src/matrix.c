#include "matrix.h"
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

void fill(double *data, int size, double val) ;

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/* Generates a random double between low and high */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col) {
    return mat->data[mat->cols * row + col];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val) {
    mat->data[mat->cols * row + col] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    if(rows <= 0 || cols <= 0){
        return -1;
    }
    matrix *my_matrix = malloc(sizeof(matrix));
    if(!my_matrix){
        return -2;
    }
    int size = rows * cols;
    double *data = calloc(size, sizeof(double));
    if(!data){
        return -2;
    }
     
    my_matrix->data = data;
    my_matrix->rows = rows;
    my_matrix->cols = cols;
    my_matrix->parent = NULL;
    my_matrix->ref_cnt = 1;
    
    *mat = my_matrix;
    return 0;

}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat) {
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
    if(!mat){
        return;
    }
    if(!mat->parent){
        mat->ref_cnt--;
        if(!mat->ref_cnt){
            free(mat->data);
            free(mat);
        }
    } else{
        deallocate_matrix(mat->parent);
        free(mat);
    }
    
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols) {
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if(rows <= 0 || cols <= 0){
        return -1;
    }
    matrix *my_matrix = malloc(sizeof(matrix));
    if(!my_matrix){
        return -2;
    }
    my_matrix->data = from->data + offset;
    my_matrix->rows = rows;
    my_matrix->cols = cols;
    my_matrix->parent = from;
    my_matrix->parent->ref_cnt += 1;
    *mat = my_matrix;

    return 0;

    
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val) {
    // Task 1.5 TODO
    fill(mat->data, mat->rows * mat->cols, val);
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat) {
    // Task 1.5 TODO
    __m256d neg_1 = _mm256_set1_pd (-1.0);
    int size = mat->rows * mat->cols;
        #pragma omp parallel for
        for(int i=0; i<size/16*16; i+=16){
            
            __m256d data = _mm256_loadu_pd (mat->data+i);
            __m256d negative = _mm256_mul_pd(neg_1, data);
            _mm256_storeu_pd (result->data+i,_mm256_max_pd(data, negative));

            data = _mm256_loadu_pd (mat->data+i+4);
            negative = _mm256_mul_pd(neg_1, data);
            _mm256_storeu_pd (result->data+i+4,_mm256_max_pd(data, negative));

            data = _mm256_loadu_pd (mat->data+i+8);
            negative = _mm256_mul_pd(neg_1, data);
            _mm256_storeu_pd (result->data+i+8,_mm256_max_pd(data, negative));

            data = _mm256_loadu_pd (mat->data+i+12);
            negative = _mm256_mul_pd(neg_1, data);
            _mm256_storeu_pd (result->data+i+12,_mm256_max_pd(data, negative));
            
        }

        for(int i = size/16*16; i < size; i++){
            if(mat->data[i] >= 0){
                result->data[i] = mat->data[i];
            }else{
                result->data[i] = -mat->data[i];
            }
        }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat) {
    __m256d neg_1 = _mm256_set1_pd (-1.0);
    int size = mat->rows * mat->cols;
    #pragma omp parallel for 
        for(int i = 0; i< size/16*16; i+=16){
            __m256d data = _mm256_loadu_pd (mat->data+i);
            data = _mm256_mul_pd(data, neg_1);
            _mm256_storeu_pd (result->data+i,data);

            data = _mm256_loadu_pd (mat->data+i+4);
            data = _mm256_mul_pd(data, neg_1);
            _mm256_storeu_pd (result->data+i+4,data);

            data = _mm256_loadu_pd (mat->data+i+8);
            data = _mm256_mul_pd(data, neg_1);
            _mm256_storeu_pd (result->data+i+8,data);

            data = _mm256_loadu_pd (mat->data+i+12);
            data = _mm256_mul_pd(data, neg_1);
            _mm256_storeu_pd (result->data+i+12,data);

        }

    for(int i = size/16*16; i < size; i++){
        result->data[i] = -mat->data[i];
    }
    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.5 TODO
// Task 1.5 TODO
    int size = mat1->rows * mat1->cols;
    #pragma omp parallel for 
    for(int i=0; i<size/16*16; i+=16){
        __m256d data1 = _mm256_loadu_pd (mat1->data+i);
        __m256d data2 = _mm256_loadu_pd (mat2->data+i);
        __m256d data = _mm256_add_pd(data1, data2);
        _mm256_storeu_pd (result->data+i,data);

        data1 = _mm256_loadu_pd (mat1->data+i+4);
        data2 = _mm256_loadu_pd (mat2->data+i+4);
        data = _mm256_add_pd(data1, data2);
        _mm256_storeu_pd (result->data+i+4,data);

        data1 = _mm256_loadu_pd (mat1->data+i+8);
        data2 = _mm256_loadu_pd (mat2->data+i+8);
        data = _mm256_add_pd(data1, data2);
        _mm256_storeu_pd (result->data+i+8,data);

        data1 = _mm256_loadu_pd (mat1->data+i+12);
        data2 = _mm256_loadu_pd (mat2->data+i+12);
        data = _mm256_add_pd(data1, data2);
        _mm256_storeu_pd (result->data+i+12,data);
        }
    for(int i = size/16*16; i < size; i++){
        result->data[i] = mat1->data[i] + mat2->data[i];
        }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.5 TODO
    int size = mat1->rows * mat1->cols;

  #pragma omp parallel for
        for(int i=0; i < size/16*16; i+=16){
            __m256d data1 = _mm256_loadu_pd (mat1->data+i);
            __m256d data2 = _mm256_loadu_pd (mat2->data+i);
            __m256d data = _mm256_sub_pd(data1, data2);
            _mm256_storeu_pd (result->data+i,data);

            data1 = _mm256_loadu_pd (mat1->data+i+4);
            data2 = _mm256_loadu_pd (mat2->data+i+4);
            data = _mm256_sub_pd(data1, data2);
            _mm256_storeu_pd (result->data+i+4,data);

            data1 = _mm256_loadu_pd (mat1->data+i+8);
            data2 = _mm256_loadu_pd (mat2->data+i+8);
            data = _mm256_sub_pd(data1, data2);
            _mm256_storeu_pd (result->data+i+8,data);

            data1 = _mm256_loadu_pd (mat1->data+i+12);
            data2 = _mm256_loadu_pd (mat2->data+i+12);
            data = _mm256_sub_pd(data1, data2);
            _mm256_storeu_pd (result->data+i+12,data);

        }
    for(int i = size /16*16; i < size; i++){
        result->data[i] = mat1->data[i] - mat2->data[i];
    }
    return 0;
}

/*
* This function returns a new transposed function of
*/

matrix* transpose(matrix *const matr){
    int size = matr->cols * matr->rows;
    matrix* result = malloc(sizeof(matrix));
    result->rows = matr->cols;
    result->cols = matr->rows;
    result->parent = NULL;
    result->ref_cnt = 1;
    result->data = malloc(sizeof(double) * size);
    int i, j;
    #pragma omp parallel for collapse(2) private(j)
    for(i = 0; i < matr->rows; i++){
        for(j = 0; j < matr->cols; j++){
            result->data[j*matr->rows + i] = matr->data[i * matr->cols + j];
        }
    }
    return result;
}


/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    // Task 1.6 TODO
    matrix *t = transpose(mat2);

    #pragma omp parallel for collapse(2) 
        for(int i = 0; i < mat1->rows; i++){
            for(int m = 0; m < t->rows; m++){
                __m256d vec_sum = _mm256_setzero_pd();
                double sum = 0.0;
                for(int k = 0; k < mat1->cols/16*16; k+=16){
                    __m256d d1 = _mm256_loadu_pd(mat1->data + i * mat1->cols + k);
                    __m256d d2 = _mm256_loadu_pd(t->data + m * t->cols + k);
                    vec_sum = _mm256_fmadd_pd(d1, d2, vec_sum);

                    d1 = _mm256_loadu_pd(mat1->data + i * mat1->cols + k + 4);
                    d2 = _mm256_loadu_pd(t->data + m * t->cols + k + 4);
                    vec_sum = _mm256_fmadd_pd(d1, d2, vec_sum);

                    d1 = _mm256_loadu_pd(mat1->data + i * mat1->cols + k + 8);
                    d2 = _mm256_loadu_pd(t->data + m * t->cols + k + 8);
                    vec_sum = _mm256_fmadd_pd(d1, d2, vec_sum);

                    d1 = _mm256_loadu_pd(mat1->data + i * mat1->cols + k + 12);
                    d2 = _mm256_loadu_pd(t->data + m * t->cols + k + 12);
                    vec_sum = _mm256_fmadd_pd(d1, d2, vec_sum);
                }

                double storage[4];
                _mm256_storeu_pd(storage, vec_sum);
                sum = vec_sum[0] + vec_sum[1] + vec_sum[2] + vec_sum[3];
                for(int j = t->cols/16*16; j < mat1->cols; j++){
                    sum += t->data[m * t->cols + j]  * mat1->data[i * mat1->cols + j];
                }
                result->data[i * result->cols + m] = sum;
            }
        }
    deallocate_matrix(t);
    return 0;
}




/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    // Task 1.6 TODO
    if(pow == 0){
        #pragma omp parallel for
        for(int i = 0; i < result->rows; i++){
            set(result, i, i, 1);
        }
        return 0;
    }
    else if(pow == 1){
        #pragma omp parallel for 
        for(int i = 0; i < result->rows; i++){
            for(int j = 0; j < result->cols; j++){
                set(result, i, j, get(mat, i, j));      
            }
        }

        return 0;
    }
    else if(pow == 2){
        mul_matrix(result, mat, mat);
        return 0;
    }
    else{
        int p = pow / 2;
        int r = pow % 2;
        matrix *pow2;
        allocate_matrix(&pow2, result->rows, result->cols);
        pow_matrix(pow2, mat, 2);
        pow_matrix(result, pow2, p);
        deallocate_matrix(pow2);

        if(r == 1){
            matrix *tmp;
            allocate_matrix(&tmp, result->rows, result->cols);
            mul_matrix(tmp, result, mat);
            pow_matrix(result, tmp, 1);
            deallocate_matrix(tmp);
        }
    }
    
    return 0;
}






/*
    These functions are optimized helper functions
*/
void fill(double *data, int size, double val) {
    // Task 1.5 TODO
    __m256d vals = _mm256_set1_pd (val);
    #pragma omp parallel for
        for(int i=0; i<size/16*16; i+=16)
        {
            _mm256_storeu_pd (data+i, vals);
            _mm256_storeu_pd (data+i+4, vals);
            _mm256_storeu_pd (data+i+8, vals);
            _mm256_storeu_pd (data+i+12, vals);
        }

    
    for(int i = size/16*16; i < size; i++){
        data[i] = val;
    }
}





