#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* TODO:
   create wrapper functions that automatically detect length and
   starting position of the string to be decoded */

void freechar (char * ptr) {
	if (ptr != NULL)
		free (ptr);
}

/*** FUNCTIONS ***/

/* decode string of binary into a plaintext string */
char * decodenBin (const char * binStr, size_t length) {
	int index;
	int outputIndex = 0;
	unsigned char num;
	
	/* allocate enough memory for output string:
	   8 is the minimum number of bits/spaces that can be used to display a
	   single character, so we divide the length by 8 to find the maximum
	   length of the output string. Add 1 to accommodate the null terminator. */
	char * stringOut = (char *) malloc (length / 8 + 1);
	
	/* check that all characters are either binary or space */
	for (index = 0 ; index < length; index++) {
		num = binStr[index];
		if (!(num == '0' || num == '1' || num == ' ')) {
			/* return NULL if invalid characters are found */
			free (stringOut);
			return NULL;
		}
	}
	
	/* skip past any leading spaces */
	index = 0;
	while (index < length) {
		if (binStr[index] != ' ')
			break;
		index++;
	}

	char bitNo;
	while (index + 8 <= length) {
		num = 0;

		for (bitNo = 0; bitNo < 8; bitNo++) {
			num += (128 >> bitNo) * (binStr[index + bitNo] - 48);
		}

		stringOut[outputIndex++] = num;
		index += 8;
		if (index + 1 >= length) {
			break;
		}

		while (!(binStr[index] == '1' || binStr[index] == '0') && index < length) {
			index++;
		}
	}

	/* terminate output string */
	stringOut[outputIndex] = '\0';
	
	return stringOut;
	/* remember to free stringOut after use! */
}

/* encode plaintext string into binary */
char * encodenBin (const char * string, size_t length) {
	int index;
	int outputIndex = 0;
	unsigned char num;
		
	/* allocate enough memory for output string:
	   9 is the maximum number of bits/spaces that can be used to display a
	   single character, so we multiply the length by 9 to find the maximum
	   length of the output string. Add 1 to accommodate the null terminator. */
	char * binaryString = (char *) malloc (9 * length + 1);

	char bit;
	for (index = 0; index < length; index++) {
		num = string[index];
		for (bit = 0; bit < 8; bit++) {
			/* if most significant bit is set */
			binaryString[outputIndex++] = (num & 0x80)
				? '1'
				: '0';
			num <<= 1;
		}
		/* append delimiter character between bytes */
		binaryString[outputIndex++] = ' ';
	}

	/* terminate output string */
	binaryString[outputIndex] = '\0';

	return binaryString;
	/* remember to free binaryString after use! */
}

