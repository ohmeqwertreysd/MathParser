//mathparser.h
//Заголовочный файл парсера математических выражений - содержит объявление класса
#pragma once
#ifndef MATHPARSER_H
#define MATHPARSER_H

namespace mathparser
{
	class MathParser
	{
	private:
		const char* carrage_;
	public:
		explicit MathParser(const char* carrage);
		~MathParser();
		std::string parseToken();
		void moveToken(int);
		double parser();
		double parseSimpleExpression();
		double parseBinaryExpression(int);
		int getPriority(char op);
	};
}

#endif // !MATHPARSER_H
