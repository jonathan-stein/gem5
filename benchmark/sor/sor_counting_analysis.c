/*
float sqrtImpl(const float x) {
  float z = 1.0f;
  for (int i = 0; i < 30; i += 1) {
    z -= (z*z - x) / (2*z);
  }
  return z;
}
*/

//-- R(x) * Rfpu^150 * Ralu^61
float sqrtImpl(const float x) {
  const float z0Before = 1.0f;
  const int i0 = 0;
  //-- R(z0Before, i0, x) * Rfpu^150 * Ralu^61
  const int pred0 = i0 < 30;
  //-- R(z0Before, pred0, i0, x) * Rfpu^5 * Ralu^2 * loopFactor^29
  // so the loop factor is 5 FPU operations and 2 ALU operations.
  //-- R(z0Before, pred0, i0, x) * Rfpu^150 * Ralu^60
  if (pred0) {
    //-- R(z0Before, pred0, i0,x ) * Rfpu^5 * Ralu^2 * loopFactor^29
    const float prod0 = z0Before * z0Before;
    const float sub0 = prod0 - x;
    const float denom0 = 2 * z0Before;
    const float right0 = sub0 / denom0;

    //-- R(z0Before, right0, pred0, i0, x) * Rfpu * Ralu^2 * loopFactor^29
    const float z1Before = z0Before - right0;
    //-- R(z0Before, z1Before, pred0, i0, x) * Ralu^2 * loopFactor^29
    const int i1 = i0 + 1;
    //-- R(z0Before, z1Before, pred0, i1, x) * Ralu * loopFactor^29 
    const int pred1 = i1 < 30;
    //-- R(z0Before, z1Before, pred0, pred1, i1, x) * loopFactor^29 
    if (pred1) {
      // Same stuff for 29 iterations.
    }
    //-- R(z0Before, z1Before, z2Phi, pred0, pred1)
    const float z1Phi = z2Phi; // phi(z1Before, z2Phi, pred1)
    //-- R(z0Before, z1Phi, pred0)
  } else {
    //-- R(z0Before, pred0)
    skip;
    //-- R(z0Before, z1Phi, pred0)
  }
  //-- R(z0Before, z1Phi, pred0)
  const float z0Phi = z1Phi; // phi(z0Before, z1Phi, pred0)
  
  //-- R(z0Phi)
  return z0Phi;
  //-- 1
}


/*
float norm(float *a, const int n) {
  float norm = 0;
  for (int i = 0; i < n; i++) {
    norm += (a[i] * a[i]);
  }
  norm = sqrtImpl(norm);
  return norm;
}
*/

//-- R(a) * Rfpu^158 * Ralu^78
float norm(float *a) {
  //-- R(a) * Rfpu^150 * Ralu^61 * Ralu^17 * Rfpu^8
  const float norm0Before = 0.0f;
  const int i0 = 0;
  const int pred0 = i0 < 4;
  //-- R(norm0Before, pred0, i0, a) * Rfpu^150 * Ralu^61 * Ralu^4 * Rfpu^2 * loopFactor^3
  // So the loop factor is Ralu^4 * Rfpu^2
  //-- R(norm0Before, pred0, i0, a) * Rfpu^150 * Ralu^61 * Ralu^16 * Rfpu^8
  if (pred0) {
    //-- R(norm0Before, pred0, i0, a) * Rfpu^150 * Ralu^61 * Ralu^4 * Rfpu^2 * loopFactor^3
    const int offset0 = i0 * sizeof(float);
    const int addr0 = a + offset0;
    const float a0 = *addr0;
    const float prod0 = a0 * a0;
    const float norm1Before = norm0Before + prod0;

    const int i1 = i0 + 1;
    const int pred1 = i1 < 4;
    //-- R(norm0Before, norm1Before, pred0, pred1, i1, a) * Rfpu^150 * Ralu^61 * loopFactor^3
    if (pred1) {
      // Same stuff for 3 iterations.
    }
    //-- R(norm0Before, norm1Before, norm2Phi, pred0, pred1) * Rfpu^150 * Ralu^61
    const float norm1Phi = norm2Phi; // phi(norm1Before, norm2Phi, pred1)
    //-- R(norm0Before, norm1Phi, pred0) * Rfpu^150 * Ralu^61
  }
  //-- R(norm0Before, norm1Phi, pred0) * Rfpu^150 * Ralu^61
  const float norm0Phi = norm1Phi; // phi(norm0Before, norm1Phi, pred0)
  //-- R(norm0Phi) * Rfpu^150 * Ralu^61
  const float normSqrt = sqrtImpl(norm0Phi);
  //-- R(normSqrt)
  return normSqrt;
  //-- R(1)
}


