#pragma once
#include <memory>
#include "Resource.h"
#include <assert.h>

using namespace std;

class ConstantBuffer: public Resource
{
public:
	ConstantBuffer(unsigned int aSize): Data(0), Size(aSize)
	{
		Data = _aligned_malloc(Size, 16);
		ZeroMemory(Data, sizeof(Data));
	}
	void UpdateData(void *UpdatedData, size_t DataSize)
	{
		assert(DataSize == Size);

		memcpy_s(Data, Size, UpdatedData, DataSize);
	}
	void* Data;
private:
	unsigned int Size;
};