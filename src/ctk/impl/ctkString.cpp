#include <ctk/impl/ctkString.h>
#include <cstring>
#include <utility>

ctkString& ctkString::operator=(const ctkString& other)
{
	if (this != &other)
	{
		char* newData = new char[std::strlen(other.data) + 1];
		std::strcpy(newData, other.data);
		delete[] data;
		data = newData;
	}
	return *this;
}

ctkString& ctkString::operator=(ctkString&& other) noexcept
{
	if (this != &other)
	{
		delete[] data;
		data = std::exchange(other.data, nullptr);
	}
	return *this;
}

size_t ctkString::size() const
{
	return std::strlen(data);
}

bool ctkString::operator==(const char* other) const
{
	return std::strcmp(data, other) == 0;
}

ctkString::ctkString()
	: data(new char[1] {'\0'}) {
}

ctkString::ctkString(const char* str)
{
	if (str)
	{
		size_t len = std::strlen(str);
		data = new char[len + 1];
		std::strcpy(data, str);
	}
	else
	{
		data = new char[1] {'\0'};
	}
}

ctkString::ctkString(const ctkString& other)
{
	size_t len = std::strlen(other.data);
	data = new char[len + 1];
	std::strcpy(data, other.data);
}

ctkString::ctkString(ctkString&& other) noexcept
	: data(std::exchange(other.data, nullptr)) {
}

ctkString::~ctkString()
{
	delete[] data;
}