// We first analyze the helper function.
// We need the reliability of the return value. The inputs are read-only and won't be affected.

//-- Ralu^7 * Rfpu^6 * R(A, x)
float matvec_rely_ssa_compute_element(float **A, float *x, const int row) {
  float sum;
  //-- Ralu^7 * Rfpu^6 * R(A, x)
  const float sum0Before = 0;

  // x[0]
  //-- Ralu^7 * Rfpu^6 * R(sum0Before, A, x)
  const int col0 = 0;
  //-- Ralu^7 * Rfpu^6 * R(sum0Before, A, x, col0)
  const int pred0 = col0 < 3;
  float sum0T;

  //-- Ralu^6 * Rfpu^6 * R(sum0Before, A, x, col0, pred0) & R(pred0)
  if (pred0) {
    //-- Ralu^6 * Rfpu^6 * R(sum0Before, A, x, col0, pred0)
    const float prod0 = A[row][col0] * x[col0];
    //-- Ralu^6 * Rfpu^5 * R(sum0Before, prod0, A, x, col0, pred0)
    const float sum1Before = sum0Before + prod0;

    // x[1]
    //-- Ralu^6 * Rfpu^4 * R(sum1Before, A, x, col0, pred0)
    const int col1 = col0 + 1;
    //-- Ralu^5 * Rfpu^4 * R(sum1Before, A, x, col1, pred0)
    const int pred1 = col1 < 3;
    float sum1T;

    //-- Ralu^4 * Rfpu^4 * R(sum1Before, A, x, col1, pred1, pred0) & R(pred1, pred0)
    if (pred1) {
      //-- Ralu^4 * Rfpu^4 * R(sum1Before, A, x, col1, pred1, pred0)
      const float prod1 = A[row][col1] * x[col1];
      //-- Ralu^4 * Rfpu^3 * R(sum1Before, prod1, A, x, col1, pred1, pred0)
      const float sum2Before = sum1Before + prod1;

      // x[2]
      //-- Ralu^4 * Rfpu^2 * R(sum2Before, A, x, col1, pred1, pred0)
      const int col2 = col1 + 1;
      //-- Ralu^3 * Rfpu^2 * R(sum2Before, A, x, col2, pred1, pred0)
      const int pred2 = col2 < 3;
      float sum2T;

      //-- Ralu^2 * Rfpu^2 * R(sum2Before, A, x, col2, pred2, pred1, pred0) & R(pred2, pred1, pred0)
      if (pred2) {
        //-- Ralu^2 * Rfpu^2 * R(sum2Before, A, x, col2, pred2, pred1, pred0)
        const float prod2 = A[row][col2] * x[col2];
        //-- Ralu^2 * Rfpu * R(sum2Before, prod2, col2, pred2, pred1, pred0)
        const float sum3Before = sum2Before + prod2;

        // End of loop.
        //-- Ralu^2 * R(sum3Before, col2, pred2, pred1, pred0)
        const int col3 = col2 + 1;
        //-- Ralu * R(sum3Before, col3, pred2, pred1, pred0)
        const int pred3 = col3 < 3;
        //-- R(sum3Before, pred3, pred2, pred1, pred0)
        if (pred3) {
          //-- R(sum3Before, pred3, pred2, pred1, pred0)
        } else {
          /* Crash */
          //-- R(sum3Before, pred3, pred2, pred1, pred0)
        } 

        //-- R(sum3Before, pred3, pred2, pred1, pred0)
        sum2T = sum3Before;
        //-- R(sum2T, pred2, pred1, pred0)
      } else {
        //-- R(pred2, pred1, pred0)
        skip;
        //-- R(sum2T, pred2, pred1, pred0)
      }
      //-- R(sum2T, pred2, pred1, pred0)
      sum1T = sum2T; // phi(sum2T, sum2Before, pred2).
      //-- R(sum1T, pred1, pred0)
    } else {
      //-- R(pred1, pred0)
      skip;
      //-- R(sum1T, pred1, pred0)
    }
    //-- R(sum1T, pred1, pred0)
    sum0T = sum1T; // phi(sum1T, sum1Before, pred1).
    //-- R(sum0T, pred0)
  } else {
    //-- R(pred0)
    skip;
    //-- R(sum0T, pred0)
  }
  //-- R(sum0T, pred0)
  sum = sum0T; // phi(sum0T, sum0Before, pred0).

  //-- R(sum)
  return sum;
  //-- 1
}


