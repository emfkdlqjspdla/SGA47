#pragma once

template<typename T>
class singleton
{
protected :
	singleton(){}
	virtual ~singleton(){}

public :
	static T& getReference()
	{
		static T inst;
		return inst;
	}
};

template<typename T>
void SafeDelete(T& pointer)
{
	if (pointer)
	{
		delete [] pointer;
		pointer = NULL;
	}
}

template<typename CharType>
CharType* strAlloc(CharType* &dest, const CharType* const &src)
{
	SafeDelete(dest);
	int i = 0;
	while (src[i++]);

	dest = new CharType[i];

	i = 0;
	while (dest[i++] = src[i]);

	return dest;
}

template<typename CharType>
CharType* strAppend(CharType* &dest, const CharType* const &src)
{
	int dLen = 0;
	while (dest[dLen++]);

	int i = 0;
	while (src[i++]);

	// dLen 은 dest 문자열의 길이(널문자포함)
	// i 은 src 문자열의 길이(널문자포함)
	// 최종적인 문자열의 길이는 dLen + i - 1 이 된다.
	// -1 은 둘다 널문자를 포함하기 때문에 하나는 제외.
	dLen += i-1;

	CharType* buffer = new CharType[dLen];

	i = 0;
	while (buffer[i++] = dest[i]);
	i--;
	int j = 0;
	while (buffer[i++] = src[j++]);

	SafeDelete(dest);

	dest = buffer;

	return dest;
}