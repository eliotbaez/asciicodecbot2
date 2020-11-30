#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>

/* Reddit maximum acceptable comment length */
#define MAX_COMMENT_LENGTH 10000

const wchar_t * freewchar (const wchar_t * ptr) {
	free ((void *) ptr);
	/* return address of freed memory */
	return ptr;
}

/* decode string of binary into a plaintext string */

const wchar_t * decodeBin (const wchar_t * binStr) {
	/* allocate memory for short string */
	char * sBinStr = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input to short string */
	size_t characters;
	characters = wcstombs (sBinStr, binStr, MAX_COMMENT_LENGTH);
	sBinStr[characters] = 0;
	
	char stringOut[MAX_COMMENT_LENGTH + 1];
	wchar_t * wStringOut;
	
	int index = 0;
	int outputIndex = 0;
	
	int num;
	size_t length = strlen (sBinStr);
	
	/* check that all characters are either binary or space */
	for ( ; index < length; index++) {
		num = sBinStr[index];
		if (!(num == '0' || num == '1' || num == ' ')) {
			/* return error message if invalid characters are found */
			wStringOut = (wchar_t *) malloc (20 * sizeof (wchar_t));
			wcscpy (wStringOut, L"Input invalid.");

			free (sBinStr);
			return wStringOut;
		}
	}
	
	index = 0;
	while (index < length) {
		num = 0;
		int bitNo;
		if (index + 8 > length) {
			break;
		}
		for (bitNo = 0; bitNo < 8; bitNo++) {
			num += (128 >> bitNo) * (sBinStr[index + bitNo] - 48);
		}
		/* Any non-ASCII characters replaced with dot */
		stringOut[outputIndex++] = (num & 0x80)
			? '.'
			: (char) num;
		index += 8;
		if (index + 1 >= length) {
			break;
		}

		while (!(binStr[index] == '1' || binStr[index] == '0')) {
			index += 1;
		}
	}

	/* terminate short string */
	stringOut[outputIndex] = 0;
	
	/* dynamically allocate memory for output string */
	wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1)* sizeof (wchar_t));
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
	char * sString = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sString, string, MAX_COMMENT_LENGTH);
	sString[characters] = 0;
	
	char binaryString[MAX_COMMENT_LENGTH + 1];
	
	int index;
	int outputIndex = 0;

	size_t length = strlen (sString);
	
	/*
	 * Each character of plaintext will occupy 9 characters
	 * when expressed in binary. 8 bits + 1 space
	 */
	if (length > MAX_COMMENT_LENGTH / 9) 
		length = MAX_COMMENT_LENGTH / 9;

	for (index = 0; index < length; index++) {
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
	char * sHexStr = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sHexStr, hexStr, MAX_COMMENT_LENGTH);
	sHexStr[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];
	wchar_t * wStringOut;

	int index = 0;
	int outputIndex = 0;
	int num;
	size_t length = strlen (sHexStr);
	
	/* check that all characters are valid hexadecimal or space */
	for ( ; index < length; index++) {
		num = sHexStr[index];
		if (!(
					('0' <= num && num <= '9')
					|| ('a' <= num && num <= 'f')
					|| ('A' <= num && num <= 'F')
					|| num == ' ')) {
			/* return error message if invalid characters are found */
                        wStringOut = (wchar_t *) malloc (20 * sizeof (wchar_t));
                        wcscpy (wStringOut, L"Input invalid.");

                        free (sHexStr);
                        return wStringOut;
		}
	}
	
	/* convert all lowercase to capital */
	for (index = 0; index < length; index++) {
		num = sHexStr[index];
		if ('a' <= num && num <= 'f') {
			sHexStr[index] = num - 32;
		}
	}

	index = 0;
	while (length - index >= 2) {
		num = 0;

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
		
		/* non-ASCII characters replaced with dot */
		stringOut[outputIndex++] = (num & 0x80)
			? '.'
			:(char) num;

		if (index > length)
			break;
		if (sHexStr[index] == ' ')
			index += 1;
	}

	/* terminate short string */
	stringOut[outputIndex] = 0;

	/* dynamically allocate memory for output string */
	wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
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
	char * sString = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sString, string, MAX_COMMENT_LENGTH);
	sString[characters] = 0;
	
	char hexString[MAX_COMMENT_LENGTH  + 1];

	int index;
	int outputIndex = 0;
	size_t length = strlen (sString);
	
	/*
	 * Each character of plaintext will occupy 3 characters
	 * when expressed in hexadecimal. 2 digits + 1 space
	 */
	if (length > MAX_COMMENT_LENGTH / 3)
		length = MAX_COMMENT_LENGTH / 3;
	
	int i, j;
	for (index = 0; index < length; index++) {
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
	char * sDecStr = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sDecStr, decStr, MAX_COMMENT_LENGTH);
	sDecStr[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];
	wchar_t * wStringOut;

	int index = 0;
	int outputIndex = 0;
	size_t length = strlen (sDecStr);
	int buf = 0;

	/* check that all characters are decimal or space */
	for ( ; index < length; index++) {
		buf = sDecStr[index];
		if (!(('0' <= buf && buf <= '9') || buf == ' ')) {
			/* return error message if invalid characters are found */
                        wStringOut = (wchar_t *) malloc (20 * sizeof (wchar_t));
                        wcscpy (wStringOut, L"Input invalid.");

                        free (sDecStr);
                        return wStringOut;
		}
	}

	index = 0;
	buf = 0;
	while (index < length) {
		if (sDecStr[index] == ' ') {
			/* non-ASCII characters replaced with dot */
			stringOut[outputIndex++] = (buf & 0x80)
				? '.'
				: (char) buf;
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
	if (buf) { /* if buf not equal to 0 */
		/* non-ASCII characters replaced with dot */
		stringOut[outputIndex++] = (buf & 0x80)
			? '.'
			:(char) buf;
	}

	/* terminate short string */
	stringOut[outputIndex] = 0;

	/* dynamically allocate memory for output string */
	wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
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
	char * sString = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input in to short string */
	size_t characters;
	characters = wcstombs (sString, string, MAX_COMMENT_LENGTH);
	sString[characters] = 0;
	
	/* 
	 * Every character of plaintext will occupy at most 4 characters
	 * expressed in decimal (3 decimal digits + 1 space).
	 */
	char decimalString[MAX_COMMENT_LENGTH * 4 + 1];
	
	int index = 0;
	int outputIndex = 0;
	size_t length = strlen (sString);

	while (index < length) {
		/* is this if statement even necessary? chars can't go above 255 */
		if (0 <= sString[index] && sString[index] < 256) {
			if (sString[index] > 99) {/* if there is a significant digit in the hundreds place */
				/* append hundreds digit */
				decimalString[outputIndex++] = sString[index] / 100 + 48;
				/* append tens digit */
				decimalString[outputIndex++] = ((sString[index] % 100) - (sString[index] % 10)) / 10 + 48;
			}
			else if ((sString[index] % 100) - (sString[index] % 10) > 0) /* significant tens digit but no hundreds */
				decimalString[outputIndex++] = ((sString[index] % 100) - (sString[index] % 10)) /10 + 48;
			/* ones digit will always be appended */
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

/* ROT5 cipher */

wchar_t * rot5 (const wchar_t * stringIn) {
	/* allocate memory for short string */
	char * sStringIn = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sStringIn, stringIn, MAX_COMMENT_LENGTH);
	sStringIn[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];

	int index;
	int num;
	size_t length = strlen (sStringIn);
	
	for (index = 0; index < length; index++) {
		num = sStringIn[index];
		stringOut[index] = '0' <= num && num <= '9'
			? (num - 43) % 10 + 48
			: num;
	}
	
	/* terminate short string */
	stringOut[index] = 0;
	
	/* dynamically allocate memory for output string */
	wchar_t * wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sStringIn);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* ROT13 cipher */

wchar_t * rot13 (const wchar_t * stringIn) {
	/* allocate memory for short string */
	char * sStringIn = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sStringIn, stringIn, MAX_COMMENT_LENGTH);
	sStringIn[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];
	
	int index;
	int num;
	size_t length = strlen (sStringIn);
	
	for (index = 0; index < length; index++) {
		num = sStringIn[index];
		if ('A' <= num && num <= 'Z') {
			stringOut[index] = (num - 52) % 26 + 65;
		} else if ('a' <= num && num <= 'z') {
			stringOut[index] = (num - 84) % 26 + 97;
		} else {
			stringOut[index] = num;
		}
	}
	
	/* terminate short string */
	stringOut[index] = 0;

	/* dynamically allocate memory for output string */
	wchar_t * wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sStringIn);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* ROT47 cipher */

wchar_t * rot47 (const wchar_t * stringIn) {
	/* allocate memory for short string */
	char * sStringIn = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sStringIn, stringIn, MAX_COMMENT_LENGTH);
	sStringIn[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];

	int index;
	int num;
	size_t length = strlen (sStringIn);
	
	for (index = 0; index < length; index++) {
		num = sStringIn[index];
		stringOut[index] = '!' <= num && num <= '~'
			? (num + 14) % 94 + 33
			: num;
	}
	
	/* terminate short string */
	stringOut[index] = 0;
	
	/* dynamically allocate memory for output string */
	wchar_t * wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sStringIn);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* plaintext string to base64 */

wchar_t * encodeBase64 (const wchar_t * stringIn) {
	/* allocate memory for short string */
	/* Short string will not necessarily be the same length as the wide string */
	char * sStringIn = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* char * sStringIn = (char *) malloc (wcslen (stringIn) + 1); */
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sStringIn, stringIn, MAX_COMMENT_LENGTH);
	sStringIn[characters] = 0;
	
	char stringOut[MAX_COMMENT_LENGTH + 1];
	wchar_t * wStringOut;
	int outputIndex = 0;

	int index = 0;
	/* group must be at least 24 bits wide */
	u_int32_t group = 0; 
	char buf;
	size_t length = wcslen (stringIn);
	
	/* check for invalid characters in wide string, abort if found */
	for ( ; index < length; index++) {
		if (stringIn[index] > 127) {
			/* return error message if invalid characters are found */
                        wStringOut = (wchar_t *) malloc (20 * sizeof (wchar_t));
                        wcscpy (wStringOut, L"Input invalid.");

                        free (sStringIn);
                        return wStringOut;
		}
		
	}

	index = 0;
	length = strlen (sStringIn);
	/* ceiling division for integers */
	if ((length / 3 + (length % 3 != 0)) * 4 > MAX_COMMENT_LENGTH) 
		length = MAX_COMMENT_LENGTH / 4 * 3;
	printf ("length adjusted to %d\n", length);
	
	int emptyBytes = (3 - (length % 3)) % 3;

	/* split byte triplets into groups of three 6-bit characters */
	while (index < length) {
		group = 0;
		if (length - index <= 3) {
			/* only if on last triplet in input string: */
			/* first byte will always be valid */
			group |= sStringIn[index] << 16;
			if (emptyBytes == 1 || emptyBytes == 0) {
				/* only if last byte will need padding or if none needed */
				group |= sStringIn[index + 1] << 8;
			}
			if (emptyBytes == 0) {
				/* only if no padding will be needed */
				group |= sStringIn[index + 2];
			}
		} else {
			/* for all other byte triplets: */
			group |= sStringIn[index] << 16;
			group |= sStringIn[index + 1] << 8;
			group |= sStringIn[index + 2];
		}

		/* now split into four 6-byte characters */
		for (buf = 3; buf >= 0; buf--) 
			stringOut[outputIndex++] = (group >> (6 * buf)) & 63;
		index += 3;
	}
	/* terminate output string */
	stringOut[outputIndex] = 0;

	/* translate output characters into valid text */
	index = 0;
	/* more ceiling division for integers */
	length = (length / 3 + (length % 3 != 0)) * 4;
	while (index < length) {
		buf = stringOut[index];
		if (0 <= buf && buf < 26) /* capital alphabet */
			buf += 65;
		else if (26 <= buf && buf < 52) /* small alphabet */
			buf += 71;
		else if (52 <= buf && buf < 62) /* numbers */
			buf -= 4;
		else if (buf == 62) /* '+' symbol */
			buf = 43;
		else if (buf == 63) /* '/' symbol */
			buf = 47;
		stringOut[index++] = buf;
	}

	/* overwrite padding characters */
	for (index = length - emptyBytes; index < length; index++)
		stringOut[index] = '=';
	
	/* terminate short string */
	stringOut[index] = 0;
	
	/* dynamically allocate memory for output string */
	wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sStringIn);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* base64 to plaintext */