/* decode string of hexadecimal into plaintext */
char * decodenHex (const char * hexStr, size_t length) {
	int index;
	int outputIndex = 0;
	unsigned char num;
	
	/* allocate enough memory for output string:
	   2 is the minimum number of hex digits that can be used to display a
	   single character, so we divide the length by 2 to find the maximum
	   length of the output string. Add 1 to accommodate the null terminator. */
	char * stringOut = (char *) malloc (length / 2 + 1);

	/* allocate memory for a copy of the input string:
	   This string will be a copy of hexStr, but with all alphabets
	   capitalized. We use a new string instead of hexStr, so that none
	   of the bytes in hexStr are overwritten, in case decodeHex() is
	   called with a pointer to a constant. */
	char * hexStrCaps = (char *) malloc ((length + 1) * sizeof (char));
		
	/* convert all lowercase to capital, while copying to the new string */
	for (index = 0; index < length; index++) {
		num = hexStr[index];
		hexStrCaps[index] = (96 <= num && num <= 123)
			? num - 32
			: num;
	}
	/* terminate new string */
	hexStrCaps[index] = '\0';
	
	/* check that all characters are valid hexadecimal or space */
	for (index = 0; index < length; index++) {
		num = hexStrCaps[index];
		if (!(
					('0' <= num && num <= '9')
					|| ('A' <= num && num <= 'F')
					|| num == ' ')) {
			/* return NULL if invalid characters are found */
			free (stringOut);
			free (hexStrCaps);
			return NULL;
		}
	}

	/* skip past any leading spaces */
	index = 0;
	while (index < length) {
		if (hexStr[index] != ' ')
			break;
		index++;
	}

	while (length - index >= 2) {
		num = 0;

		if ('0' <= hexStrCaps[index] && hexStrCaps[index] <= '9')
			num += (hexStrCaps[index] - 48) * 16;
		else if ('A' <= hexStrCaps[index] && hexStrCaps[index] <= 'F')
			num += (hexStrCaps[index] - 55) * 16;
		index++;
		
		if ('0' <= hexStrCaps[index] && hexStrCaps[index] <= '9')
			num += (hexStrCaps[index] - 48);
		else if ('A' <= hexStrCaps[index] && hexStrCaps[index] <= 'F')
			num += (hexStrCaps[index] - 55);
		index++;
		
		stringOut[outputIndex++] = num;

		if (hexStrCaps[index] == ' ')
			index++;
	}

	/* terminate output string */
	stringOut[outputIndex] = '\0';

	/* free memory used for capitalized input string */
	free (hexStrCaps);

	return stringOut;
	/* remember to free stringOut after use! */
}

/* encode plaintext string into hexadeximal */
char * encodenHex (const char * string, size_t length) {
	int index;
	int outputIndex = 0;
	
	/* allocate enough memory for output string:
	   3 is the maximum number of digits/spaces that can be used to display a
	   single character, so we multiply the length by 3 to find the maximum
	   length of the output string. Add 1 to accommodate the null terminator. */
	char * hexString = (char *) malloc (3 * length + 1);
	
	unsigned char i, j; 
	for (index = 0; index < length; index++) {
		i = string[index];
		j = i - (i % 16);
		j >>= 4; /* j now represents the first hex digit */
		if (j < 10)
			hexString[outputIndex++] = j + 48; /* 48 is the offset from '0' to 0 */
		else /* j will always be less than 16 at this point */
			hexString[outputIndex++] = j + 55; /* 55 is the offset from 'A' to 10 */
		
		i &= 0xF; /* i now represents the second hex digit */
		if (i < 10)
			hexString[outputIndex++] = i + 48;
		else /* i will always be less than 16 at this point */
			hexString[outputIndex++] = i + 55;

		hexString[outputIndex++] = ' ';
	}

	/* terminate output string */
	hexString[outputIndex] = '\0';

	return hexString;
	/* remember to free hexString after use! */
}

/* decode decimal string into plaintext */
char * decodenDec (const char * decStr, size_t length) {
	int index;
	int outputIndex = 0;
		/* maximum value of buf is 4294967265
	   Any number string contained in decStr up to 4294967296 will yield that
	   number % 256. Any number larger than this will yield undefined behavior. */
	uint32_t buf = 0;

	/* allocate enough memory for output string:
	   2 is the minimum number of decimal digits and spaces that can be used
	   to display a single character, so we divide the length by 2 to find the
	   maximum length of the output string. Add 1 to accommodate the null
	   terminator. */
	char * stringOut = (char *) malloc (length / 2 + 1);

	/* check that all characters are decimal or space */
	for (index = 0; index < length; index++) {
		buf = decStr[index];
		if (!(('0' <= buf && buf <= '9') || buf == ' ')) {
			/* return NULL if invalid characters are found */
			free (stringOut);
			return NULL;
		}
	}

	/* skip past any leading spaces */
	index = 0;
	while (index < length) {
		if (decStr[index] != ' ')
			break;
		index++;
	}

	buf = 0;
	while (index < length) {
		/* for every consecutive decimal digit read, the buffer is multiplied
		   by 10 to account for the new digit. */
		if ('0' <= decStr[index] && decStr[index] <= '9') {
			buf *= 10;
			buf += decStr[index] - 48;
			index++;
		}
		/* when a delimiter is encountered, write the buffer to stringOut and
		   reset the buffer to 0 */
		if (decStr[index] == ' ') {
			stringOut[outputIndex++] = (char) buf;
			buf = 0;
			index++;
		}
	}
	if (buf) { /* if buf not equal to 0 */
		stringOut[outputIndex++] = (char) buf;
	}

	/* terminate short string */
	stringOut[outputIndex] = '\0';

	return stringOut;
	/* remember to free stringOut after use! */
}

