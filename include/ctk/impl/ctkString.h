/**********************************************/
/* THIS IS FOR INTERNAL CTK USE. DO NOT USE! */
/********************************************/

#pragma once

struct ctkString final
{
private:
	char* data;

public:
	inline const char* c_str() const { return data; }
	inline size_t size() const;

	bool operator==(const char* other) const;
	bool operator!=(const char* other) const { return !(*this == other); }

	ctkString& operator=(const ctkString& other);
	ctkString& operator=(ctkString&& other) noexcept;

	ctkString();
	ctkString(const char* str);
	ctkString(const ctkString& other);
	ctkString(ctkString&& other) noexcept;

	~ctkString();
};