// We focuse on the reliability of b.

//-- Ralu^37 * Rfpu^24 * R(A, x)
void matvec_rely_ssa(float **A, float *x, float *b) {
  // b[0]
  //-- Ralu^37 * Rfpu^24 * R(A, x)
  const int row0 = 0;
  //-- Ralu^37 * Rfpu^24 * R(row0, A, x)
  const int pred0 = row0 < 4;

  //-- Ralu^36 * Rfpu^24 * R(pred0, row0, A, x) & R(pred0)
  if (pred0) {
    //-- Ralu^36 * Rfpu^24 * R(pred0, row0, A, x)
    const float b0 = matvec_rely_ssa_compute_element(A, x, row0);
    //-- Ralu^29 * Rfpu^18 * R(pred0, row0, b0, A, x)
    b[row0] = b0;

    // b[1]
    //-- Ralu^29 * Rfpu^18 * R(pred0, row0, A, x)
    const int row1 = row0 + 1;
    //-- Ralu^28 * Rfpu^18 * R(pred0, row1, A, x)
    const int pred1 = row1 < 4;

    //-- Ralu^27 * Rfpu^18 * R(pred0, pred1, row1, A, x) & R(pred0, pred1)
    if (pred1) {
      //-- Ralu^27 * Rfpu^18 * R(pred0, pred1, row1, A, x)
      const float b1 = matvec_rely_ssa_compute_element(A, x, row1);
      //-- Ralu^20 * Rfpu^12 * R(pred0, pred1, row1, b1, A, x)
      b[row1] = b1;

      // b[2]
      //-- Ralu^20 * Rfpu^12 * R(pred0, pred1, row1, A, x)
      const int row2 = row1 + 1;
      //-- Ralu^19 * Rfpu^12 * R(pred0, pred1, row2, A, x)
      const int pred2 = row2 < 4;

      //-- Ralu^18 * Rfpu^12 * R(pred0, pred1, pred2, row2, A, x) & R(pred0, pred1, pred2)
      if (pred2) {
        //-- Ralu^18 * Rfpu^12 * R(pred0, pred1, pred2, row2, A, x)
        const float b2 = matvec_rely_ssa_compute_element(A, x, row2);
        //-- Ralu^11 * Rfpu^6 * R(pred0, pred1, pred2, row2, b2, A, x)
        b[row2] = b2;

        // b[3]
        //-- Ralu^11 * Rfpu^6 * R(pred0, pred1, pred2, row2, A, x)
        const int row3 = row2 + 1;
        //-- Ralu^10 * Rfpu^6 * R(pred0, pred1, pred2, row3, A, x)
        const int pred3 = row3 < 4;

        //-- Ralu^9 * Rfpu^6 * R(pred0, pred1, pred2, pred3, row3, A, x) & R(pred0, pred1, pred2, pred3)
        if (pred3) {
          //-- Ralu^9 * Rfpu^6 * R(pred0, pred1, pred2, pred3, row3, A, x)
          const float b3 = matvec_rely_ssa_compute_element(A, x, row3);
          //-- Ralu^2 * R(pred0, pred1, pred2, pred3, row3, b3)
          b[row3] = b3;

          // End of the loop.
          //-- Ralu^2 * R(pred0, pred1, pred2, pred3, row3)
          const int row4 = row3 + 1;
          //-- Ralu * R(pred0, pred1, pred2, pred3, row4)
          const int pred4 = row4 < 4;
          //-- R(pred0, pred1, pred2, pred3, pred4)
          if (pred4) {} else { exit(1) /* Crash */; }
          //-- R(pred0, pred1, pred2, pred3, pred4)
        } else {
          //-- R(pred0, pred1, pred2, pred3)
          skip;
          //-- R(pred0, pred1, pred2, pred3)
        }
        //-- R(pred0, pred1, pred2, pred3)
      } else {
        //-- R(pred0, pred1, pred2)
        skip;
        //-- R(pred0, pred1, pred2)
      }
      //-- R(pred0, pred1, pred2)
    } else {
      //-- R(pred0, pred1)
      skip;
      //-- R(pred0, pred1)
    }
    //-- R(pred0, pred1)
  } else {
    //-- R(pred0)
    skip;
    //-- R(pred0)
  }
  //-- R(pred0)
}