/* encode plaintext into decimal string */
char * encodenDec (const unsigned char * string, size_t length) {
	/* string is declared as const unsigned char* instead of const char*
	   because we require the characters to be expressed as solely positive
	   values in order for the math in this function to work. */
	
	int index;
	int outputIndex = 0;
	
	/* allocate enough memory for output string:
	   4 is the maximum number of digits/spaces that can be used to display a
	   single character, so we multiply the length by 4 to find the maximum
	   length of the output string. Add 1 to accommodate the null terminator. */
	char * decimalString = malloc (4 * length + 1);

	for (index = 0; index < length; index++) {
		if (string[index] > 99) { /* if there is a significant digit in the hundreds place */
			/* append hundreds digit */
			decimalString[outputIndex++] = string[index] / 100 + 48;
			/* append tens digit */
			decimalString[outputIndex++] = ((string[index] % 100) - (string[index] % 10)) / 10 + 48;
		}
		else if ((string[index] % 100) - (string[index] % 10) > 0) /* significant tens digit but no hundreds */
			decimalString[outputIndex++] = ((string[index] % 100) - (string[index] % 10)) /10 + 48;
		/* ones digit will always be appended */
		decimalString[outputIndex++] = (string[index] % 10) + 48;
		decimalString[outputIndex++] = ' ';
	}

	/* terminate output string */
	decimalString[outputIndex] = '\0';

	return decimalString;
	/* remember to free decimalString after use! */
}

/* ROT5 cipher */
char * nrot5 (const char * stringIn, size_t length) {
	int index;
	int num;
		
	/* allocate memory for output string */
	char * stringOut = (char *) malloc (length + 1);

	for (index = 0; index < length; index++) {
		num = stringIn[index];
		stringOut[index] = '0' <= num && num <= '9'
			? (num - 43) % 10 + 48
			: num;
	}
	
	/* terminate output string */
	stringOut[index] = '\0';

	return stringOut;
	/* remember to free stringOut after use! */
}

/* ROT13 cipher */
char * nrot13 (const char * stringIn, size_t length) {
	int index;
	int num;
		
	/* allocate memory for output string */
	char * stringOut = (char *) malloc (length + 1);
	
	for (index = 0; index < length; index++) {
		num = stringIn[index];
		if ('A' <= num && num <= 'Z') {
			stringOut[index] = (num - 52) % 26 + 65;
		} else if ('a' <= num && num <= 'z') {
			stringOut[index] = (num - 84) % 26 + 97;
		} else {
			stringOut[index] = num;
		}
	}
	
	/* terminate output string */
	stringOut[index] = 0;

	return stringOut;
	/* remember to free stringOut after use! */
}

/* ROT47 cipher */
char * nrot47 (const char * stringIn, size_t length) {
	int index;
	int num;
		
	/* allocate memory for output string */
	char * stringOut = (char *) malloc (length + 1);

	for (index = 0; index < length; index++) {
		num = stringIn[index];
		/* if character is an ASCII printable character */
		stringOut[index] = '!' <= num && num <= '~'
			? (num + 14) % 94 + 33
			: num;
	}
	
	/* terminate output string */
	stringOut[index] = 0;

	return stringOut;
	/* remember to free stringOut after use! */
}

