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
void matvec(float **A, float *x, float *b, int m, int n) {
  for (int row = 0; row < m; row += 1) {
    float sum = 0;
    for (int col = 0; col < n; col += 1) {
      sum += A[row][col] * x[col];
    }
    b[row] = sum;
  }
}


int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: .\\matvec m n\n");
    return -1;
  }

  const int m = atoi(argv[1]);
  const int n = atoi(argv[2]);

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