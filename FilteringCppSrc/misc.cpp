template<typename T>
T min(T x, T y) { return x < y ? x : y; }

template<typename T>
void insertionSort(T* arr, int size) //sorting in place
{
	T key = T(0);
	int i = 0;
	for (int j = 1; j < size; j++)
	{
		key = arr[j];
		i = j - 1;
		while (i >= 0 && arr[i] > key)
		{
			arr[i + 1] = arr[i];
			i = i - 1;
			arr[i + 1] = key;
		}
	}
}

template<typename T>
T percentile(T* data, int size, float percentile)// implements sorting in place; if percentile = 0 or 1 implements min and max respectively
{
	try
	{
		if (percentile >= 0 and percentile <= 1)
		{
			insertionSort(data, size);
			int idx = int(size*percentile) - 1;
			return idx >= size ? data[size - 1] : data[idx];
		}
		else
			throw std::exception("Percentile expected to be float between 0 and 1\n");
	}
	catch (const std::exception ex)
	{
		std::cerr << ex.what();
		return T(0);
	}
}