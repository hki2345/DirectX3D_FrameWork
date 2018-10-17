#pragma once
#include <iostream>

class FileStream
{
public:
	FILE* m_pFile;

public:
	bool IsFile() 
	{
		if (nullptr == m_pFile)
		{
			return false;
		}
		
		return true;
	}

public:
	void Seek(long _Size, int _Mode = SEEK_SET);
	void Write(void* _WBuf, int _ByteSize);
	void Read(void* _RBuf, int _BufByteSize, int _ReadByteSize);

public:
	FileStream(const wchar_t* _FileName, const wchar_t* _Mode);
	~FileStream();
};

