// Parser.h
// ������������ ���� ��� ������� - �������� ���������� �������� � ������� ��� ���
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

// ������� ������ ��������.
double parser(struct Parse* parse);

// ������ ������(�����/�������/��������).
char* parseToken(struct Parse* parse);

// ������ ������� ��������(-, cos, ^).
double parseSimpleExpression(struct Parse* parse);

// ������ �������� ��������(a + b, a * b).
double parseBinaryExpression(struct Parse* parse, int minPriority);

// ��������� ��������.
int getPriority(char* str);

// ����
double calculate(char* op, double leftExpr, double rightExpr);

#endif PARSER_H
