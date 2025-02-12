#include <ctk/impl/ctkString.h>
#include <cstring>
#include <utility>

ctkString& ctkString::operator=(const ctkString& other)
{
	if (this != &other)
	{
		char* newData = new char[other.length + 1];
		std::strcpy(newData, other.data);
		delete[] data;
		data = newData;
		length = other.length;
	}
	return *this;
}

ctkString& ctkString::operator=(ctkString&& other) noexcept
{
	if (this != &other)
	{
		delete[] data;
		data = std::exchange(other.data, nullptr);
		length = other.length;
	}
	return *this;
}

ctkString& ctkString::operator+=(const ctkString& other)
{
	length = length + other.length;
	char* newData = new char[length + 1];

	std::strcpy(newData, data);
	std::strcat(newData, other.data);

	delete[] data;
	data = newData;

	return *this;
}

size_t ctkString::size() const
{
	return length;
}

bool ctkString::operator==(const char* other) const
{
	return std::strcmp(data, other) == 0;
}

ctkString::ctkString()
	: data(new char[1] {'\0'}), length(0) {
}

ctkString::ctkString(const char* str)
{
	if (str)
	{
		length = std::strlen(str);
		data = new char[length + 1];
		std::strcpy(data, str);
	}
	else
	{
		data = new char[1] {'\0'};
		length = 0;
	}
}

ctkString::ctkString(const ctkString& other)
{
	length = other.length;
	data = new char[length + 1];
	std::strcpy(data, other.data);
}

ctkString::ctkString(ctkString&& other) noexcept
	: data(std::exchange(other.data, nullptr)), length(other.length) {
}

ctkString::~ctkString()
{
	delete[] data;
}