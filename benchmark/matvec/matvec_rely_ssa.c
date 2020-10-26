/* Benchmark Application in C.
 *
 * Multiplying a matrix with a vector.
 * The result is a new vector.
 * Ax = b. A: m*n, x: n*1, b: m*1
 * 
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>
#include <stdlib.h>


// The main function: matrix vector multiplication.
// Ax = b. A: m*n, x: n*1, b: m*1
void matvec(float **A, float *x, float *b, const int m, const int n) {
  for (int row = 0; row < m; row += 1) {
    float sum = 0;
    for (int col = 0; col < n; col += 1) {
      sum += A[row][col] * x[col];
    }
    b[row] = sum;
  }
}


// Rely transformation of the function with m = 4 and n = 3.
// Some transformations:
// (1) Bounded while loop transformed into recursive function calls.
// (2) Conditional flattening and phi nodes.
float matvec_rely_ssa_compute_element(float **A, float *x, const int row) {
  float sum;
  const float sum0Before = 0;

  // x[0]
  const int col0 = 0;
  const int pred0 = col0 < 3;
  float sum0T;
  if (pred0) {
    const float prod0 = A[row][col0] * x[col0];
    const float sum1Before = sum0Before + prod0;

    // x[1]
    const int col1 = col0 + 1;
    const int pred1 = col1 < 3;
    float sum1T;
    if (pred1) {
      const float prod1 = A[row][col1] * x[col1];
      const float sum2Before = sum1Before + prod1;

      // x[2]
      const int col2 = col1 + 1;
      const int pred2 = col2 < 3;
      float sum2T;
      if (pred2) {
        const float prod2 = A[row][col2] * x[col2];
        const float sum3Before = sum2Before + prod2;

        // End of loop.
        const int col3 = col2 + 1;
        const int pred3 = col3 < 3;
        if (pred3) {} else { exit(1) /* Crash */; } 

        sum2T = sum3Before;
      } else {}
      sum1T = sum2T; // phi(sum2T, sum2Before, pred2).
    } else {}
    sum0T = sum1T; // phi(sum1T, sum1Before, pred1).
  } else {}
  sum = sum0T; // phi(sum0T, sum0Before, pred0).

  return sum;
}


void matvec_rely_ssa(float **A, float *x, float *b) {
  // b[0]
  const int row0 = 0;
  const int pred0 = row0 < 4;
  if (pred0) {
    const float b0 = matvec_rely_ssa_compute_element(A, x, row0);
    b[row0] = b0;

    // b[1]
    const int row1 = row0 + 1;
    const int pred1 = row1 < 4;
    if (pred1) {
      const float b1 = matvec_rely_ssa_compute_element(A, x, row1);
      b[row1] = b1;

      // b[2]
      const int row2 = row1 + 1;
      const int pred2 = row2 < 4;
      if (pred2) {
        const float b2 = matvec_rely_ssa_compute_element(A, x, row2);
        b[row2] = b2;

        // b[3]
        const int row3 = row2 + 1;
        const int pred3 = row3 < 4;
        if (pred3) {
          const float b3 = matvec_rely_ssa_compute_element(A, x, row3);
          b[row3] = b3;

          // End of the loop.
          const int row4 = row3 + 1;
          const int pred4 = row4 < 4;
          if (pred4) {} else { exit(1) /* Crash */; }

        } else {}
      } else {}
    } else {}
  } else {}
}


// Since m = 4, n = 3, unroll the whole thing.
// I believe it is better to apply the Rely analysis to rely_ssa.
// However, we can also try the unrolled one.
void matvec_unrolled(float **A, float *x, float *b) {
  const float sum00 = 0;
  const float product00 = A[0][0] * x[0];
  const float sum01 = sum00 + product00;
  const float product01 = A[0][1] * x[1];
  const float sum02 = sum01 + product01;
  const float product02 = A[0][2] * x[2];
  const float sum03 = sum02 + product02;
  b[0] = sum03;

  const float sum10 = 0;
  const float product10 = A[1][0] * x[0];
  const float sum11 = sum10 + product10;
  const float product11 = A[1][1] * x[1];
  const float sum12 = sum11 + product11;
  const float product12 = A[1][2] * x[2];
  const float sum13 = sum12 + product12;
  b[1] = sum13;

  const float sum20 = 0;
  const float product20 = A[2][0] * x[0];
  const float sum21 = sum20 + product20;
  const float product21 = A[2][1] * x[1];
  const float sum22 = sum21 + product21;
  const float product22 = A[2][2] * x[2];
  const float sum23 = sum22 + product22;
  b[2] = sum23;

  const float sum30 = 0;
  const float product30 = A[3][0] * x[0];
  const float sum31 = sum30 + product30;
  const float product31 = A[3][1] * x[1];
  const float sum32 = sum31 + product31;
  const float product32 = A[3][2] * x[2];
  const float sum33 = sum32 + product32;
  b[3] = sum33;
}


int main(int argc, char* argv[]) {
  const int m = 4;
  const int n = 3;

  // Ax = b. row-major memory allocation.
  float **A = malloc(m * sizeof(float*));
  for (int row = 0; row < m; row += 1) {
    A[row] = malloc(n * sizeof(float));
  }
  float *x = malloc(n * 1 * sizeof(float));
  float *b = malloc(m * 1 * sizeof(float));

  // Init A and x.
  float base = 0.5;
  for (int row = 0; row < m; row += 1) {
    for (int col = 0; col < n; col += 1) {
      A[row][col] = base;
      base += 0.5; 
    }
  }

  for (int col = 0; col < n; col += 1) {
    x[col] = base;
    base += 0.5;
  }

  // Main: matrix vector multiplication.
  matvec(A, x, b, m, n);
  // matvec_unrolled(A, x, b);
  // matvec_rely_ssa(A, x, b);

  // Print result b.
  printf("Result: ");
  for (int i = 0; i < m - 1; i += 1) {
    printf("%f ", b[i]);
  }
  printf("%f\n", b[m - 1]);

  // Free
  for (int row = 0; row < m; row += 1) {
    free(A[row]);
  }
  free(A);
  free(x);
  free(b);

  return 0;
}
