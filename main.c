// Demo of the matrix functions in mat.h. Each block prints what it expects
// in a comment above it, so a wrong result stands out.

#include <stdio.h>

#include "mat.h"

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

    // Heap matrix sized from a variable. Expected: 1s on the diagonal.
    size_t n = 3;
    Mat id = mat_alloc(n, n);
    printf("%zux%zu identity:\n", n, n);
    mat_identity(id);
    mat_print(id);
    mat_free(id);

    return 0;
}
