//file.cpp
//—одержит определени€ данных и методов дл€ класса FStream


#include <string>
#include <fstream>
#include "file.h"

namespace filestream
{
	FileStream::FileStream() : filename_("tabl.txt")
	{
		file_.open(filename_);
		if (file_.is_open())
			getline(file_, func_);
		else
			throw std::runtime_error("File not found.");
		file_.close();

		file_.open(filename_, std::ios::app);
		file_ << "\nThe result of the calculation." << std::endl;
	}

	FileStream::~FileStream()
	{
		file_.close();
	}

	std::string FileStream::getRead() const
	{
		return func_;
	}

	void FileStream::write(int i, std::string text)
	{
		file_ << i << "\t" << text << std::endl;
	}
}