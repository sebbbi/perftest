#include "file.hpp"
#include <assert.h>
#include <cstdio>

long fileSize(FILE *f)
{
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}

std::vector<unsigned char> loadFile(const std::string& filename)
{
	std::vector<unsigned char> outVec;

	FILE *f = nullptr;
	fopen_s(&f, filename.c_str(), "rb");
	assert(f);

	if(f)
	{
		long size = fileSize(f);
		outVec.resize(size);
		fread_s(outVec.data(), size, sizeof(unsigned char), size, f);
		fclose(f);
	}

	return outVec;
}
