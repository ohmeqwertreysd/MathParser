#include "Parser.h"

double parser(struct Parse* parse) {
	return  parseBinaryExpression(parse, 0);
}

char* parseToken(struct Parse* parse) {
	// ������� ��������. 
	while (isspace(parse->str[parse->carriage]))
		parse->carriage++;

	// ���� ����� ���������� � ����� - �������� ������� �����.
	if (isdigit(parse->str[parse->carriage])) {
		char number[MAX] = "";
		int i = 0;

		while (isdigit(parse->str[parse->carriage]) || parse->str[parse->carriage] == '.') {
			number[i] = parse->str[parse->carriage];
			parse->carriage++;
			i++;
		}

		return number;
	}

	// ���� �������� - ������ ��
	// ��������� � ���������� ����������.
	char* tokens[] = { "+","-","/","*","^","cos","sin","ln","abs","tan","(",")" };

	int len = sizeof(tokens) / (sizeof(char) * 4);
	int i;
	char newstr[MAX] = "";
	int lenstr = strlen(parse->str);

	// ���������� ������, ������� ��� �� ����������
	// ��� �������� ������ ��������.
	for (i = 0; i < lenstr - parse->carriage; i++)
		newstr[i] = parse->str[parse->carriage + i];

	for (i = 0; i < len; i++) {
		if (!strncmp(newstr, tokens[i], strlen(tokens[i]))) {
			// ����������� ������� ������ �� ����� ��������� ��������.
			parse->carriage += strlen(tokens[i]);
			// ���������� ��������� ��������.
			return tokens[i];
		}
	}

	// ����������� �����.
	return "";
}

double parseSimpleExpression(struct Parse* parse) {
	char token[MAX] = "";
	// ������� ������� ������.
	strcpy(token, parseToken(parse));

	// ���� ����� - ���������� ��.
	if (isdigit(token[0]))
		return atof(token);

	// ���� ������ - �������� ������� �� ������������.
	if (!strcmp(token, "(")) {
		double result = parser(parse);
		if (strcmp(parseToken(parse), ")"))
			parse->carriage++;
		return result;
	}

	// ����� ������� ��������
	// ������ ��������.
	double arg = parseSimpleExpression(parse);

	return calculate(token, arg, 0);
}

double parseBinaryExpression(struct Parse* parse, int minPriority) {
	double left = parseSimpleExpression(parse);

	while(1) {
		// ����� ���� ���������� � ���������.
		char* op = parseToken(parse);
		int prior = getPriority(op);
		// ���� ��������� ����� - ����� �� �����.
		if (prior <= minPriority) {
			parse->carriage -= strlen(op);
			// ���������� ����� ���������.
			return left;
		}

		double right = parseBinaryExpression(parse, prior);

		// ���������� ������ ���������.
		left = calculate(op, left, right);
	}
}

int getPriority(char* str) {
	if (!strcmp(str, "+") || !strcmp(str, "-"))
		return 1;

	if (!strcmp(str, "*") || !strcmp(str, "/"))
		return 2;

	if (!strcmp(str, "^"))
		return 3;

	return 0;
}

double calculate(char* op, double leftExpr, double rightExpr) {
	// ��������.
	if (leftExpr && rightExpr) {
		if (!strcmp(op, "+"))
			return leftExpr + rightExpr;

		if (!strcmp(op, "-"))
			return leftExpr - rightExpr;

		if (!strcmp(op, "*"))
			return leftExpr * rightExpr;

		if (!strcmp(op, "/"))
			return leftExpr / rightExpr;

		if (!strcmp(op, "^"))
			return powl(leftExpr, rightExpr);
	}
	// �������.
	else if (leftExpr) {
		if (!strcmp(op, "+"))
			return +leftExpr;

		if (!strcmp(op, "-"))
			return -leftExpr;
		if (!strcmp(op, "abs"))
			return fabsl(leftExpr);

		if (!strcmp(op, "sin"))
			return sin(leftExpr);

		if (!strcmp(op, "cos"))
			return cos(leftExpr);
	}
}