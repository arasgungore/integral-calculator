#ifndef _INTEGRAL_H_
#define _INTEGRAL_H_

#include "calculator.h"

typedef struct {
	long double lowerBound, upperBound, dx;
	Expression expr;
} Integral;

long double riemannSum(Integral *ig, const unsigned int n, const char variable, long double xi);
long double lowerRiemannSum(Integral *ig, const unsigned int n, const char variable);
long double upperRiemannSum(Integral *ig, const unsigned int n, const char variable);
long double midpointRule(Integral *ig, const unsigned int n, const char variable);
long double trapezoidalRule(Integral *ig, const unsigned int n, const char variable);
long double trapezoidalOpenRule(Integral *ig, const unsigned int n, const char variable);
long double simpsonsRule(Integral *ig, const unsigned int n, const char variable);
long double simpsonsThreeEighthsRule(Integral *ig, const unsigned int n, const char variable);
long double simpsonsExtendedRule(Integral *ig, const unsigned int n, const char variable);
long double boolesRule(Integral *ig, const unsigned int n, const char variable);
long double milnesRule(Integral *ig, const unsigned int n, const char variable);
long double weddlesRule(Integral *ig, const unsigned int n, const char variable);
long double sixPointClosedRule(Integral *ig, const unsigned int n, const char variable);


// calculates the definite integral according to Riemann sum
long double riemannSum(Integral *ig, const unsigned int n, const char variable, long double xi) {
	long double sum = 0.0;
	unsigned int i;		//dx=(b-a)/n, xi=a+i*dx ==>  integral = f(x0)*dx + f(x1)*dx + f(x2)*dx + ... + f(x(n-1))*dx = (f(x0) + f(x1) + f(x2) + ... + f(x(n-1)))*dx = sigma(f(xi))*dx
	for(i=0;i<n;i++)	sum += parse(&ig->expr, variable, xi += ig->dx);
	return ig->dx * sum;
}

// calculates the definite integral according to lower Riemann sum
long double lowerRiemannSum(Integral *ig, const unsigned int n, const char variable) {
	return riemannSum(ig, n, variable, ig->lowerBound - ig->dx);
}

// calculates the definite integral according to upper Riemann sum
long double upperRiemannSum(Integral *ig, const unsigned int n, const char variable) {
	return riemannSum(ig, n, variable, ig->lowerBound);
}

// calculates the definite integral according to midpoint Riemann sum
long double midpointRule(Integral *ig, const unsigned int n, const char variable) {
	return riemannSum(ig, n, variable, ig->lowerBound - ig->dx/2.0);
}

// calculates the definite integral according to trapezoidal 2-point rule of closed type
long double trapezoidalRule(Integral *ig, const unsigned int n, const char variable) {
	long double sum = 0.0, xi = ig->lowerBound;
	unsigned int i;
	for(i=1;i<n;i++)	sum += parse(&ig->expr, variable, xi += ig->dx);
	return ig->dx * ((parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound)) / 2.0 + sum);
}

// calculates the definite integral according to trapezoid method of open type
long double trapezoidalOpenRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%3) {
		puts("\nInvalid partition for Open Trapezoidal Rule.\nn (number of intervals) must be a multiple of 3.");
		exit(1);
	}
	long double sum = 0.0, xi = ig->lowerBound;
	unsigned int i;
	for(i=1;i<n;i+=3) {
		sum += parse(&ig->expr, variable, xi += ig->dx);
		sum += parse(&ig->expr, variable, xi += ig->dx);
		xi += ig->dx;
	}
	return 1.50 * ig->dx * sum;
}

// calculates the definite integral according to Simpson's 1/3 3-point rule
long double simpsonsRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%2) {
		puts("\nInvalid partition for Simpson's (1/3) Rule.\nn (number of intervals) must be even.");
		exit(1);
	}
	long double sum1 = 0.0, sum2 = 0.0, xi = ig->lowerBound;
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	unsigned int i;
	for(i=2;i<n;i+=2) {
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
	}
	return ig->dx * (parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound) + 4.0*sum1 + 2.0*sum2) / 3.0;
}

// calculates the definite integral according to Simpson's 3/8 4-point rule
long double simpsonsThreeEighthsRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%3) {
		puts("\nInvalid partition for Simpson's 3/8 Rule.\nn (number of intervals) must be a multiple of 3.");
		exit(1);
	}
	long double sum1 = 0.0, sum2 = 0.0, xi = ig->lowerBound;
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	unsigned int i;
	for(i=3;i<n;i+=3) {
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
	}
	return 0.3750 * ig->dx * (parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound) + 3.0*sum1 + 2.0*sum2);
}

