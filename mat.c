// Matrix function definitions. What each one does is described in mat.h.

#include "mat.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

void mat_print(Mat m) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            printf(" %f", MAT_AT(m, i, j));
        }
        printf("\n");
    }
}

void mat_fill(Mat m, float x) {
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MAT_AT(m, i, j) = x;
        }
    }
}

void mat_identity(Mat m) {
    assert(m.rows == m.cols);

    mat_fill(m, 0.0f);
    for (size_t i = 0; i < m.rows; ++i) {
        MAT_AT(m, i, i) = 1.0f;
    }
}

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

void mat_scale(Mat c, Mat a, float s) {
    assert(c.rows == a.rows);
    assert(c.cols == a.cols);

    for (size_t i = 0; i < c.rows; ++i) {
        for (size_t j = 0; j < c.cols; ++j) {
            MAT_AT(c, i, j) = s * MAT_AT(a, i, j);
        }
    }
}

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
