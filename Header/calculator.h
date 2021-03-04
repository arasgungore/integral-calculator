#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <float.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_EXPRESSION_LENGTH 300

typedef struct {
	char expression[MAX_EXPRESSION_LENGTH], currentChar;
	unsigned int currentPos;
	unsigned short anglesAreDegrees;
} Expression;

long double factorial(const long double x);
void removeChar(char *str, const char charToRemove);
void putCharToString(char* str, const char charToPut, const unsigned int index);
char* getSubstring(char* str, const unsigned int beginPos, const unsigned int endPos);
void getNextChar(Expression *expr);
unsigned short eat(Expression *expr, const char charToEat);
long double parse(Expression *expr, const char variable, const long double value);
long double parseExpression(Expression *expr, const char variable, const long double value);
long double parseTerm(Expression *expr, const char variable, const long double value);
long double parseTerm2(Expression *expr, const char variable, const long double value);
long double parseTerm3(Expression *expr, const char variable, const long double value);
long double parseTerm4(Expression *expr, const char variable, const long double value);
long double parseFactor(Expression *expr, const char variable, const long double value);


// returns the factorial of x
long double factorial(const long double x) {
	if(x!=(long double)(long long)x || x<0.0) {
		__mingw_printf("\nInvalid factorial: \"%Lg!\"\nCan't take factorial of %s numbers.\n", x, x<0 ? "negative" : "non-integer");
		exit(1);
	}
	unsigned short i;
	long double res = 1.0;
	for(i=2; i<=x; res*=i, i++);
	return res;
}

// removes all the occurrences of "charToRemove" in the given string
void removeChar(char *str, const char charToRemove) {
	char *src, *dst;
	for(src=dst=str; *src; src++)
		if((*dst = *src) != charToRemove)
			dst++;
	*dst = '\0';
}

// inserts a character onto the specified index in the given string
void putCharToString(char* str, const char charToPut, const unsigned int index) {
	unsigned int i;
	for(i=strlen(str);i>=index;i--)
		str[i+1] = str[i];
	str[index] = charToPut;
}

// returns a substring of the given string
char* getSubstring(char* str, const unsigned int beginPos, const unsigned int endPos) {
	if(beginPos>=endPos) {
		puts("\nWrong parameters for substring function.");
		exit(1);
	}
	char* subStr = (char*) malloc((endPos-beginPos+1) * sizeof(char));
	memcpy(subStr, &str[beginPos], endPos-beginPos);
	subStr[endPos-beginPos] = '\0';
	return subStr;
}

// gets the next character given in expression
void getNextChar(Expression *expr) {
	expr->currentChar = expr->expression[++expr->currentPos];
}

// returns true if the current character is "charToEat" and progresses to the next character, false otherwise
unsigned short eat(Expression *expr, const char charToEat) {
	if(expr->currentChar == charToEat) {
		getNextChar(expr);
		return TRUE;
	}
	return FALSE;
}

long double parse(Expression *expr, const char variable, const long double value) {
	removeChar(expr->expression, ' ');
	unsigned int i;
	for(i=0;expr->expression[i];i++)
		if((expr->expression[i]>='0' && expr->expression[i]<='9') || expr->expression[i]==')')
			if((expr->expression[i+1]>='a' && expr->expression[i+1]<='z') || expr->expression[i+1]=='(')	//add a multiplication sign if not specified 
				putCharToString(expr->expression, '*', i+1);												//e.g., 2(5x+3) => 2*(5*x+3)
	const long double x = parseExpression(expr, variable, value);
	if(expr->currentPos < strlen(expr->expression)) {
		printf("\nUnexpected character: '%c'\n", expr->currentChar);
		exit(1);
	}
	expr->currentPos = 0;
	expr->currentChar = expr->expression[0];
	return x;
}

// parses the expression to terms according to addition and substraction
long double parseExpression(Expression *expr, const char variable, const long double value) {
	long double x = parseTerm(expr, variable, value);
	while(TRUE)
		if(eat(expr, '+'))
			x += parseTerm(expr, variable, value);		//addition
		else if(eat(expr, '-'))
			x -= parseTerm(expr, variable, value);		//substraction
		else
			return x;
}

// parses the terms of the expression according to multiplication and division
long double parseTerm(Expression *expr, const char variable, const long double value) {
	long double x = parseTerm2(expr, variable, value);
	while(TRUE)
		if(eat(expr, '*'))
			x *= parseTerm2(expr, variable, value);		//multiplication
		else if(eat(expr, '/')) {
			const long double temp = parseTerm2(expr, variable, value);
			if(!temp) {
				__mingw_printf("\nInvalid division: \"%Lg/0\"\nCan't divide by zero.\n", x);
				exit(1);
			}
			x /= temp;									//division
		}
		else
			return x;
}

