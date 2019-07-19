#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>


int errors;

void test(char* str, double otv) {
	struct Parse* Pr = malloc(sizeof(struct Parse));
	strcpy(Pr->str, str);
	Pr->carriage = 0;

	double result = parser(Pr);
	if (result == otv)
		printf("ok: %s = %lf\n", str, result);
	else {
		printf("Error: %s\n", str);
		errors++;
	}
}

void input(char* str) {
	struct Parse* Pr = malloc(sizeof(struct Parse));
	if (Pr) {
		strcpy(Pr->str, str);
		Pr->carriage = 0;

		double result = parser(Pr);
		printf("ok: %s = %lf\n", str, result);
	}
}

int main() {
	test("1", 1);
	test("222", 222);
	test("-3", -3);
	test("4.0", 4.0);
	test("-555.0", -555.0);
	test("--555.0", 555.0);
	test("---555.0", -555.0);
	test("---6", -6);
	test("-+-6", 6);
	test("-++6", -6);
	test("abs(-2)", 2);
	test("sin(-2)", sin(-2));
	test("2", 2);
	test("-3", -3);
	test("abs(-101)", 101);
	test("-101", -101);
	test("sin(101)", sin(101));
	test("101", 101);
	test("0", 0);
	test("1", 1);
	test("9", 9);
	test("10", 10);
	test("+1", 1);
	test("-1", -1);
	test("(1)", 1);
	test("(-1)", -1);
	test("abs(-1)", 1);
	test("1+20", 21);
	test("1 + 20", 21);
	test("1+20+300", 321);
	test("1+20+300+4000", 4321);
	test("-1+20", 19);
	test("--1+20", 21);
	test("---1+20", 19);
	test("(1+20)", 21);
	test("-2*3", -6);
	test("2*-3", -6);
	test("1++2", 3);
	test("1+10*2", 21);
	test("10*2+1", 21);
	test("(1+20)*2", 42);
	test("2*(1+20)", 42);
	test("(1+2)*(3+4)", 21);
	test("2*3+4*5", 26);
	test("100+2*10+3", 123);
	test("2^3", 8);
	test("2^3*5+2", 42);
	test("5*2^3+2", 42);
	test("2+5*2^3", 42);
	test("1+2^3*10", 81);
	test("2^3+2*10", 28);
	test("5 * 4 + 3 * 2 + 1", 27);


	printf("End with %d errors\n", errors);

	char buf[MAX];
	scanf_s("%s", &buf, MAX);
	input(buf);


	system("pause");
	return 0;
}