#include <stdlib.h>
#include <string.h>
#include <wchar.h>
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
			num += (128 >> bitNo) * (sBinStr[index + bitNo] - 48);
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
	wchar_t * wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1)* sizeof (wchar_t));
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
	wchar_t * wBinaryString = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wBinaryString, binaryString, MAX_COMMENT_LENGTH);
	/* terminate output wide string */
	wBinaryString[characters] = 0;
	/* free memory used for short input string */
	free (sString);

	return wBinaryString;
	/* remember to free wBinaryString after use! */
}

/* decode string of hexadecimal into plaintext */

const wchar_t * decodeHex (const wchar_t * hexStr) {
	/* allocate memory for short string */
	char * sHexStr = (char *) malloc (wcslen (hexStr) + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sHexStr, hexStr, MAX_COMMENT_LENGTH);
	sHexStr[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];

	int index = 0;
	int outputIndex = 0;

	while (strlen (sHexStr) - index >= 2) {
		int num = 0;

		if ('0' <= sHexStr[index] && sHexStr[index] <= '9')
			num += (sHexStr[index] - 48) * 16;
		else if ('A' <= sHexStr[index] && sHexStr[index] <= 'F')
			num += (sHexStr[index] - 55) * 16;
		index++;
		
		if ('0' <= sHexStr[index] && sHexStr[index] <= '9')
			num += (sHexStr[index] - 48);
		else if ('A' <= sHexStr[index] && sHexStr[index] <= 'F')
			num += (sHexStr[index] - 55);
		index++;

		stringOut[outputIndex++] = num;

		if (index > strlen (sHexStr))
			break;
		if (sHexStr[index] == ' ')
			index += 1;
	}

	/* terminate short string */
	stringOut[outputIndex] = 0;

	/* dynamically allocate memory for output string */
	wchar_t * wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sHexStr);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* encode plaintext string into hexadeximal */

const wchar_t * encodeHex (const wchar_t * string) {
	/* allocate memory for short string */
	char * sString = (char *) malloc (wcslen (string) + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sString, string, MAX_COMMENT_LENGTH);
	sString[characters] = 0;

	char hexString[MAX_COMMENT_LENGTH + 1];

	int index;
	int outputIndex = 0;
	
	int i, j;
	for (index = 0; index < strlen (sString); index++) {
		i = sString[index];
		j = i - (i % 16);
		j /= 16; /* j now represents the first hex digit */
		if (j < 10)
			hexString[outputIndex++] = j + 48;
		else if (j < 16)
			hexString[outputIndex++] = j + 55;
		else
			hexString[outputIndex++] = '0';
		
		i -= j * 16; /* i represents the second hex digit */
		if (i < 10)
			hexString[outputIndex++] = i + 48;
		else if (i < 16)
			hexString[outputIndex++] = i + 55;
		else
			hexString[outputIndex++] = '0';

		hexString[outputIndex++] = ' ';
	}

	/* terminate short string */
	hexString[outputIndex] = 0;

	/* dynamically allocate memory for output string */
	wchar_t * wHexString = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wHexString, hexString, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wHexString[characters] = 0;
	/* free memory used for short input string */
	free (sString);

	return wHexString;
	/* remember to free wHexString after use! */
}

/* decode decimal string into plaintext */

wchar_t * decodeDec (const wchar_t * decStr) {
	/* allocate memory for short string */
	char * sDecStr = (char *) malloc (wcslen (decStr) + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sDecStr, decStr, MAX_COMMENT_LENGTH);
	sDecStr[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];

	int index = 0;
	int outputIndex = 0;
	int buf = 0;

	while (index < strlen (sDecStr)) {
		if (sDecStr[index] == ' ') {
			if (0 <= buf && buf < 256)
				stringOut[outputIndex++] = buf;
			buf = 0;
			index++;
			continue;
		}
		if ('0' <= sDecStr[index] && sDecStr[index] <= '9') {
			buf *= 10;
			buf += sDecStr[index] - 48;
			index++;
		}
	}
	if (buf) /* if not equal to 0 */
		stringOut[outputIndex++] = buf;

	/* terminate short string */
	stringOut[outputIndex] = 0;

	/* dynamically allocate memory for output string */
	wchar_t * wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sDecStr);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* encode plaintext into decimal string */

wchar_t * encodeDec (const wchar_t * string) {
	/* allocate memory for short string */
	char * sString = (char *) malloc (wcslen (string) + 1);
	/* convert wide input in to short string */
	size_t characters;
	characters = wcstombs (sString, string, MAX_COMMENT_LENGTH);
	sString[characters] = 0;

	char decimalString[MAX_COMMENT_LENGTH + 1];
	
	int index = 0;
	int outputIndex = 0;

	while (index < strlen (sString)) {
		/* is this if statement even necessary? chars can't go above 255 */
		if (0 <= sString[index] && sString[index] < 256) {
			if (sString[index] > 99) /* if there is a significant digit in the hundreds place */
				decimalString[outputIndex++] = sString[index] / 100 + 48;
			if ((sString[index] % 100) - (sString[index] % 10) > 0) /* significant tens digit */
				decimalString[outputIndex++] = ((sString[index] % 100) - (sString[index] % 10)) /10 + 48;
			decimalString[outputIndex++] = (sString[index] % 10) + 48;
			decimalString[outputIndex++] = ' ';
		}
		index++;
	}

	/* terminate short string */
	decimalString[outputIndex] = 0;

	/* dynamically allocate memory for output string */
	wchar_t * wDecimalString = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wDecimalString, decimalString, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wDecimalString[characters] = 0;
	/* free memory used for short input string */
	free (sString);

	return wDecimalString;
	/* remember to free wDecimalString after use! */
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
