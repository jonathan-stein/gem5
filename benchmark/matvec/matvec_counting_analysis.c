// The output is b.
// R(b) = R(A, x) * Ralu^37 * Rfpu^24
void matvec(float **A, float *x, float *b) {
  // The row loop has:
  // ALU op: 1 + (7 + 2) * 4 = 37
  // FPU op: 6 * 4 = 24
  for (int row = 0; row < 4; row += 1) {
    // The computation of sum has:
    // ALU op: 1 + 2 * 3 = 7
    // FPU op: 2 * 3 = 6
    float sum = 0;
    for (int col = 0; col < 3; col += 1) {
      sum += A[row][col] * x[col];
    }
    b[row] = sum;
  }
}