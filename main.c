#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "erasure.h"

int readPlain(const char *filename, unsigned char **buffer, size_t *bufferSize);
int readEncoded(const char *filenameMask, unsigned char **buffer, size_t *bufferSize, unsigned fileSize);
int readMeta(const char *metaFilename, char *decodeFilename);
int writePlain(const char *filename, const unsigned char *buffer, size_t bufferSize);
int writeEncoded(const char *filenameMask, const unsigned char *buffer, size_t bufferSize);
int writeMeta(const char *filename);
int encodeMode(unsigned char **buffer, size_t *bufferSize);
int decodeMode(unsigned char *buffer, size_t *bufferSize);
unsigned parseInput(int argc, char **argv, char **encodeFilename, char **metaFilename);
void printHelp(void);

extern int Parity;
extern int Degree;
extern unsigned FileSize;
extern unsigned PieceSize;
extern unsigned PieceNumber;
extern unsigned ErrorNumber;

int main(int argc, char **argv)
{
	unsigned char *buffer;
	size_t bufferSize;
	unsigned inputFlag;
	char *encodeFilename;
	char *metaFilename = META_FILE;
	char decodeFilename[256];
	time_t start, finish;

	start = clock();

	inputFlag = parseInput(argc, argv, &encodeFilename, &metaFilename);
	if (inputFlag & FLAG_ERROR)
	{
		printHelp();
		return 0;
	}

	if ((inputFlag & FLAG_CLEANUP) && !(inputFlag & FLAG_DECODE))
	{
		system("rmdir /s /q " OUTPUT_DIR "> nul 2> nul");
		if (!(inputFlag & FLAG_ENCODE) && !(inputFlag & FLAG_DECODE))
			return 0;
	}

	if (inputFlag & FLAG_ENCODE)
	{
		if (readPlain(encodeFilename, &buffer, &bufferSize))
			return 0;

		Parity = (int) ErrorNumber * 2;
		Degree = Parity * 2;

		PieceSize = bufferSize / PieceNumber;
		if (!PieceSize || (bufferSize % PieceSize))
			PieceSize++;
		printf("number: %u, size: %u\n", PieceNumber + 2 * ErrorNumber, PieceSize);
		if (PieceNumber + 2 * ErrorNumber > 255)
		{
			printf("Too much pieces\n");
			return 0;
		}

		encodeMode(&buffer, &bufferSize);
		writeEncoded(OUTPUT_MASK, buffer, bufferSize);
		writeMeta(encodeFilename);
	}
	else if (inputFlag & FLAG_DECODE)
	{
		if (readMeta(metaFilename, decodeFilename))
			return 0;

		Parity = (int) ErrorNumber * 2;
		Degree = Parity * 2;

		readEncoded(OUTPUT_MASK, &buffer, &bufferSize, FileSize);
		decodeMode(buffer, &bufferSize);
		writePlain(decodeFilename, buffer, FileSize);
	}

	if ((inputFlag & FLAG_CLEANUP) && !(inputFlag & FLAG_ENCODE))
		system("rmdir /s /q " OUTPUT_DIR "> nul 2> nul");

	finish = clock();
	printf("%lli ms\n", finish - start);

	return 0;
}