#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#define MAX_COMMENT_LENGTH 40000

const wchar_t * freewchar (const wchar_t * ptr) {
	free ((void *) ptr);
	/* return address of freed memory */
	return ptr;
}

/* decode string of binary into a plaintext string */

const wchar_t * decodeBin (const wchar_t * binStr) {
	/* allocate memory for short string */
	char * sBinStr = (char *) malloc (wcslen (binStr) + 1);
	/* convert wide input to short string */
	size_t characters;
	characters = wcstombs (sBinStr, binStr, MAX_COMMENT_LENGTH);
	sBinStr[characters] = 0;
	
	char stringOut[MAX_COMMENT_LENGTH + 1];
	/*stringOut = (char *) malloc (maxStringLength);*/ 
	
	int index = 0;
	int outputIndex = 0;
	
	int num;
	while (index < strlen (sBinStr)) {
		num = 0;
		int bitNo;
		if (index + 8 > strlen (sBinStr)) {
			break;
		}
		for (bitNo = 0; bitNo < 8; bitNo++) {
			num += 128 >> bitNo * (sBinStr[index + bitNo] - 48);
		}
		/* printf ("%d\n", num); */
		stringOut[outputIndex++] = (char) num;
		index += 8;
		if (index + 1 >= strlen (sBinStr)) {
			break;
		}

		while (!(binStr[index] == '1' || binStr[index] == '0')) {
			index += 1;
		}
	}

	/* terminate short string */
	stringOut[outputIndex] = 0;
	
	/* dynamically allocate memory for output string */
	wchar_t * wStringOut = malloc ((MAX_COMMENT_LENGTH + 1)* sizeof (wchar_t));
	/* convert to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate output wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sBinStr);
	
	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* encode plaintext string into binary */

const wchar_t * encodeBin (const wchar_t * string) {
	/* allocate memory for short string */
	char * sString = (char *) malloc (wcslen (string) + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sString, string, MAX_COMMENT_LENGTH);
	sString[characters] = 0;

	char binaryString[MAX_COMMENT_LENGTH + 1];
	
	int index;
	int outputIndex = 0;

	for (index = 0; index < strlen (sString); index++) {
		char bit;
		char i = sString[index];
		for (bit = 0; bit < 8; bit++) {
			if (i & 0x80) { /* if most significant bit is 1 */
				binaryString[outputIndex++] = '1';
			}
			else {
				binaryString[outputIndex++] = '0';
			}
			i <<= 1;
			if (i > 0xFF) {
				i -= 0x100;
			}
		}
		binaryString[outputIndex++] = ' ';
	}

	/* terminate short string */
	binaryString[outputIndex] = 0;
	
	/* dynamically allocate memory for output string */
	wchar_t * wBinaryString = malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wBinaryString, binaryString, MAX_COMMENT_LENGTH);
	/* terminate output wide string */
	wBinaryString[characters] = 0;
	/* free memory used for short input string */
	free (sString);

	return wBinaryString;
	/* remember to free wBinaryString after use! */
}

/* this was for testing purposes only */

int main () {
	const wchar_t input[9];
	scanf ("%ls", input);
	const wchar_t * string = encodeBin (input);
	printf ("%ls\n", string);
	freewchar (string);
	/* free (string);*/
	printf ("\nyes\n");
	return 0;
}
