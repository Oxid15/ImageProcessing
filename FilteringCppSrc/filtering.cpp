#include "misc.cpp"
#include <iostream>
#include <fstream>

template<typename T>
class Kernel
{
public:
	Kernel() {};
	virtual void operator()(T*** src, T*** out, uint32_t height, uint32_t width, uint8_t colors, uint32_t i, uint32_t j) {};
};

//class for kernels with constant weights
template<typename T>
class StaticWsKernel : public Kernel<T>
{
public:
	T** kernel;
	int kernelX;
	int kernelY;

	StaticWsKernel()
	{
		kernel = new T*;
		kernelX = 0;
		kernelY = 0;
	}

	StaticWsKernel(T** _kernel, int _kernelX, int _kernelY)
	{
		kernel = _kernel;
		kernelX = _kernelX;
		kernelY = _kernelY;
	}

	void operator()(T*** src, T*** out, uint32_t height, uint32_t width, uint8_t colors, uint32_t i, uint32_t j)
	{
		//for each color plane do the filtering
		for (uint8_t c = 0; c < colors; c++)
		{
			int up = i - kernelY / 2;
			int left = j - kernelX / 2;
			int down = i + kernelY / 2 + 1;
			int right = j + kernelX / 2 + 1;
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
			out[i][j][c] = T(round(result / sumWeights));//check double conversion
		}
	}
};

//class for kernels with order statistics (like median or percentile)
template<typename T>
class PercKernel : public Kernel<T>
{
	float perc;
	int kernelX;
	int kernelY;
public:

	PercKernel()
	{
		perc = 0.0;
		kernelX = 0;
		kernelY = 0;
	}

	PercKernel(float _percentile, int _kernelX, int _kernelY)
	{
		perc = _percentile;
		kernelX = _kernelX;
		kernelY = _kernelY;
	}

	void operator()(T*** src, T*** out, uint32_t width, uint32_t height, uint8_t colors, uint32_t i, uint32_t j)
	{
		//for each color plane do the filtering
		for (uint8_t c = 0; c < colors; c++)
		{
			int up = i - kernelY / 2;
			int left = j - kernelX / 2;
			int down = i + kernelY / 2 + 1;
			int right = j + kernelX / 2 + 1;

			int r = 0;
			T* kernel = new T[kernelX*kernelY];
			for (int k = up; k < down; k++)
			{
				if (k < 0 || k >= height) continue;
				for (int l = left; l < right; l++)
				{
					if (l < 0 || l >= width) continue;
					kernel[r] = src[k][l][c];
					r++;
				}
			}
			out[i][j][c] = percentile(kernel, r, perc);
		}
	}
};

template<typename T>
class Filter
{
public:
	Kernel<T>* kernel;

	Filter(Kernel<T>* _kernel)
	{
		kernel = _kernel;
	}

	void operator()(T*** src, T*** out, uint32_t height, uint32_t width, uint8_t colors)
	{
		for (uint32_t i = 0; i < height; i++)
			for (uint32_t j = 0; j < width; j++)
				kernel->operator()(src, out, height, width, colors, i, j);
	}
};