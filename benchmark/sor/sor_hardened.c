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


void matvec(float **A, float *x, float *b, const int m, const int n) {
  for (int row = 0; row < m; row += 1) {
    float sum = 0;

    for (int col = 0; col < n; col += 1) {
      //-- Hardened: prod = A[row][col] * x[col];
      float prod, prodH;
      do {
        prod = A[row][col] * x[col];
        prodH = A[row][col] * x[col];
      } while (prod != prodH);

      //-- Hardened: add = sum + prod
      float add, addH;
      do {
        add = sum + prod;
        addH = sum + prod;
      } while (add != addH);

      sum = add;
    }

    b[row] = sum;
  }
}

void vecsub(float *a, float *b, float *result, const int n) {
  for (int i = 0; i < n; i++) {
    //-- Hardened: result[i] = a[i] - b[i]
    float sub, subH;
    do {
      sub = a[i] - b[i];
      subH = a[i] - b[i];
    } while (sub != subH);

    result[i] = subH;
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

  for (int k = 0; k < 40; k += 1) {
    for (int i = 0; i < n; i++) {
      float sigma = 0;
      for (int j = 0; j < n; j++) {
        if (j != i) {
          //-- Hardened: sigma += A[i][j] * phi[j];
          float prod, prodH;
          do {
            prod = A[i][j] * phi[j];
            prodH = A[i][j] * phi[j];
          } while (prod != prodH);

          float add, addH;
          do {
            add = sigma + prod;
            addH = sigma + prod;
          } while (add != addH);

          sigma = add;
        }
      }

      //-- Hardened: phi[i] = (1 - omega) * phi[i] + (omega / A[i][i]) * (b[i] - sigma);
      float sub1, sub1H;
      do {
        sub1 = 1 - omega;
        sub1H = 1 - omega;
      } while (sub1 != sub1H);

      float sub2, sub2H;
      do {
        sub2 = b[i] - sigma;
        sub2H = b[i] - sigma;
      } while (sub2 != sub2H);

      float div, divH;
      do {
        div = omega / A[i][i];
        divH = omega / A[i][i];
      } while (div != divH);

      float prod1, prod1H;
      do {
        prod1 = sub1 * phi[i];
        prod1H = sub1 * phi[i];
      } while (prod1 != prod1H);
      
      float prod2, prod2H;
      do {
        prod2 = div * sub2;
        prod2H = div * sub2;
      } while (prod2 != prod2H);
      
      float add, addH;
      do {
        add = prod1 + prod2;
        addH = prod1 + prod2;
      } while (add != addH);

      phi[i] = add;
    }

    matvec(A, phi, multResult, n, n);
    vecsub(multResult, b, subResult, n);
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

  float *b = malloc(n * sizeof(float));
  b[0] = 2;
  b[1] = 21;
  b[2] = -12;
  b[3] = -6;

  const float omega = 0.5;

  float *phi = malloc(n * sizeof(float));
  phi[0] = 0;
  phi[1] = 0;
  phi[2] = 0;
  phi[3] = 0;

  sor(A, b, omega, phi, n);

  printf("Result: ");
  for (int i = 0; i < n; i++) {
    printf("%f ", phi[i]);
  }
  printf("\n");
  free(phi);

  for (int i = 0; i < n; i++) {
    free(A[i]);
  }
  free(A);
  free(b);
  return 0;
}
