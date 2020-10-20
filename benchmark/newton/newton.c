/* Benchmark Application in C.
 * The error injection function is newton.
 *
 * Finding a root of a given function:
 * f(x) = x^2 - 2x + 1.
 * df(x) = 2x - 2
 *
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>


// The given function.
float f(const float x) {
  const float fx = x * x - 2.0f * x + 1.0f;
  return fx;
}

float df(const float x) {
  const float dfx = 2.0f * x - 2.0f;
  return dfx;
}

// The newton method.
static const float tolerance = 0.00001f;

float newton(const float xStart) {
  float xCurrent = xStart;
  float xPrevious = xStart + 2 * tolerance;

  float fx;
  float dfx;
  while ((xPrevious - xCurrent >= tolerance) ||
      (xCurrent - xPrevious >= tolerance)) {
    xPrevious = xCurrent;
    fx = f(xCurrent);
    dfx = df(xCurrent);
    xCurrent = xCurrent - fx / dfx;
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

