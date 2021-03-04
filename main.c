#include "Header/integral.h"
#include <conio.h>

#define N_MAX pow(4, 10)
#define DX_MAX powl(4, -10)
#define DX_MIN powl(4, -24)
typedef enum {
	CALCULATOR_MODE='1', INTEGRAL_MODE, DERIVATIVE_MODE
} Mode;
int main() {
	printf( "Trigonometric functions:\n\tsin\n\tcos\n\ttan\n\tcot\n\tsec\n\tcsc/cosec"
			"\n\nHyperbolic trigonometric functions:\n\tsinh\n\tcosh\n\ttanh\n\tcoth\n\tsech\n\tcsch"
			"\n\nInverse trigonometric functions:\n\tasin/arcsin\n\tacos/arccos\n\tatan/arctan\n\tacot/arccot\n\tasec/arcsec\n\tacsc/arccsc"
			"\n\nExponential and logarithmic functions:\n\tsqr\n\tcube\n\tsqrt\n\tcbrt\n\texp\n\tlog\n\tln"
			"\n\nMiscellaneous functions:\n\tabs\n\tceil\n\tfloor\n\tround\n\tfact"
			"\n\nOperations:\n\t+\n\t-\n\t*\n\t/\n\t%%\n\t^\n\t!");
	while(TRUE) {
		printf( "\n\nPress 1 to calculate expressions."
				"\nPress 2 to calculate the definite integral of functions using numerical integration."
				"\nPress 3 to calculate the derivate of functions using limit definition."
				"\nPress any other key to quit...");
		switch(getch()) {
			case CALCULATOR_MODE: {
				printf("\n\nPress D to enable degrees, press any other key to enable radians.");
				const char key = getch();
				while(TRUE) {
					Expression *expr = (Expression[]){{"", '\0', 0, key=='d' || key=='D'}};
					printf("\n\nEvaluate the following algebraic expression: ");
					gets(expr->expression);
					if(!(strncmp(expr->expression, "exit", 4) && strncmp(expr->expression, "quit", 4)))
						break;
					expr->currentChar = expr->expression[0];
					__mingw_printf("Answer: %.16Lg\n", parse(expr, ' ', 0.0));
				}
				continue; }
			case INTEGRAL_MODE: {
				printf("\n\nPress D to enable degrees, press any other key to enable radians.");
				const char key = getch();
				while(TRUE) {
					Integral *ig = (Integral[]){{0.0, 0.0, 0.0, {"", '\0', 0, key=='d' || key=='D'}}};
					printf("\n\nVariable of integration: ");
					char variable;
					scanf("%c%*c", &variable);
					printf("Integrate the following function: ");
					gets(ig->expr.expression);
					if(!(strncmp(ig->expr.expression, "exit", 4) && strncmp(ig->expr.expression, "quit", 4)))
						break;
					printf("Enter the lower bound (from) and upper bound (to) respectively: ");
					Expression *lb_value = (Expression*) malloc(sizeof(Expression)), *ub_value = (Expression*) malloc(sizeof(Expression));
					lb_value->currentPos = ub_value->currentPos = 0, lb_value->anglesAreDegrees = ub_value->anglesAreDegrees = key=='d' || key=='D';
					scanf("%s%s%*c", lb_value->expression, ub_value->expression);
					ig->expr.currentChar = ig->expr.expression[0], lb_value->currentChar = lb_value->expression[0], ub_value->currentChar = ub_value->expression[0];
					ig->lowerBound = parse(lb_value, ' ', 0.0), ig->upperBound = parse(ub_value, ' ', 0.0);
					free(lb_value), free(ub_value);
					unsigned int n;
					for(n=1;n<=N_MAX;n*=4) {
						ig->dx = (ig->upperBound - ig->lowerBound) / n;
						printf("\nFor n=%u intervals:", n);
						__mingw_printf("\n\tLower Riemann Sum:\t\t\t%.16Lg", lowerRiemannSum(ig, n, variable));
						__mingw_printf("\n\tUpper Riemann Sum:\t\t\t%.16Lg", upperRiemannSum(ig, n, variable));
						__mingw_printf("\n\tMidpoint Rule:\t\t\t\t%.16Lg", midpointRule(ig, n, variable));
						__mingw_printf("\n\tTrapezoidal Rule:\t\t\t%.16Lg", trapezoidalRule(ig, n, variable));
						ig->dx = (ig->upperBound - ig->lowerBound) / (2*n);
						__mingw_printf("\n\tSimpson's (1/3) Rule:\t\t\t%.16Lg", simpsonsRule(ig, 2*n, variable));
						ig->dx = (ig->upperBound - ig->lowerBound) / (3*n);
						__mingw_printf("\n\tSimpson's 3/8 Rule:\t\t\t%.16Lg", simpsonsThreeEighthsRule(ig, 3*n, variable));
						ig->dx = (ig->upperBound - ig->lowerBound) / (4*n);
						__mingw_printf("\n\tBoole's Rule:\t\t\t\t%.16Lg", boolesRule(ig, 4*n, variable));
						__mingw_printf("\n\tMilne's Rule:\t\t\t\t%.16Lg", milnesRule(ig, 4*n, variable));
						ig->dx = (ig->upperBound - ig->lowerBound) / (5*n);
						__mingw_printf("\n\t6-Point Closed Rule:\t\t\t%.16Lg", sixPointClosedRule(ig, 5*n, variable));
						ig->dx = (ig->upperBound - ig->lowerBound) / (6*n);
						__mingw_printf("\n\tWeddle's Rule:\t\t\t\t%.16Lg", weddlesRule(ig, 6*n, variable));
						puts("");
					}
				}
				continue; }
			case DERIVATIVE_MODE: {
				printf("\n\nPress D to enable degrees, press any other key to enable radians.");
				const char key = getch();
				while(TRUE) {
					Expression *expr = (Expression[]){{"", '\0', 0, key=='d' || key=='D'}}, *x_value = (Expression[]){{"", '\0', 0, key=='d' || key=='D'}};
					printf("\n\nVariable of differentiation: ");
					char variable;
					scanf("%c%*c", &variable);
					printf("Differentiate the following function: ");
					gets(expr->expression);
					if(!(strncmp(expr->expression, "exit", 4) && strncmp(expr->expression, "quit", 4)))			break;
					printf("Enter the %c value: ", variable);
					gets(x_value->expression);
					expr->currentChar = expr->expression[0], x_value->currentChar = x_value->expression[0];
					long double value = parse(x_value, ' ', 0.0), dx;
					for(dx=DX_MAX;dx>=DX_MIN;dx/=4.0)
						__mingw_printf("Derivative at point %c=%Lg for d%c=%Lg: %.16Lg\n", variable, value, variable, dx, (parse(expr, variable, value+dx) - parse(expr, variable, value)) / dx);
				}
				continue; }
			default:
				exit(0);
		}
	}
}
