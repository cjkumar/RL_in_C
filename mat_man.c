#include <stdio.h>

typedef struct {
    size_t rows, cols;
    float *data;
} Mat;


void mat_print(Mat m) {
    for (size_t i=0; i<m.rows; ++i) {
        for (size_t j=0; j<m.cols; ++j) {
            printf(" %f", m.data[i * m.cols+j]);
        }
        printf("\n");
    }
}

Mat mat_mul(Mat a, Mat b, Mat c) {
    return
}



int main(void)
{
    Mat a = {.rows = 2, .cols = 2, .data= (float[]){1,2,3,4}};
    mat_print(a);
    return 0;
}