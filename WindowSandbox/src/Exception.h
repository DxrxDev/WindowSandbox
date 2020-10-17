#pragma once
#include<exception>
#include<string>

class Exception : public std::exception {
public:
	Exception(unsigned int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	unsigned int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	int line;
	std::string file;

protected:
	mutable std::string whatBuffer;
};