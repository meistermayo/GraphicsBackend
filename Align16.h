#pragma once

#include <corecrt_malloc.h>

class Align16
{
public:
	void* operator new(size_t size)
	{
		void* p = _aligned_malloc(size, 16);
		return p;
	}

	void* operator new[](size_t size)
	{
		void* p = _aligned_malloc(size, 16);
		return p;
	}

		void operator delete(void* p)
	{
		_aligned_free(p);
	}

	void operator delete[](void* p)
	{
		_aligned_free(p);
	}
};
