#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <istream>
#include <map>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <cmath>
#include <queue>
#include <tuple>
#include <typeinfo>
#include <typeindex>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include "exampleKata.h"

namespace expr
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
		Term(double value) : value_(value)
		{	}

		~Term() 
		{	}

		double calculation() const override
		{ 
			return this->value_; 
		}
	};

	class BinaryExpression : public Expression
	{
	private:
		Term const* left_;
		Term const* right_;
		char op_;
	public:
		explicit BinaryExpression(Term const* left, std::string op, Term const* right) :left_(left), op_(op.front()), right_(right)
		{	}

		double calculation() const override
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

		~BinaryExpression() 
		{
			delete left_; 
			delete right_; 
		}
	};

	class UnaryExpression : public Expression
	{
	private:
		Term const* left_;
		std::string op_;
	public:
		explicit UnaryExpression(Term const* left, std::string op) : left_(left), op_(op) 
		{	}

		double calculation() const override
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

		~UnaryExpression()
		{
			delete left_; 
		}
	};
}

namespace mprs
{
	class MathParser
	{
	private:
		const char* carrage_;
	public:
		explicit MathParser(const char* carrage) : carrage_(carrage) 
		{  }

		std::string parseToken()
		{
			// skip space
			while (isspace(*carrage_))
				moveToken(1);

			// if char is start of digit
			if (std::isdigit(*carrage_))
			{
				std::string num;
				while (std::isdigit(*carrage_) || *carrage_ == '.')
				{
					num.push_back(*carrage_);
					moveToken(1);
				}
				return num;
			}

			// else it`s alpha

			const std::string operations[] = { "+","-","/","*","^","sin","cos","sqrt","tan","abs","ln", "(",")" };

			for (auto n : operations)
				if (!strncmp(carrage_, n.c_str(), n.size()))
				{
					moveToken(n.size());
					return n;
				}

			// def return
			return "";
		}

		void moveToken(int n)
		{
			if (*carrage_ != '\0')
				std::advance(carrage_, n);
		}

		double parser()
		{
			return parseBinaryExpression(0);
		}

		double parseSimpleExpression()
		{
			// извлекает символ
			std::string token = parseToken();

			// если это число, то вернуть его
			if (isdigit(token.front()))
				return std::stod(token);

			//  если это скобка, то начать вычислять в ней значение
			if (token == "(")
			{
				double ans = parser();
				if (parseToken() != ")")
					moveToken(1);
				return ans;
			}

			// иначе это унарная операция, ищем для нее значение
			expr::UnaryExpression* unary = new expr::UnaryExpression(new expr::Term(parseSimpleExpression()), token);
			double result = unary->calculation();
			delete unary;

			return result;
		}
		double parseBinaryExpression(int minPriority)
		{
			// unary or binary
			double left = parseSimpleExpression();

			for (;;)
			{
				// search operation
				std::string op = parseToken();
				int priority = op != "" ? getPriority(op.front()) : 0;



				if (priority <= minPriority)
				{
					int size = op.size();
					moveToken(-size);
					return left;
				}

				double right = parseBinaryExpression(priority);

				expr::BinaryExpression* binary = new expr::BinaryExpression(new expr::Term(left), op, new expr::Term(right));
				left = binary->calculation();
				delete binary;
			}
		}
		int getPriority(char op)
		{
			switch (op)
			{
			case '+': case '-':
				return 1;
			case '*': case '/':
				return 2;
			case '^':
				return 3;
			}
			return 0;
		}
		~MathParser() { }
	};
}

int errors = 0;
void test(std::string text, double answer)
{
	//text.push_back('\0');
	mprs::MathParser* dad = new mprs::MathParser(text.c_str());
	if (dad->parser() == answer)
		std::cout << "ok: " << text << "= " << answer << std::endl;
	else
	{
		errors++;
		std::cout << "errors: " << text << std::endl;
	}
	delete dad;
}
void exampleForParser()
{
	//Expression* binary = new BinaryOperation(new Term(1), "/", new Term(10));
	//std::cout << binary->evaluate() << std::endl;
	//delete binary;
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
	test("(3/5)/(3/5)",1);
	test("4^(2+1)",64);
	test("4^2+1",17);

	std::cout << "End with " << errors << " errors" << std::endl;
}


//https://www.codewars.com/kata/541af676b589989aed0009e7/train/cpp
//https://stackoverflow.com/questions/4243831/how-to-count-possible-combination-for-coin-problem


namespace fstr
{
	class FStream
	{
	private:
		std::fstream file_;
		std::string filename_;
		std::string func_;
	public:
		FStream() : filename_("tabl.txt")
		{
			file_.open(filename_, std::fstream::in | std::fstream::out| std::fstream::app);

			if (file_.is_open())
				getline(file_, func_);
			else
				throw std::runtime_error("File not found.");

			file_ << "The result of the calculation." << std::endl;
		}
		~FStream()
		{
			file_.close();
		}
		std::string getRead() const
		{
			return func_;
		}
		void write(int i, std::string text)
		{
			file_ << i << "\t" << text << std::endl;
		}
	};
}

namespace tbl
{
	class Tabl
	{
	private:
		std::string func;
		double a;
		double b;
		double dx;
	public:
		Tabl(): a(0.0), b(0.0), dx(0.0) {	}
		Tabl(double q,double w, double e): a(q), b(w), dx(e) {	}
		~Tabl() {	}
		void setArg()
		{
			std::cout << "Enter the data : interval [a;b] and step dx." << std::endl;
			std::cout << "a: "; std::cin >> a;
			std::cout << "b: "; std::cin >> b;
			std::cout << "dx: "; std::cin >> dx;
		}
		void tabulation()
		{
			for (double n = a; n <= b; n += dx)
			{
				n;
			}
		}
	};
}

//http://qaru.site/questions/121234/how-to-find-and-replace-string
std::string replaceString(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos)
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void tabl()
{
	std::string func = "sqrt(ln((x+2)/cos(x)))\0";
	for (int i = 1; i <= 10; i++)
	{
		std::string funcTmp = replaceString(func, "x", std::to_string(i));
		
		
		mprs::MathParser* dad = new mprs::MathParser(funcTmp.c_str());
		std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(6) << i << std::setw(15) << dad->parser() << std::endl;

		delete dad;
		funcTmp.clear();
	}
}


int main()
{
	//tabl();
	exampleForParser();
	fstr::FStream* da = new fstr::FStream();
	da->write(1, "da");
	da->write(2, "net");
	da->write(3, "cho");
	da->write(4, "sa");
	da->write(5, "fsda");
	da->write(6, "fsfdsda");
	da->write(7, "fsfdsda");
	da->write(8, "fsfdsda");
	da->write(9, "fsfdsgfdda");
	da->write(10, "fsddda");
	delete da;
	
	
	system("pause");
	return 0;
}
