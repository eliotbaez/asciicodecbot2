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

const wchar_t * decodeBin (const wchar_t * binStr) {
	/* allocate memory for short string */
	char * sBinStr = (char *) malloc (wcslen (binStr) + 1);
	/* convert wide input to short string */
	size_t characters;
	characters = wcstombs (sBinStr, binStr, MAX_COMMENT_LENGTH);
	sBinStr[characters] = 0;
	
	/* 
	 * calculate max length of output string 
	 * -> no longer necessary due to no further use of dynamic allocation
	 */
	/* maxStringLength = strlen (sBinStr) / 8 + 1; */
	char stringOut[MAX_COMMENT_LENGTH];
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
	wchar_t * wStringOut = malloc (MAX_COMMENT_LENGTH * sizeof (wchar_t));
	/* convert to wchar_t string */
	characters = mbstowcs (wStringOut, stringOut, MAX_COMMENT_LENGTH);
	/* terminate output wide string */
	wStringOut[characters] = 0;
	/* free memory used for short input string */
	free (sBinStr);
	
	return wStringOut;
	/* remember to free wStringOut after use! */
}

/* this was for testing purposes only
int main () {
	const wchar_t input[9];
	scanf ("%ls", input);
	const wchar_t * string = decodeBin (input);
	printf ("%ls\n", string);
	freewchar (string);
	/\* free (string);*/
	/* printf ("\nyes\n"); *\/
	return 0;
}

*/