/* plaintext string to base64 */
char * encodenBase64 (const unsigned char * stringIn, size_t length) {
	int outputIndex = 0;
	int index;
	/* group variable must be at least 24 bits wide */
	uint32_t group; 
	char buf;
		/* length of input string excluding any bytes that require padding */
	size_t adjustedLength = length - (length % 3);
	
	/* allocate memory for output string:
	   The length of the output string will be 4 * ceil (length of input / 3).
	   Since there's no ceiling division operator for ints, we use this obscure
	   calculation to obtain the same result. We add 1 byte to accomodate the
	   null terminator. */
	char * stringOut = (char *) malloc ((length / 3 + (length % 3 != 0)) * 4 + 1);
	
	/* the number of empty bytes at the end of the base64-encoded string;
	   these bytes will be used as padding. */
	int emptyBytes = (3 - (length % 3)) % 3;

	/* split byte triplets into groups of four 6-bit characters */
	index = 0;
	while (index < adjustedLength) {
		/* merge byte triplets into a single 24-bit integer */
		group = 0;
		group |= stringIn[index] << 16;
		group |= stringIn[index + 1] << 8;
		group |= stringIn[index + 2];
		
		/* now split that integer into four 6-bit characters */
		for (buf = 3; buf >= 0; buf--) 
			stringOut[outputIndex++] = (group >> (6 * buf)) & 0x3F;
		index += 3;
	}

	/* for the bytes at the end that require padding, if any;
	   If none require padding, then adjustedLength == length, so this loop
	   will not execute at all. */
	while (index < length) {
		group = 0;
		/* first byte will always be a complete character */
		group |= stringIn[index] << 16;
		if (emptyBytes == 1)
			/* only if last byte will need padding */
			group |= stringIn[index + 1] << 8;

		/* now split into four 6-bit characters */
		for (buf = 3; buf >= 0; buf--) 
			stringOut[outputIndex++] = (group >> (6 * buf)) & 0x3F;
		index += 3;
	}

	/* terminate output string */
	stringOut[outputIndex] = '\0';

	/* translate output characters into valid ASCII text */
	index = 0;
	/* more ceiling division for integers (refer to declaration of stringOut) */
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

	/* overwrite bytes designated as padding, with the padding character '=' */
	for (index = length - emptyBytes; index < length; index++)
		stringOut[index] = '=';
	
	/* terminate short string */
	stringOut[index] = 0;

	return stringOut;
	/* remember to free stringOut after use! */
}

