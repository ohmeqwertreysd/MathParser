// Parser.h
// заголовочный файл для парсера - содержит объявления структур и методов для них
#pragma once

#ifndef PARSER_H
#define PARSER_H

#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 80

struct Parse {
	char str[MAX];
	int carriage;
};

// Функция начала парсинга.
double parser(struct Parse* parse);

// Парсер токена(число/функция/операция).
char* parseToken(struct Parse* parse);

// Парсер унарной операции(-, cos, ^).
double parseSimpleExpression(struct Parse* parse);

// Парсер бинарной операции(a + b, a * b).
double parseBinaryExpression(struct Parse* parse, int minPriority);

// Приоритет операции.
int getPriority(char* str);

// Счет
double calculate(char* op, double leftExpr, double rightExpr);

#endif PARSER_H
