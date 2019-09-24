//expression.h
//Заголовочный файл математических операций - содержит объявление класса
#pragma once

#ifndef EXPRESSION_H
#define EXPRESSION_H

namespace expression
{
	class Expression
	{
	public:
		virtual double calculation() const = 0;
		virtual ~Expression() {};
	};

	class Term : public Expression
	{
	private:
		double value_;
	public:
		Term(double);
		~Term();
		double calculation() const override;
	};

	class BinaryExpression : public Expression
	{
	private:
		Term const* left_;
		Term const* right_;
		char op_;
	public:
		explicit BinaryExpression(Term const*, std::string, Term const*);
		double calculation() const override;
		~BinaryExpression();
	};

	class UnaryExpression : public Expression
	{
	private:
		std::string op_;
		Term const* left_;
	public:
		explicit UnaryExpression(std::string,Term const*);
		double calculation() const override;
		~UnaryExpression();
	};
}

#endif // !EXPRESSION_H
