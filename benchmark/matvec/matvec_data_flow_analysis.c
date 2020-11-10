// We care about the reliability of b and won't modify A and x.
// A(x): user-specified reliability for x.
// R(x): reliability at that point of program execution.

// Rfpu^24 * R(A, x)
void matvec_unrolled(float **A, float *x, float *b) {
  //-- r^24 * R(A, x)
  const float sum00 = 0;
  const float product00 = A[0][0] * x[0];
  const float sum01 = sum00 + product00;
  const float product01 = A[0][1] * x[1];
  const float sum02 = sum01 + product01;
  const float product02 = A[0][2] * x[2];
  const float sum03 = sum02 + product02;
  b[0] = sum03;

  //-- r^18 * R(A, x)
  const float sum10 = 0;
  const float product10 = A[1][0] * x[0];
  const float sum11 = sum10 + product10;
  const float product11 = A[1][1] * x[1];
  const float sum12 = sum11 + product11;
  const float product12 = A[1][2] * x[2];
  const float sum13 = sum12 + product12;
  b[1] = sum13;

  //-- r^12 * R(A, x)
  const float sum20 = 0;
  const float product20 = A[2][0] * x[0];
  const float sum21 = sum20 + product20;
  const float product21 = A[2][1] * x[1];
  const float sum22 = sum21 + product21;
  const float product22 = A[2][2] * x[2];
  const float sum23 = sum22 + product22;
  b[2] = sum23;

  //-- r^6 * R(A, x)
  const float sum30 = 0;
  //-- r^6 * R(sum30, A, x)
  const float product30 = A[3][0] * x[0];
  //-- r^5 * R(sum30, product30, A, x)
  const float sum31 = sum30 + product30;
  //-- r^4 * R(sum31, A, x)
  const float product31 = A[3][1] * x[1];
  //-- r^3 * R(sum31, product31, A, x)
  const float sum32 = sum31 + product31;
  //-- r^2 * R(sum32, A, x)
  const float product32 = A[3][2] * x[2];
  //-- r * R(sum32, product32)
  const float sum33 = sum32 + product32;
  //-- R(sum33)
  b[3] = sum33;
  //-- 1
}