//expression.cpp
//Содержит определения данных и методов для класса математических операций

#include <string>
#include <stdexcept>
#include "expression.h"

namespace expression
{
	// Term
	Term::Term(double value) : value_(value) {	}

	Term::~Term() {	}

	double Term::calculation() const
	{
		return this->value_;
	}

	// Binary
	BinaryExpression::BinaryExpression(Term const* left, std::string op, Term const* right)
		:left_(left), op_(op.front()), right_(right) {	}

	double BinaryExpression::calculation() const
	{
		switch (op_)
		{
		case '+':
			return this->left_->calculation() + this->right_->calculation();
		case '-':
			return this->left_->calculation() - this->right_->calculation();
		case'^':
			return pow(this->left_->calculation(), this->right_->calculation());
		case '*':
			return this->left_->calculation() * this->right_->calculation();
		case '/':
			if (this->right_->calculation() == 0)
				throw std::logic_error("division by 0");
			else
				return this->left_->calculation() / this->right_->calculation();
		default:
			throw std::logic_error("Unknown operator.");
		}
		return 0.0;
	}

	BinaryExpression::~BinaryExpression()
	{
		delete left_;
		delete right_;
	}

	//Unary
	UnaryExpression::UnaryExpression(std::string op, Term const* left): op_(op), left_(left) {	}

	double UnaryExpression::calculation() const
	{
		if (op_ == "+")
			return +left_->calculation();
		else if (op_ == "-")
			return -left_->calculation();
		else if (op_ == "cos")
			return cos(left_->calculation());
		else if (op_ == "sin")
			return sin(left_->calculation());
		else if (op_ == "sqrt")
			return sqrt(left_->calculation());
		else if (op_ == "abs")
			return abs(left_->calculation());
		else if (op_ == "ln")
			return log(left_->calculation());
		else
			throw std::logic_error("Unknown operator.");
	}

	UnaryExpression::~UnaryExpression()
	{
		delete left_;
	}
}