/* base64 to plaintext */
char * decodenBase64 (const char * stringIn, size_t length) {
	int outputIndex = 0;
	int index;
	/* group variable must be at least 24 bits wide */
	uint32_t group;
	char buf;
		/* length of input string excluding any bytes that used padding */
	size_t adjustedLength;
	/* number of bytes in the output string that will be empty */
	int emptyBytes;
	
	/* copy stringIn to a mutable string */
	char * adjustedStringIn = (char *) malloc (length + 1);
	strcpy (adjustedStringIn, stringIn);
	
	/* trim off up to two trailing padding characters from input */
	for (buf = 0; buf < 2; buf++) { /* do twice :^) */
		if (adjustedStringIn[length - 1] == '=') {
			adjustedStringIn[--length] = '\0';
		}
	}
	/* use this newly calculated length to find adjusted length */
	adjustedLength = length - (length % 4);

	/* allocate memory for output string:
	   The length of the output string will be 3 * ceil (length of input / 4).
	   Since there's no ceiling division operator for ints, we use this obscure
	   calculation to obtain the same result. We add 1 byte to accomodate the
	   null terminator. */
	char * stringOut = (char *) malloc ((length / 4 + (length % 4 != 0)) * 3 + 1);
	
	/* check for invalid characters, exit if found */
	for (index = 0; index < length; index++) {
		buf = adjustedStringIn[index];
		if (!(
			('A' <= buf && buf <= 'Z')
			|| ('a' <= buf && buf <= 'z')
			|| ('0' <= buf && buf <= '9')
			|| buf == '/'
			|| buf == '+')) {
			/* return NULL if invalid characters are found */
			free (stringOut);
			free (adjustedStringIn);
			return NULL;
		}
	}

	/* if no invalid characters are found */
	if (length % 4 == 1) {
		/* return NULL if not enough or too many characters */
		free (stringOut);
		free (adjustedStringIn);
		return NULL;
	}

	/* translate ASCII characters into integers */
	index = 0;
	while (index < length) {
		buf = adjustedStringIn[index];
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
		adjustedStringIn[index++] = buf;
	}

	/* group characters into byte triplets */
	emptyBytes = (4 - (length % 4)) % 4;
	index = 0;
	while (index < adjustedLength) {
		group = 0;
		group |= adjustedStringIn[index] << 18;
		group |= adjustedStringIn[index + 1] << 12;
		group |= adjustedStringIn[index + 2] << 6;
		group |= adjustedStringIn[index + 3];

		/* now split into 3 bytes */
		for (buf = 2; buf >= 0; buf--)
			stringOut[outputIndex++] = (group >> (8 * buf));
		index += 4;
	}

	/* for the bytes at the end that required padding, if any;
	   If none required padding, then adjustedLength == length, so this loop
	   will not execute at all. */
	while (index < length) {
		group = 0;
		group |= adjustedStringIn[index] << 18; /* first 2 chars will always be valid */
		group |= adjustedStringIn[index + 1] << 12; 
		if (emptyBytes == 1 || emptyBytes == 0) /* if only last byte used padding or none used */
			group |= adjustedStringIn[index + 2] << 6;
		if (emptyBytes == 0) /* if no padding was needed */
			group |= adjustedStringIn[index + 3];
		
		/* now split into 3 bytes */
		for (buf = 2; buf >= 0; buf--)
			stringOut[outputIndex++] = (group >> (8 * buf));
		index += 4;
	}

	/* terminate output string and trim off any empty bytes */
	stringOut[outputIndex - emptyBytes] = 0;

	return stringOut;
	/* remember to free stringOut after use! */
}

/*** WRAPPER FUNCTIONS ***/

/* decode string of binary into a plaintext string */
char * decodeBin (const char * binStr) {
	size_t length = strlen (binStr);
	return decodenBin (binStr, length);
}

/* encode plaintext string into binary */
char * encodeBin (const char * string) {
	size_t length = strlen (string);
	return encodenBin (string, length);
}

/* decode string of hexadecimal into plaintext */
char * decodeHex (const char * hexStr) {
	size_t length = strlen (hexStr);
	return decodenHex (hexStr, length);
}

/* encode plaintext string into hexadeximal */
char * encodeHex (const char * string) {
	size_t length = strlen (string);
	return encodenHex (string, length);
}

/* decode decimal string into plaintext */
char * decodeDec (const char * decStr) {
	size_t length = strlen (decStr);
	return decodenDec (decStr, length);
}

/* encode plaintext into decimal string */
char * encodeDec (const unsigned char * string) {
	size_t length = strlen (string);
	return encodenDec (string, length);
}

/* ROT5 cipher */
char * rot5 (const char * stringIn) {
	size_t length = strlen (stringIn);
	return nrot5 (stringIn, length);
}

/* ROT13 cipher */
char * rot13 (const char * stringIn) {
	size_t length = strlen (stringIn);
	return nrot13 (stringIn, length);
}

/* ROT47 cipher */
char * rot47 (const char * stringIn) {
	size_t length = strlen (stringIn);
	return nrot47 (stringIn, length);
}

/* plaintext string to base64 */
char * encodeBase64 (const unsigned char * stringIn) {
	size_t length = strlen (stringIn);
	return encodenBase64 (stringIn, length);
}

/* base64 to plaintext */
char * decodeBase64 (const char * stringIn) {
	size_t length = strlen (stringIn);
	return decodenBase64 (stringIn, length);
}
