#include <stdio.h>

// Matrix stored row-major in a flat array: element (i, j) is data[i * cols + j].
// Mat doesn't own its data, so copying a Mat still points at the same numbers.
typedef struct {
    size_t rows, cols;
    float *data;
} Mat;


// Print m one row per line.
void mat_print(Mat m) {
    for (size_t i=0; i<m.rows; ++i) {
        for (size_t j=0; j<m.cols; ++j) {
            printf(" %f", m.data[i * m.cols+j]);
        }
        printf("\n");
    }
}

// c = a * b. Output comes first, like memcpy(dst, src).
// Requires a.cols == b.rows, and c sized a.rows x b.cols with its own storage.
void mat_mul(Mat c, Mat a, Mat b) {
    for (size_t i=0; i<c.rows; ++i) {
        for (size_t j=0; j<c.cols; ++j) {
            u_int32_t c_ix = i * c.cols + j;
            c.data[c_ix] = 0.0f;
            // Dot product of row i of a with column j of b.
            for (size_t k = 0; k<a.cols; ++k) {
                c.data[c_ix] += a.data[i * a.cols + k] * b.data[k*b.cols+j];
            }
        }
    } 
}



int main(void)
{
    // Compound literals give each matrix storage that lives until main returns.
    Mat a = {.rows = 2, .cols = 2, .data= (float[]){1,2,3,4}};
    Mat b = {.rows = 2, .cols = 2, .data= (float[]){4,3,2,1}};
    Mat c = {.rows = 2, .cols = 2, .data= (float[4]){0}};
    mat_print(a);

    mat_mul(c,a,b);

    // Expected: 8 5 / 20 13
    mat_print(c);

    // Passing a Mat copies the whole struct; a pointer would only copy 8 bytes.
    printf("size of Mat pointer: %zu\n", sizeof (Mat *));
    printf("size of Mat         :%zu\n", sizeof (Mat ));

    return 0;
}
