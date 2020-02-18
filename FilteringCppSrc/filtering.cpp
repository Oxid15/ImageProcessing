#include "misc.cpp"
#include <iostream>
#include <fstream>

template<typename T>
class Kernel
{
public:
	T** kernel;
	int kernelX;
	int kernelY;

	Kernel()
	{
		kernel = new T*;
		kernelX = 0;
		kernelY = 0;
	}

	Kernel(T** _kernel, int _kernelX, int _kernelY)
	{
		kernel = _kernel;
		kernelX = _kernelX;
		kernelY = _kernelY;
	}

	void operator()(T** src, T** out, uint32_t height, uint32_t width, int i, int j)
	{
		int ki = 0;
		int kj = 0;
		int up = i - floor(kernelY / 2);
		int left = j - floor(kernelX / 2);
		int down = i + floor(kernelY / 2) + 1;
		int right = j + floor(kernelX / 2) + 1;
		uint64_t sumWeights = 0;
		uint64_t result = 0;
		for (int k = up; k < down; k++)
		{
			if (k < 0 || k >= height) continue;
			for (int l = left; l < right; l++)
			{
				if (l < 0 || l >= width) continue;
				result += kernel[ki][kj] * src[k][l]; //TODO: check how type conversion works
				sumWeights += kernel[ki][kj];
				kj++;
			}
			ki++;
		}
		out[i][j] = T(round(result / sumWeights)); //same here
	}
};

//template<typename T>
//class OrderStatKernel :Kernel
//{
//public:
//	T** operator()(T** neighb)
//	{
//
//	}
//};

template<typename T>
class Filter
{
public:
	Kernel<T> kernel;

	Filter(Kernel<T> _kernel)
	{
		kernel = _kernel;
	}

	void operator()(T** src, T** out, uint32_t height, uint32_t width)
	{
		for (uint32_t i = 0; i < height; i++)
		{
			for (uint32_t j = 0; j < width; j++)
			{
				kernel(src, out, height, width, i, j);
			}
		}
	}
};