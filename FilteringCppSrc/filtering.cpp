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

	void operator()(T*** src, T*** out, uint32_t height, uint32_t width, uint8_t colors, int i, int j)
	{
		//for each color plane do the filtering
		for (uint8_t c = 0; c < colors; c++)
		{
			int up = i - floor(kernelY / 2);//TODO: check conversion to double
			int left = j - floor(kernelX / 2);
			int down = i + floor(kernelY / 2) + 1;
			int right = j + floor(kernelX / 2) + 1;
			int64_t sumWeights = 0;
			int64_t result = 0;
			int ki = 0;
			for (int k = up; k < down; k++)
			{
				if (k < 0 || k >= height) continue;
				int kj = 0;
				for (int l = left; l < right; l++)
				{
					if (l < 0 || l >= width) continue;
					result += kernel[ki][kj] * src[k][l][c];
					sumWeights += kernel[ki][kj];
					kj++;
				}
				ki++;
			}
			out[i][j][c] = T(round(result / sumWeights)); //same here
		}
	}
};

//template<typename T>
//class OrderStatKernel :Kernel
//{
//public:
//	T** operator()(T** )
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

	void operator()(T*** src, T*** out, uint32_t height, uint32_t width, uint8_t colors)
	{
		for (uint32_t i = 0; i < height; i++)
			for (uint32_t j = 0; j < width; j++)
				kernel(src, out, height, width, colors, i, j);
	}
};