/* Benchmark Application in C.
 * The error injection function is newton.
 *
 * Finding a root of a given function using Newton's method:
 * f(x) = x^2 - 2x + 1.
 * df(x) = 2x - 2
 *
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>

// The given function.
float f(const float x) {
  float x_1, x_2;
  do {
    x_1 = x * x;
    x_2 = x * x;
  } while (x_1 != x_2);

  float x_3, x_4;
  do {
    x_3 = 2.0f * x;
    x_4 = 2.0f * x;
  } while (x_3 != x_4);

  float x_5, x_6;
  do {
    x_5 = x_1 - x_3;
    x_6 = x_1 - x_3;
  } while (x_5 != x_6);

  float x_7, x_8;
  do {
    x_7 = x_5 + 1.0f;
    x_8 = x_5 + 1.0f;
  } while (x_7 != x_8);

  //const float fx = x * x - 2.0f * x + 1.0f;
  return x_7;
}

float df(const float x) {
  float x_1, x_2;
  do {
    x_1 = 2.0f * x;
    x_2 = 2.0f * x;
  } while (x_1 != x_2);

  float x_3, x_4;
  do {
    x_3 = x_1 - 2.0f;
    x_4 = x_1 - 2.0f;
  } while (x_3 != x_4);
  //const float dfx = 2.0f * x - 2.0f;
  return x_3;
}

// The newton method.
static const float tolerance = 0.00001f;

float newton(const float xStart) {
  float xCurrent = xStart;
  float xCurrent_1 = xStart;

  float x_1, x_2;
  do {
    x_1 = 2 * tolerance;
    x_2 = 2 * tolerance;
  } while (x_1 != x_2);

  //float xPrevious = xStart + 2 * tolerance;
  float xPrevious, xPrevious_1;
  do {
    xPrevious = xStart + x_1;
    xPrevious_1 = xStart + x_1;
  } while (xPrevious != xPrevious_1);

  float fx;
  float dfx;
  //while ((xPrevious - xCurrent >= tolerance) ||
  //    (xCurrent - xPrevious >= tolerance)) {
  //  xPrevious = xCurrent;
  //  fx = f(xCurrent);
  //  dfx = df(xCurrent);
  //  xCurrent = xCurrent - fx / dfx;
  //}

  float sub_1, sub_2, sub_3, sub_4;
  int b_1, b_2, b_3, b_4;
  float div_1, div_2;
  while (1) {
    do {
      sub_1 = xPrevious - xCurrent;
      sub_2 = xPrevious - xCurrent;
    } while (sub_1 != sub_2);

    do {
      b_1 = sub_1 >= tolerance;
      b_2 = sub_1 >= tolerance;
    } while (b_1 != b_2);

    do {
      sub_3 = xCurrent - xPrevious;
      sub_4 = xCurrent - xPrevious;
    } while (sub_3 != sub_4);

    do {
      b_3 = sub_3 >= tolerance;
      b_4 = sub_3 >= tolerance;
    } while (b_3 != b_4);

    if (!b_1 && !b_3) {
      break;
    }

    xPrevious = xCurrent;
    fx = f(xCurrent);
    dfx = df(xCurrent);

    do {
      div_1 = fx / dfx;
      div_2 = fx / dfx;
    } while (div_1 != div_2);

    float xCurrentOld = xCurrent;

    do {
      xCurrent = xCurrentOld - div_1;
      xCurrent_1 = xCurrentOld - div_1;
    } while (xCurrent != xCurrent_1);
  }

  return xCurrent;
}


// Run newton's method.
int main() {
  float xStart = 0.0f;
  float answer = newton(xStart);
  printf("Result: %f\n", answer);
  return 0;
}