// For matvec, we can reuse the previous analysis result. The output is b.
//-- R(b) = R(A, x) * Rfpu^32 * Ralu^45
// ALU op: 45
// FPU op: 32
void matvec(float **A, float *x, float *b) {
  // The row loop has:
  // ALU op: 1 + (9 + 2) * 4 = 45
  // FPU op: 8 * 4 = 32
  for (int row = 0; row < 4; row += 1) {
    // This inner loop has:
    // ALU op: 9
    // FPU op: 8
    float sum = 0;
    for (int col = 0; col < 4; col += 1) {
      sum += A[row][col] * x[col];
    }
    b[row] = sum;
  }
}

//-- R(result) = R(a, b) * Rfpu^4 * Ralu^9
// ALU op: 9
// FPU op: 4
void vecsub(float *a, float *b, float *result) {
  for (int i = 0; i < 4; i++) {
    result[i] = a[i] - b[i];
  }
}


/*
void sor(float **A, float *b, float omega, float* phi) {
  float multResult[4];
  float subResult[4];

  matvec(A, phi, multResult);
  vecsub(multResult, b, subResult);

  for (int i = 0; i < 40; i += 1) {
    for (int i = 0; i < 4; i++) {
      float sigma = 0;
      for (int j = 0; j < 4; j++) {
        if (j != i) {
          sigma += A[i][j] * phi[j];
        }
      }
      phi[i] = (1 - omega) * phi[i] + (omega / A[i][i]) * (b[i] - sigma);
    }

    matvec(A, phi, multResult);
    vecsub(multResult, b, subResult);
  }
}
*/

// The result is stored in phi.
// ALU op: 54 + 4681 = 4735
// FPU op: 36 + 3680 = 3716
void sor(float **A, float *b, float omega, float* phi) {
  float multResult[4];
  float subResult[4];

  // matvec + vecsub has:
  // ALU op: 45 + 9 = 54
  // FPU op: 32 + 4 = 36
  matvec(A, phi, multResult);
  vecsub(multResult, b, subResult);

  // The k loop has:
  // ALU op: 1 + (61 + 54 + 2) * 40 = 4681
  // FPU op: (56 + 36) * 40 = 3680
  for (int k = 0; k < 40; k += 1) {
  
    // This i loop has:
    // ALU op: 1 + (13 + 2) * 4 = 61
    // FPU op: (8 + 6) * 4 = 56
    for (int i = 0; i < 4; i++) {
      // This inner loop has:
      // ALU op: 1 + (1 + 2) * 4 = 13
      // FPU op: 2 * 4 = 8
      float sigma = 0;
      for (int j = 0; j < 4; j++) {
        if (j != i) {
          sigma += A[i][j] * phi[j];
        }
      }
      // This line has:
      // ALU op: 0
      // FPU op: 6
      phi[i] = (1 - omega) * phi[i] + (omega / A[i][i]) * (b[i] - sigma);
    }

    // matvec + vecsub has:
    // ALU op: 45 + 9 = 54
    // FPU op: 32 + 4 = 36
    matvec(A, phi, multResult);
    vecsub(multResult, b, subResult);
  }
}