// calculates the definite integral according to alternative extended Simpson's rule, which is obtained by taking the mean of the original composite Simpson's rule and 3/8 Simpson's rule
long double simpsonsExtendedRule(Integral *ig, const unsigned int n, const char variable) {
	if(n<8) {
		puts("\nInvalid partition for Alternative Extended Simpson's Rule.\nn (number of intervals) must be at least 8.");
		exit(1);
	}
	long double sum = 0.0, xi = ig->lowerBound + 3.0*ig->dx;
	unsigned int i;
	for(i=7;i<n;i++)	sum += parse(&ig->expr, variable, xi += ig->dx);
	return ig->dx * ((17.0*(parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound))
					+ 59.0*(parse(&ig->expr, variable, ig->lowerBound + ig->dx) + parse(&ig->expr, variable, ig->upperBound - ig->dx))
					+ 43.0*(parse(&ig->expr, variable, ig->lowerBound + 2.0*ig->dx) + parse(&ig->expr, variable, ig->upperBound - 2.0*ig->dx))
					+ 49.0*(parse(&ig->expr, variable, ig->lowerBound + 3.0*ig->dx) + parse(&ig->expr, variable, ig->upperBound - 3.0*ig->dx))) / 48.0 + sum);
}

// calculates the definite integral according to Boole's 5-point rule
long double boolesRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%4) {
		puts("\nInvalid partition for Boole's Rule.\nn (number of intervals) must be a multiple of 4.");
		exit(1);
	}
	long double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, xi = ig->lowerBound;
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	sum2 += parse(&ig->expr, variable, xi += ig->dx);
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	unsigned int i;
	for(i=4;i<n;i+=4) {
		sum3 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
	}
	return 2.0 * ig->dx * (7.0*(parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound)) + 32.0*sum1 + 12.0*sum2 + 14.0*sum3) / 45.0;
}

// calculates the definite integral according to the Milne's rule
long double milnesRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%4) {
		puts("\nInvalid partition for Milne's Rule.\nn (number of intervals) must be a multiple of 4.");
		exit(1);
	}
	long double sum1 = 0.0, sum2 = 0.0, xi = ig->lowerBound;
	unsigned int i;
	for(i=1;i<n;i+=4) {
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		xi += ig->dx;
	}
	return 4.0 * ig->dx * (2.0*sum1 - sum2) / 3.0;
}

// calculates the definite integral according to the Weddle's rule
long double weddlesRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%6) {
		puts("\nInvalid partition for Weddle's Rule.\nn (number of intervals) must be a multiple of 6.");
		exit(1);
	}
	long double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, sum4 = 0.0, xi = ig->lowerBound;
	sum2 += parse(&ig->expr, variable, xi += ig->dx);
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	sum3 += parse(&ig->expr, variable, xi += ig->dx);
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	sum2 += parse(&ig->expr, variable, xi += ig->dx);
	unsigned int i;
	for(i=6;i<n;i+=6) {
		sum4 += parse(&ig->expr, variable, xi += ig->dx);
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		sum3 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
	}
	return 0.30 * ig->dx * (parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound) + sum1 + 5.0*sum2 + 6.0*sum3 + 2.0*sum4);
}

// calculates the definite integral according to the six point rule of closed type
long double sixPointClosedRule(Integral *ig, const unsigned int n, const char variable) {
	if(n%5) {
		puts("\nInvalid partition for 6-Point Closed Rule.\nn (number of intervals) must be a multiple of 5.");
		exit(1);
	}
	long double sum1 = 0.0, sum2 = 0.0, sum3 = 0.0, xi = ig->lowerBound;
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	sum2 += parse(&ig->expr, variable, xi += ig->dx);
	sum2 += parse(&ig->expr, variable, xi += ig->dx);
	sum1 += parse(&ig->expr, variable, xi += ig->dx);
	unsigned int i;
	for(i=5;i<n;i+=5) {
		sum3 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum2 += parse(&ig->expr, variable, xi += ig->dx);
		sum1 += parse(&ig->expr, variable, xi += ig->dx);
	}
	return 5.0 * ig->dx * (19.0*(parse(&ig->expr, variable, ig->lowerBound) + parse(&ig->expr, variable, ig->upperBound)) + 75.0*sum1 + 50.0*sum2 + 38.0*sum3) / 288.0;
}

#endif
