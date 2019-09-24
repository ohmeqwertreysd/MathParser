//file.h
//Заголовочный файл для чтения и записи в файл - содержит объявление класса
#pragma once
#ifndef FILE_H
#define FILE_H

namespace filestream
{
	class FileStream
	{
	private:
		std::string filename_;
		std::fstream file_;
		std::string func_;
	public:
		FileStream();
		~FileStream();
		std::string getRead() const;
		void write(int i, std::string text);
	};
}

#endif // !FILE_H


