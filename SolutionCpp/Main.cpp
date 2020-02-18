#include "C:\\Users\\илья\\Documents\\ImageProcessing\\BmpHandlerCppScr\\BmpHandler.cpp"

int main()
{
	bmpFile f;
	f.readFile("C:\\Users\\илья\\Google Диск\\Учёба\\Image Processing\\task\\Images\\34.bmp");
	f.writeFile("C:\\Users\\илья\\Google Диск\\Учёба\\Image Processing\\task\\Images\\2.bmp");
	return 0;	
}

//int height = abs(f.infoHeader.height);
//int width = f.infoHeader.width * f.infoHeader.bpp / 8;

//uint8_t** output = new uint8_t*[height];
//for (int i = 0; i < height; i++)
//	output[i] = new uint8_t[width];

//int size = 3;

//uint8_t** k = new uint8_t*[size];
//for (int i = 0; i < size; i++)
//	k[i] = new uint8_t[size];
//for (int i = 0; i < size; i++)
//	for (int j = 0; j < size; j++)
//		k[i][j] = 1;
//Kernel<uint8_t> kernel(k, size, size);
//Filter<uint8_t> filter(kernel);
//filter(f.data, output, height, width);
//f.data = output;