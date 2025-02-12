/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once
#include <stdlib.h>

struct ctkString final
{
private:
	char* data;
	size_t length;

public:
	inline const char* c_str() const { return data; }
	inline size_t size() const;

	bool operator==(const ctkString& other) const { return *this == other.c_str(); }
	bool operator!=(const ctkString& other) const { return !(*this == other); }

	bool operator==(const char* other) const;
	bool operator!=(const char* other) const { return !(*this == other); }

	ctkString& operator=(const ctkString& other);
	ctkString& operator=(ctkString&& other) noexcept;

	ctkString& operator+=(const ctkString& other);

	ctkString();
	ctkString(const char* str);
	ctkString(const ctkString& other);
	ctkString(ctkString&& other) noexcept;

	~ctkString();
};