wchar_t * decodeBase64 (const wchar_t * stringIn) {
	/* allocate memory for short string */
	char * sStringIn = (char *) malloc (MAX_COMMENT_LENGTH + 1);
	/* convert wide input into short string */
	size_t characters;
	characters = wcstombs (sStringIn, stringIn, MAX_COMMENT_LENGTH);
	sStringIn[characters] = 0;

	char stringOut[MAX_COMMENT_LENGTH + 1];
	wchar_t * wStringOut;
	int outputIndex = 0;
	
	int index;
	/* group must be at least 24 bits wide */
	u_int32_t group;
	char buf;
	size_t length = strlen (sStringIn);
	
	int emptyBytes;
	
	/* trim off up to two padding characters */
	for (buf = 0; buf < 1; buf++) { /* do twice :^) */
		if (sStringIn[length - 1] == '=') {
			sStringIn[--length] = 0;
		}
	}

	/* check for invalid characters, exit if found */
	for (index = 0; index < length; index++) {
		buf = sStringIn[index];
		if (!(
					('A' <= buf && buf <= 'Z')
					|| ('a' <= buf && buf <= 'z')
					|| ('0' <= buf && buf <= '9')
					|| buf == '/'
					|| buf == '+')) {
			/* return error message if invalid characters are found */
                        wStringOut = (wchar_t *) malloc (20 * sizeof (wchar_t));
                        wcscpy (wStringOut, L"Input invalid.");
			
                        free (sStringIn);
                        return wStringOut;
		}
	}

	/* else (implied) */
	if (length % 4 == 1) { /* not enough characters */
		/* return error message if invalid characters are found */
		wStringOut = (wchar_t *) malloc (20 * sizeof (wchar_t));
		wcscpy (wStringOut, L"Input invalid.");
		
		free (sStringIn);
		return wStringOut;
	}

	/* translate text into integers */
	index = 0;
	while (index < length) {
		buf = sStringIn[index];
		if ('A' <= buf && buf <= 'Z') /* capital alphabet */
			buf -= 65;
		else if ('a' <= buf && buf <= 'z') /* small alphabet */
			buf -= 71;
		else if ('0' <= buf && buf <= '9') /* numbers */
			buf += 4;
		else if (buf == '+') /* these two are self explanatory */
			buf = 62;
		else if (buf == '/')
			buf = 63;
		sStringIn[index++] = buf;
	}

	/* group integers into byte triplets */
	index = 0;
	emptyBytes = (4 - (length % 4)) % 4;

	while (index < length) {
		group = 0;

		if (length + emptyBytes - index <= 4) { /* only if on last group */
			group |= sStringIn[index] << 18; /* first 2 chars will always be valid */
			group |= sStringIn[index + 1] << 12; 
			if (emptyBytes == 1 || emptyBytes == 0) /* if only last byte used padding or none used */
				group |= sStringIn[index + 2] << 6;
			if (emptyBytes == 0) /* if no padding was needed */
				group |= sStringIn[index + 3];
		} else { /* for all other character groups */
			group |= sStringIn[index] << 18;
			group |= sStringIn[index + 1] << 12;
			group |= sStringIn[index + 2] << 6;
			group |= sStringIn[index + 3];
		}

		/* now split into 3 bytes */
		for (buf = 2; buf >= 0; buf--) {
			stringOut[outputIndex] = (group >> (8 * buf));
			if (stringOut[outputIndex] == 0) {
				/* replace null with dot to avoid premature termination */
				stringOut[outputIndex] = '.';
			}
			outputIndex++;
		}
		index += 4;
	}

	/* terminate output string and discard any empty bytes */
	stringOut[outputIndex - emptyBytes] = 0;

	/* dynamically allocate memory for output string */
	wStringOut = (wchar_t *) malloc ((MAX_COMMENT_LENGTH + 1) * sizeof (wchar_t));
	/* convert short string to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sStringIn);

	return wStringOut;
	/* remember to free wStringOut after use! */
}

