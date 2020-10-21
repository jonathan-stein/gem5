/* Benchmark Application in C.
 * The error injection function is blacsholes.
 * 
 * The C function to compute the cumulative normal distribution is adopted from
 * https://stackoverflow.com/questions/2328258/cumulative-normal-distribution-function-in-c-c
 * 
 * The blacksholes equation is from:
 * https://www.investopedia.com/terms/b/blackscholes.asp.
 * 
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Cumulative normal distribution function.
double CNDF(double x) {
  const double a1 =  0.254829592;
  const double a2 = -0.284496736;
  const double a3 =  1.421413741;
  const double a4 = -1.453152027;
  const double a5 =  1.061405429;
  const double p  =  0.3275911;

  // Save the sign of x
  int sign = 1;
  if (x < 0) {
    sign = -1;
  }
  x = fabs(x) / sqrt(2.0);

  // A&S formula 7.1.26
  double t = 1.0 / (1.0 + p*x);
  double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);
  return 0.5 * (1.0 + sign*y);
}


// The main function.
int main(int argc, char *argv[]) {
  if (argc != 6) {
    fprintf(stderr, "Usage: ./blacksholes stockPrice exercisePrice riskFreeInterest timeToExpire volatility\n");
    return -1;
  }

  const float stockPrice = strtof(argv[1], NULL);
  const float exercisePrice = strtof(argv[2], NULL);
  const float riskFreeInterest = strtof(argv[3], NULL);

  return 0;
}