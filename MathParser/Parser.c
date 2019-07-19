#include "Parser.h"

double parser(struct Parse* parse) {
	return  parseBinaryExpression(parse, 0);
}

char* parseToken(struct Parse* parse) {
	// Пропуск пробелов. 
	while (isspace(parse->str[parse->carriage]))
		parse->carriage++;

	// Если токен начинается с цифры - начинаем парсить число.
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

	// Если операция - парсим ее
	// Сравнивая с известными операциями.
	char* tokens[] = { "+","-","/","*","^","cos","sin","ln","abs","tan","(",")" };

	int len = sizeof(tokens) / (sizeof(char) * 4);
	int i;
	char newstr[MAX] = "";
	int lenstr = strlen(parse->str);

	// Перезапись строки, которую еще не распарсили
	// Для удобного поиска операции.
	for (i = 0; i < lenstr - parse->carriage; i++)
		newstr[i] = parse->str[parse->carriage + i];

	for (i = 0; i < len; i++) {
		if (!strncmp(newstr, tokens[i], strlen(tokens[i]))) {
			// Передвигаем каретку вперед на длину известной операции.
			parse->carriage += strlen(tokens[i]);
			// Возвращаем найденную операцию.
			return tokens[i];
		}
	}

	// Неизвестный токен.
	return "";
}

double parseSimpleExpression(struct Parse* parse) {
	char token[MAX] = "";
	// Парсинг первого токена.
	strcpy(token, parseToken(parse));

	// Если цифра - возвращаем ее.
	if (isdigit(token[0]))
		return atof(token);

	// Если скобка - пытаемся парсить ее внутренности.
	if (!strcmp(token, "(")) {
		double result = parser(parse);
		if (strcmp(parseToken(parse), ")"))
			parse->carriage++;
		return result;
	}

	// Иначе унарная операция
	// Парсим аргумент.
	double arg = parseSimpleExpression(parse);

	return calculate(token, arg, 0);
}

double parseBinaryExpression(struct Parse* parse, int minPriority) {
	double left = parseSimpleExpression(parse);

	while(1) {
		// Поиск всех операторов в выражении.
		char* op = parseToken(parse);
		int prior = getPriority(op);
		// Если приоритет низок - выход из цикла.
		if (prior <= minPriority) {
			parse->carriage -= strlen(op);
			// Возвращаем левое выражение.
			return left;
		}

		double right = parseBinaryExpression(parse, prior);

		// Обновление левого выражения.
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
	// Бинарная.
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
	// Унарная.
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