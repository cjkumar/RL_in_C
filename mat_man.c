#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// Matrix stored row-major in a flat array: element (i, j) is data[i * cols + j].
// Mat doesn't own its data, so copying a Mat still points at the same numbers.
typedef struct {
    size_t rows, cols;
    float *data;
} Mat;

// Element (i, j) of m. Works on both sides of '=':
//     float x = MAT_AT(m, 0, 1);
//     MAT_AT(m, 0, 1) = 5.0f;
// A macro is text substitution, not a function call. The parentheses around
// each argument stop MAT_AT(m, i + 1, j) from turning into i + 1 * cols.
// m is pasted in twice, so pass a plain variable, not an expression.
#define MAT_AT(m, i, j) ((m).data[(i) * (m).cols + (j)])


// Print m one row per line.
void mat_print(Mat m) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            printf(" %f", MAT_AT(m, i, j));
        }
        printf("\n");
    }
}

// Set every element of m to x.
void mat_fill(Mat m, float x) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = x;
        }
    }
}

// c = a + b, element by element. All three must be the same shape.
// c may be the same matrix as a or b: mat_add(a, a, b) does a += b.
void mat_add(Mat c, Mat a, Mat b) {
    assert(a.rows == b.rows);
    assert(a.cols == b.cols);
    assert(c.rows == a.rows);
    assert(c.cols == a.cols);

    for (size_t i = 0; i < c.rows; ++i) {
        for (size_t j = 0; j < c.cols; ++j) {
            MAT_AT(c, i, j) = MAT_AT(a, i, j) + MAT_AT(b, i, j);
        }
    }
}

// c = s * a: every element of a multiplied by s. c must be the same shape as a.
// c may be a itself: mat_scale(a, a, 0.5f) halves a in place.
void mat_scale(Mat c, Mat a, float s) {
    assert(c.rows == a.rows);
    assert(c.cols == a.cols);

    for (size_t i = 0; i < c.rows; ++i) {
        for (size_t j = 0; j < c.cols; ++j) {
            MAT_AT(c, i, j) = s * MAT_AT(a, i, j);
        }
    }
}

// c = a transposed: rows become columns, so c(j, i) = a(i, j).
// Requires c sized a.cols x a.rows, with its own storage (c can't be a).
void mat_transpose(Mat c, Mat a) {
    assert(c.rows == a.cols);
    assert(c.cols == a.rows);
    assert(c.data != a.data);

    for (size_t i = 0; i < a.rows; ++i) {
        for (size_t j = 0; j < a.cols; ++j) {
            MAT_AT(c, j, i) = MAT_AT(a, i, j);
        }
    }
}

void mat_sub(Mat c, Mat a, Mat b) {
    assert(a.rows == b.rows);
    assert(a.cols == b.cols);
    assert(c.rows == a.rows);
    assert(c.cols == a.cols);

    for (size_t i = 0; i < c.rows; ++i) {
        for (size_t j = 0; j < c.cols; ++j) {
            MAT_AT(c, i, j) = MAT_AT(a, i, j) - MAT_AT(b, i, j);
        }
    }
}

// c = a * b. Output comes first, like memcpy(dst, src).
// Requires a.cols == b.rows, and c sized a.rows x b.cols with its own storage.
void mat_mul(Mat c, Mat a, Mat b) {
    assert(a.cols == b.rows);
    assert(c.rows == a.rows);
    assert(c.cols == b.cols);

    for (size_t i = 0; i < c.rows; ++i) {
        for (size_t j = 0; j < c.cols; ++j) {
            MAT_AT(c, i, j) = 0.0f;
            // Dot product of row i of a with column j of b.
            for (size_t k = 0; k < a.cols; ++k) {
                MAT_AT(c, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
            }
        }
    }
}

Mat mat_alloc(size_t rows, size_t cols) {
    Mat m;
    m.rows = rows;
    m.cols = cols;
    m.data = malloc(rows * cols * sizeof(float));
    assert(m.data != NULL);
    return m; 
}

void mat_free(Mat m) {
    free(m.data);
}


void mat_identity(Mat m) {
    assert(m.rows == m.cols);

    mat_fill(m, 0.0f);
    for (size_t i = 0; i < m.rows; ++i) {
        MAT_AT(m, i, i) = 1.0f;
    }
}
int main(void)
{
    // Compound literals give each matrix storage that lives until main returns.
    Mat a = {.rows = 2, .cols = 2, .data= (float[]){1,2,3,4}};
    Mat b = {.rows = 2, .cols = 2, .data= (float[]){4,3,2,1}};
    Mat c = {.rows = 2, .cols = 2, .data= (float[4]){0}};

    printf("a:\n");
    mat_print(a);
    printf("b:\n");
    mat_print(b);

    // Expected: 8 5 / 20 13
    printf("a * b:\n");
    mat_mul(c, a, b);
    mat_print(c);

    // Expected: 5 5 / 5 5
    printf("a + b:\n");
    mat_add(c, a, b);
    mat_print(c);

    // Expected: 2 4 / 6 8
    printf("2 * a:\n");
    mat_scale(c, a, 2.0f);
    mat_print(c);

    // Expected: 7 7 / 7 7
    printf("c filled with 7:\n");
    mat_fill(c, 7.0f);
    mat_print(c);

    // A non-square matrix shows the transpose properly: 2x3 becomes 3x2.
    // Expected: 1 4 / 2 5 / 3 6
    Mat d = {.rows = 2, .cols = 3, .data= (float[]){1,2,3,4,5,6}};
    Mat t = {.rows = 3, .cols = 2, .data= (float[6]){0}};
    printf("d:\n");
    mat_print(d);
    printf("d transposed:\n");
    mat_transpose(t, d);
    mat_print(t);

    // MAT_AT reads and writes a single element. Row 1, column 2 of d is 6.
    printf("d(1, 2) = %f\n", MAT_AT(d, 1, 2));
    MAT_AT(d, 1, 2) = 60.0f;
    printf("d(1, 2) after writing 60 = %f\n", MAT_AT(d, 1, 2));

    // Passing a Mat copies the whole struct; a pointer would only copy 8 bytes.
    printf("size of Mat pointer: %zu\n", sizeof (Mat *));
    printf("size of Mat         :%zu\n", sizeof (Mat ));

    size_t n = 3;
    Mat id = mat_alloc(n, n);
    mat_identity(id);
    mat_print(id);
    mat_free(id);

    return 0;
}