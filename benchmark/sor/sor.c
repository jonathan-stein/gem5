/* Benchmark Application in C.
 * The error injection function is sor.
 * The error injection function contains library calls.
 * Compile the code with: clang sor.c -lm
 *
 * Applying successive over relaxation.
 * The result is the solution vector.
 *
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>
#include <stdlib.h>


// Implementat the math sqrt() on our own. We need to run Rely analysis on that.
// Ref: https://stackoverflow.com/questions/3581528/how-is-the-square-root-function-implemented
// NOTE: it is very important not to use while loop. Rely analysis cannot handle unbounded loops.
// We use a fixed-iteration for loop instead.
float sqrtImpl(const float x) {
  float z = 1.0f;
  for (int i = 0; i < 30; i += 1) {
    z -= (z*z - x) / (2*z);
  }
  return z;
}

float norm(float *a, const int n) {
  float norm = 0;
  for (int i = 0; i < n; i++) {
    norm += (a[i] * a[i]);
  }
  norm = sqrtImpl(norm);
  return norm;
}


static const float tolerance = 0.00000001f;

void matvec(float **A, float *x, float *b, const int m, const int n) {
  for (int row = 0; row < m; row += 1) {
    float sum = 0;
    for (int col = 0; col < n; col += 1) {
      sum += A[row][col] * x[col];
    }
    b[row] = sum;
  }
}

void vecsub(float *a, float *b, float *result, const int n) {
  for (int i = 0; i < n; i++) {
    result[i] = a[i] - b[i];
  }
}

// A: n*n matrix
// b: n dimensional vector
// omega: relaxation factor
// phi: solution vector (starts as initial guess)
//
// The main function: sor.
//
// NOTE: To facilitate Rely analysis, we cannot have unbounded while loops.
// I replace the while loop with a fixed iteration for loop.
// It is kind of tricky, as it renders the residual value useless.
void sor(float **A, float *b, float omega, float* phi, const int n) {
  float multResult[n];
  float subResult[n];

  matvec(A, phi, multResult, n, n);
  vecsub(multResult, b, subResult, n);
  // float residual = norm(subResult, n);

  // while (residual > tolerance) {
  for (int i = 0; i < 40; i += 1) {
    for (int i = 0; i < n; i++) {
      float sigma = 0;
      for (int j = 0; j < n; j++) {
        if (j != i) {
          sigma += A[i][j] * phi[j];
        }
      }
      phi[i] = (1 - omega) * phi[i] + (omega / A[i][i]) * (b[i] - sigma);
    }

    matvec(A, phi, multResult, n, n);
    vecsub(multResult, b, subResult, n);
    // residual = norm(subResult, n);
  }
}


// System of equations:
// 4*x1 - x2 - 6*x3 = 2
// -5*x1 - 4*x2 + 10*x3 + 8*x4 = 21
// 9*x2 + 4*x3 -2*x4 = -12
// x1 - 7*x3 + 5*x4 = -6
// Solution:
// x1 = 3, x2 = -2, x3 = 2, x4 = 1
int main(int argc, char* argv[]) {
  const int n = 4;
  float **A = malloc(n * sizeof(float*));
  for (int row = 0; row < n; row++) {
    A[row] = malloc(n * sizeof(float));
  }
  float *b = malloc(n * sizeof(float));
  float *phi = malloc(n * sizeof(float));
  
  A[0][0] = 4;
  A[0][1] = -1;
  A[0][2] = -6;
  A[0][3] = 0;

  A[1][0] = -5;
  A[1][1] = -4;
  A[1][2] = 10;
  A[1][3] = 8;

  A[2][0] = 0;
  A[2][1] = 9;
  A[2][2] = 4;
  A[2][3] = -2;

  A[3][0] = 1;
  A[3][1] = 0;
  A[3][2] = -7;
  A[3][3] = 5;

  b[0] = 2;
  b[1] = 21;
  b[2] = -12;
  b[3] = -6;

  phi[0] = 0;
  phi[1] = 0;
  phi[2] = 0;
  phi[3] = 0;

  const float omega = 0.5;
  sor(A, b, omega, phi, n);

  printf("Result: ");
  for (int i = 0; i < n; i++) {
    printf("%f ", phi[i]);
  }
  printf("\n");

  for (int i = 0; i < n; i++) {
    free(A[i]);
  }
  free(A);
  free(b);
  free(phi);
  return 0;
}
