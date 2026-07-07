#include <stdio.h>
#include <cblas.h>

int main() {
    // Matrix dimensions: A (2x3) and B (3x2) -> Result C (2x2)
    int m = 2, k = 3, n = 2;
    
    // OpenBLAS/CBLAS expects 1D arrays representing 2D matrices
    // Row-Major layout: elements of a row are contiguous in memory
    double A[] = {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0
    };

    double B[] = {
        1.0, 2.0,
        3.0, 4.0,
        5.0, 6.0
    };

    double C[] = {
        0.0, 0.0,
        0.0, 0.0
    };

    // Parameters for scaling
    double alpha = 1.0;
    double beta = 0.0;

    // Perform C = alpha*A*B + beta*C
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);

    // Print the result
    printf("Result Matrix C (2x2):\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%f ", C[i * n + j]);
        }
        printf("\n");
    }

    return 0;
}
