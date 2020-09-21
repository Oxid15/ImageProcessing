#pragma pack(push, 2)
#include <fstream>

struct bmpHeader
{
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
	//14 bytes
};

struct bmpInfoHeader
{
	uint32_t size;
	int32_t width;
	int32_t height;
	//12 + 14 = 26 bytes

	uint16_t planes;
	uint16_t bpp; //1, 4, 8, 16, 24, 32 bit per pixel
	uint32_t compression;
	uint32_t bit_size_image;
	int32_t x_ppm;
	int32_t y_ppm;
	uint32_t colors_used;
	uint32_t colors_important;
	//26 + 28 = 54 bytes
};

//handles .bmp files with colors > 8
struct bmpFile
{
	bmpHeader header;
	bmpInfoHeader infoHeader;
	uint8_t* palette;
	uint8_t*** data;

	bool readFile(const char* fileName)
	{
		std::ifstream inFile;
		inFile.open(fileName, std::ios::binary);
		if (inFile)
		{
			inFile.read((char*)&header, sizeof(bmpHeader));
			if (header.type != 0x4D42)
			{
				inFile.close();
				return false;
			}
			inFile.read((char*)&infoHeader, sizeof(bmpInfoHeader));
			inFile.seekg(header.offset);

			int height = abs(infoHeader.height);
			int width = infoHeader.width;
			int colors = infoHeader.bpp / 8;
			int padding = (width*colors % 4);

			data = new uint8_t**[height];
			for (int i = 0; i < height; i++)
			{
				data[i] = new uint8_t*[width];
				for (int j = 0; j < width; j++)
				{
					data[i][j] = new uint8_t[colors];
					for (int c = 0; c < colors; c++)
						data[i][j][c] = 0;
				}
			}
				
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					for (int k = 0; k < colors; k++)
					{
						inFile.read((char*)&data[i][j][k], 1);
					}
				}
				for (int k = 0; k < padding; k++)
				{
					char buffer;
					inFile.read(&buffer, 1);
				}
			}

			inFile.close();
			return true;
		}
		inFile.close();
		return false;
	}

	bool writeFile(const char* filename)
	{
		std::ofstream outFile;
		outFile.open(filename, std::ios::binary);
		if(outFile)
		{
			outFile.write(reinterpret_cast<char*>(&header), sizeof(header));
			outFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
			outFile.seekp(header.offset);
			if (infoHeader.bpp <= 8)
			{
				palette = new uint8_t[4 * infoHeader.colors_used];
				for (uint32_t i = 0; i > 4 * infoHeader.colors_used; i++)
					outFile.write((char*)palette[i], sizeof(char));
			}
			int height = abs(infoHeader.height);
			int width = infoHeader.width;
			int colors = infoHeader.bpp / 8;
			int padding = (width*colors % 4);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					for (int k = 0; k < colors; k++)
					{
						outFile.write((char*)&data[i][j][k], 1);
					}
				}
				for (int k = 0; k < padding; k++)
				{
					uint8_t null = 0;
					outFile.write((char*)&null, 1);
				}
			}
			return true;
		}
		else
			return false;
	}
	
	//~bmpFile()
	//{
	//	for (int i = 0; i < infoHeader.height; i++)
	//		for (int j = 0; j < infoHeader.width; j++)
	//			delete data[i][j];
	//	for (int i = 0; i < infoHeader.height; i++)
	//		delete data[i];
	//	delete data;
	//}
};
#pragma pack (pop)