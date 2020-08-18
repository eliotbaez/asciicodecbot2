#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#define MAX_COMMENT_LENGTH 40000

const wchar_t * decodeBin (const wchar_t * binStr) {
	/* allocate memory for short string */
	char * sBinStr = (char *) malloc (wcslen (binStr) + 1);
	/* convert to short string */
	size_t characters;
	characters = wcstombs (sBinStr, binStr, MAX_COMMENT_LENGTH);
	binStr[characters] = 0;
	free (binStr);
	
	/* calculate max length of output string */
	maxStringLength = strlen (sBinStr) / 8 + 1;
	char * stringOut;
	stringOut = (char *) malloc (maxStringLength);
	int index = 0;
	int outputIndex = 0;
	
	int num;
	while (index < strlen (sBinStr)) {
		num = 0;
		int bitNo;
		for (bitNo = 0; bitNo < 8; bitNo++) {
			num += 128 >> bitNo * (sBinStr[index + bitNo] - 48);
		}
		printf ("%d\n", num);
		stringOut[outputIndex++] = num;
		index += 8;
		if (index + 1 >= strlen (sBinStr)) {
			break;
		}

		while (!(binStr[index] == '1' || binStr[index] == '0')) {
			index += 1;
		}
	}

	/* terminate string */
	stringOut[outputIndex] = 0;
	
	/* convert to wchar_t string */
	wchar_t * wStringOut = (wchar_t *) malloc (strlen (stringOut) + 1);
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* how to free memory before return? */

	return stringOut;
}

int main () {
	wchar_t input[9];
	scanf ("%ls", input);
	printf ("%ls", decodeBin (input));
	printf ("\nyes\n");
	return 0;
}
