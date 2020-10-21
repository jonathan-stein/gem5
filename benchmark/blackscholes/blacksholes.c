/* Benchmark Application in C.
 * The error injection function is blacsholes.
 * The error injection function contains library calls.
 * Compile it with: clang blacksholes.c -lm
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

// The main function: blacksholes.
// Calculate the price of a call option based on the parameters.
double blacksholes(const double stockPrice,
    const double exercisePrice,
    const double riskFreeInterest,
    const double timeToExpiration,
    const double volatility) {
  const double d1 = (log(stockPrice / exercisePrice) +
      (riskFreeInterest + volatility * volatility / 2) * timeToExpiration) /
      (volatility * sqrt(timeToExpiration));
  const double d2 = (log(stockPrice / exercisePrice) +
      (riskFreeInterest - volatility * volatility / 2) * timeToExpiration) /
      (volatility * sqrt(timeToExpiration));
  const double callPrice = stockPrice * CNDF(d1) -
      exercisePrice * exp(-riskFreeInterest * timeToExpiration) * CNDF(d2);
  return callPrice;
}


int main(int argc, char *argv[]) {
  const double stockPrice = 150.0;
  const double exercisePrice = 160.0;
  const double riskFreeInterest = 0.02;
  const double timeToExpiration = 10.0;
  const double volatility = 0.05;

  // The result call price should be 21.406580.
  const double callPrice = blacksholes(stockPrice, exercisePrice, riskFreeInterest,
      timeToExpiration, volatility);
  printf("Result: %f\n", callPrice);
  return 0;
}