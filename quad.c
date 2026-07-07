#include <stdio.h>
#include <lapacke.h>

int main() {
    // Matrix size is 2x2 for a quadratic equation
    lapack_int n = 3;
    lapack_int lda = 3;
    lapack_int info;

    /* * Companion Matrix for x^2 - 2x - 1.5 = 0
     * LAPACKE defaults to Row-Major layout in standard C configurations:
     * Row 0: 0.0,  1.5
     * Row 1: 1.0,  2.0
     */
//    double A[4] = {
//        0.0, 2,// 1.5,
//        1.0,1// 2.0
//    };
//
    double A[9] = {
    0.0,  0.0,   6.0,  // Row 0
    1.0,  0.0, -11.0,  // Row 1
    0.0,  1.0,   6.0   // Row 2
};
    // Arrays to store the real and imaginary parts of the roots (eigenvalues)
    double wr[3];
    double wi[3];

    // We only need eigenvalues, not eigenvectors
    char jobvl = 'N'; 
    char jobvr = 'N';
    lapack_int ldvl = 1;
    lapack_int ldvr = 1;
    double vl[1];
    double vr[1];

    // Call LAPACKE_dgeev to calculate eigenvalues
    info = LAPACKE_dgeev(LAPACK_ROW_MAJOR, jobvl, jobvr, n, A, lda, 
                         wr, wi, vl, ldvl, vr, ldvr);

    // Check for success
    if (info == 0) {
        printf("The roots of y = 2x^2 - 4x - 3 are:\n");
        for (int i = 0; i < n; i++) {
            if (wi[i] == 0.0) {
                // Real root
                printf("x%d = %f\n", i + 1, wr[i]);
            } else {
                // Complex root (just in case)
                printf("x%d = %f + %fi\n", i + 1, wr[i], wi[i]);
            }
        }
    } else {
        printf("LAPACK failed to compute eigenvalues. Info code: %d\n", info);
    }

    return 0;
}