// parses the terms of the expression according to modulo operation
long double parseTerm2(Expression *expr, const char variable, const long double value) {
	long double x = parseTerm3(expr, variable, value);
	while(TRUE)
		if(eat(expr, '%')) {
			const long double temp = parseTerm3(expr, variable, value);
			if(!temp) {
				__mingw_printf("\nInvalid modulo: \"%Lg%%0\"\nModulus can't be zero.\n", x);
				exit(1);
			}
			x = fmod((temp+fmod(x, temp)), temp);		//modulo operation
		}
		else
			return x;
}

// parses the terms of the expression according to exponentiation
long double parseTerm3(Expression *expr, const char variable, const long double value) {
	long double x = parseTerm4(expr, variable, value);
	while(TRUE)
		if(eat(expr, '^'))
			x = pow(x, parseTerm4(expr, variable, value));						//exponentiation
		else
			return x;
}

// parses the terms of the expression according to factorial
long double parseTerm4(Expression *expr, const char variable, const long double value) {
	long double x = parseFactor(expr, variable, value);
	while(TRUE)
		if(eat(expr, '!'))
			x = factorial(x);													//factorial
		else
			return x;
}

// parses the factors of the terms according to various elements such as parantheses, numbers, functions, constants, etc.
long double parseFactor(Expression *expr, const char variable, const long double value) {
	if(eat(expr, '+'))
		return parseFactor(expr, variable, value);					//positive numbers
	if(eat(expr, '-'))
		return -parseFactor(expr, variable, value);					//negative numbers
	long double x;
	const unsigned int startPos = expr->currentPos;
	if(eat(expr, '(')) {																			//parantheses
		x = parseExpression(expr, variable, value);
		eat(expr, ')');
	}
	else if((expr->currentChar>='0' && expr->currentChar<='9') || expr->currentChar=='.') {			//numbers
		while((expr->currentChar>='0' && expr->currentChar<='9') || expr->currentChar=='.')
			getNextChar(expr);
		x = atof(getSubstring(expr->expression, startPos, expr->currentPos));
	}
	else if(expr->currentChar>='a' && expr->currentChar<='z') { 									//functions and constants
		while(expr->currentChar>='a' && expr->currentChar<='z')
			getNextChar(expr);
		char* func = getSubstring(expr->expression, startPos, expr->currentPos);
		if(!strcmp(func,(char[]){variable, '\0'}))		return value;
		if(!strcmp(func,"e"))							return M_E;									//Euler's number, pi and infinity
		if(!strncmp(func,"pi",2))						return M_PI;
		if(!strncmp(func,"inf",3))						return LDBL_MAX;
		x = parseFactor(expr, variable, value);
		if(!strncmp(func,"sqrt",4)) {								//root (square root and cubic root)
			if(x<0.0) {
				__mingw_printf("\nInvalid square root: \"sqrt(%Lg)\"\nCan't take square root of negative numbers.\n", x);
				exit(1);
			}
			x = sqrt(x);
		}
		else if(!strncmp(func,"cbrt",4))		x = cbrt(x);
		else if(!strncmp(func,"sqr",3))			x *= x;				//exponentiation (square and cube)
		else if(!strncmp(func,"cube",4))		x *= x*x;
		else if(!strncmp(func,"abs",3))			x = fabs(x);		//absolute value
		else if(!strncmp(func,"ceil",4))		x = ceil(x);		//rounding
		else if(!strncmp(func,"floor",5))		x = floor(x);
		else if(!strncmp(func,"round",5))		x = round(x);
		else if(!strncmp(func,"fact",4))		x = factorial(x);	//factorial
		else if(!strncmp(func,"sinh",4))		x = sinh(x);		//hyperbolic trigonometric functions
		else if(!strncmp(func,"cosh",4))		x = cosh(x);
		else if(!strncmp(func,"tanh",4))		x = tanh(x);
		else if(!strncmp(func,"coth",4)) {
			if(!x) {
				puts("\nInvalid hyperbolic cotangent: \"coth(0)\"\nHyperbolic cotangent of zero doesn't exist.");
				exit(1);
			}
			x = 1.0 / tanh(x);
		}
		else if(!strncmp(func,"sech",4))
			x = 1.0 / cosh(x);
		else if(!strncmp(func,"csch",4)) {
			if(!x) {
				puts("\nInvalid hyperbolic cosecant: \"csch(0)\"\nHyperbolic cosecant of zero doesn't exist.");
				exit(1);
			}
			x = 1.0 / sinh(x);
		}
		else if(!strncmp(func,"sec",3)) {
			if(expr->anglesAreDegrees)
				x *= M_PI/180.0;
			if(fmod(x, M_PI)==M_PI/2.0) {
				__mingw_printf("\nInvalid secant: \"sec(%Lg)\"\nCan't take secant of angles that are %s in which k is an integer.\n", x, expr->anglesAreDegrees ? "90+180*k degrees" : "pi/2+pi*k radians");
				exit(1);
			}
			x = 1.0 / cos(x);
		}
		else if(!strncmp(func,"csc",3) || !strncmp(func,"cosec",5)) {
			if(expr->anglesAreDegrees)
				x *= M_PI/180.0;
			if(!fmod(x, M_PI)) {
				__mingw_printf("\nInvalid cosecant: \"csc(%Lg)\"\nCan't take cosecant of angles that are %s in which k is an integer.\n", x, expr->anglesAreDegrees ? "180*k degrees" : "pi*k radians");
				exit(1);
			}
			x = 1.0 / sin(x);
		}
		else if(!strncmp(func,"sin",3)) {							//trigonometric functions
			if(expr->anglesAreDegrees)
				x *= M_PI/180.0;
			x = fmod(x, M_PI) ? sin(x) : 0.0;
		}
		else if(!strncmp(func,"cos",3)) {
			if(expr->anglesAreDegrees)
				x *= M_PI/180.0;
			x = fmod(x, M_PI)==M_PI/2.0 ? 0.0 : cos(x);
		}
		else if(!strncmp(func,"tan",3)) {
			if(expr->anglesAreDegrees)
				x *= M_PI/180.0;
			if(fmod(x, M_PI)==M_PI/2.0) {
				__mingw_printf("\nInvalid tangent: \"tan(%Lg)\"\nCan't take tangent of angles that are %s in which k is an integer.\n", x, expr->anglesAreDegrees ? "90+180*k degrees" : "pi/2+pi*k radians");
				exit(1);
			}
			x = fmod(x, M_PI) ? tan(x) : 0.0;
		}
		else if(!strncmp(func,"cot",3)) {
			if(expr->anglesAreDegrees)
				x *= M_PI/180.0;
			if(!fmod(x, M_PI)) {
				__mingw_printf("\nInvalid cotangent: \"cot(%Lg)\"\nCan't take cotangent of angles that are %s in which k is an integer.\n", x, expr->anglesAreDegrees ? "180*k degrees" : "pi*k radians");
				exit(1);
			}
			x = fmod(x, M_PI)==M_PI/2.0 ? 0.0 : 1.0 / tan(x);
		}
		else if(!strncmp(func,"asin",4) || !strncmp(func,"arcsin",6)) {		//inverse trigonometric functions
			if(x<-1.0 || x>1.0) {
				__mingw_printf("\nInvalid arcsine: \"asin(%Lg)\"\nArcsine of numbers less than -1 or greater than 1 doesn't exist.\n", x);
				exit(1);
			}
			x = asin(x);
			if(expr->anglesAreDegrees)
				x *= 180.0/M_PI;
		}
		else if(!strncmp(func,"acos",4) || !strncmp(func,"arccos",6)) {
			if(x<-1.0 || x>1.0) {
				__mingw_printf("\nInvalid arccosine: \"acos(%Lg)\"\nArccosine of numbers less than -1 or greater than 1 doesn't exist.\n", x);
				exit(1);
			}
			x = acos(x);
			if(expr->anglesAreDegrees)
				x *= 180.0/M_PI;
		}
		else if(!strncmp(func,"atan",4) || !strncmp(func,"arctan",6)) {
			x = atan(x);
			if(expr->anglesAreDegrees)
				x *= 180.0/M_PI;
		}
		else if(!strncmp(func,"acot",4) || !strncmp(func,"arccot",6)) {
			x = atan(1.0/x);
			if(expr->anglesAreDegrees)
				x *= 180.0/M_PI;
		}
		else if(!strncmp(func,"asec",4) || !strncmp(func,"arcsec",6)) {
			if(-1.0<x && x<1.0) {
				__mingw_printf("\nInvalid arcsecant: \"asec(%Lg)\"\nArcsecant of numbers between -1 and 1 doesn't exist.\n", x);
				exit(1);
			}
			x = acos(1.0/x);
			if(expr->anglesAreDegrees)
				x *= 180.0/M_PI;
		}
		else if(!strncmp(func,"acsc",4) || !strncmp(func,"arccsc",6)) {
			if(-1.0<x && x<1.0) {
				__mingw_printf("\nInvalid arccosecant: \"acsc(%Lg)\"\nArccosecant of numbers between -1 and 1 doesn't exist.\n", x);
				exit(1);
			}
			x = asin(1.0/x);
			if(expr->anglesAreDegrees)
				x *= 180.0/M_PI;
		}
		else if(!strncmp(func,"ln",2)) {							//logarithm
			if(x<=0.0) {
				__mingw_printf("\nInvalid logarithm: \"ln(%Lg)\"\nCan't take logarithm of non-positive numbers.\n", x);
				exit(1);
			}
			x = log(x);
		}
		else if(!strncmp(func,"log",3)) {
			if(x<=0.0) {
				__mingw_printf("\nInvalid logarithm: \"log(%Lg)\"\nCan't take logarithm of non-positive numbers.\n", x);
				exit(1);
			}
			x = log10(x);
		}
		else if(!strncmp(func,"exp",3))								//e^x
			x = exp(x);
		else {
			printf("\nUnknown function: \"%s\"\n", func);
			exit(1);
		}
	}
	else {				//reached an unidentified character
		printf("\nUnexpected character: '%c'\n", expr->currentChar);
		exit(1);
	}
	return x;
}

#endif
