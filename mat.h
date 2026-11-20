#ifndef MAT_H
#define MAT_H

// The include guard above makes it safe to #include "mat.h" more than once
// in the same file, which happens once headers start including each other.

#include <stddef.h>

// Matrix stored row-major in a flat array: element (i, j) is data[i * cols + j].
// A Mat from mat_alloc owns its data and must be released with mat_free.
// A Mat built from a compound literal doesn't. Either way, copying a Mat
// still points at the same numbers.
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

// Functions that write a result take the output matrix first, like
// memcpy(dst, src). Shapes are checked with assert.

// New rows x cols matrix on the heap. Contents start as garbage, so fill it
// before reading. Release it with mat_free.
Mat mat_alloc(size_t rows, size_t cols);

// Release a matrix from mat_alloc. Don't use m afterwards.
void mat_free(Mat m);

// Print m one row per line.
void mat_print(Mat m);

// Set every element of m to x.
void mat_fill(Mat m, float x);

// Set m to the identity: 1s on the diagonal, 0s elsewhere. m must be square.
void mat_identity(Mat m);

// c = a + b, element by element. All three must be the same shape.
// c may be the same matrix as a or b: mat_add(a, a, b) does a += b.
void mat_add(Mat c, Mat a, Mat b);

// c = a - b, element by element. All three must be the same shape.
// c may be the same matrix as a or b: mat_sub(a, a, b) does a -= b.
void mat_sub(Mat c, Mat a, Mat b);

// c = s * a: every element of a multiplied by s. c must be the same shape as a.
// c may be a itself: mat_scale(a, a, 0.5f) halves a in place.
void mat_scale(Mat c, Mat a, float s);

// c = a * b. Requires a.cols == b.rows, and c sized a.rows x b.cols with its
// own storage (c can't be a or b).
void mat_mul(Mat c, Mat a, Mat b);

// c = a transposed: rows become columns, so c(j, i) = a(i, j).
// Requires c sized a.cols x a.rows, with its own storage (c can't be a).
void mat_transpose(Mat c, Mat a);

#endif
