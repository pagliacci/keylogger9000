#include <stdio.h>
#include <stdlib.h>
#include "erasure.h"

extern unsigned FileSize;

int readPlain(const char *filename, unsigned char **buffer, size_t *bufferSize)
{
	FILE *input = NULL;
	size_t read;

	*buffer = NULL;
	*bufferSize = 0;

	input = fopen(filename, "rb");
	if (!input)
	{
		printf("Can not read in readPlain\n");
		return 1;
	}

	do
	{
		*buffer = (unsigned char *) realloc(*buffer, *bufferSize + BUFFER_PART + 1u);
		*bufferSize += read = fread(*buffer + *bufferSize, sizeof(unsigned char), BUFFER_PART, input);
	} while (read >= BUFFER_PART);

	(*buffer)[*bufferSize] = '\0';
	FileSize = *bufferSize;

	fclose(input);
	return 0;
}