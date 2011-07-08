/**
 * @file: util.c
 * @group: S11-G22
 * @brief: Utility functions
 * @date: 2011/05/26
 */

#include "util.h"
#include "dbug.h"

/**
 * @brief: Converts an integer to its ASCII representation
 * @param: n The integer to convert
 * @return: CHAR* Char pointer to the ASCII string
 */

 CHAR s[100];

 CHAR* itoa(int n)
{
	int i, sign;
	int num_digits = 1;

	/* Get number of digits in n */
	i = n;
	do
	{
		i /= 10;
		++num_digits;
	} while (i != 0);

	/* Record sign, make n positive, increase num_digits */
	if ((sign = n) < 0)						
	{		
		n = -n;
		++num_digits;
	}

	/* CHAR array to hold the result */
	
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0)
		s[i++] = '-';				/* Append negative sign */

	s[i] = '\0';					/* Append terminating null char */

	reverse(s, num_digits);
	
	return s;
}

int atoi(char s[]) {
	int returnVal = 0;
	int i = 0;
	int a;
	while(s[i])
	{
		char b = s[i];
		switch (b) {
		 case '0' : a = 0; break;
		 case '1' : a = 1; break;
		 case '2' : a = 2; break;
		 case '3' : a = 3; break;
		 case '4' : a = 4; break;
		 case '5' : a = 5; break;
		 case '6' : a = 6; break;
		 case '7' : a = 7; break;
		 case '8' : a = 8; break;
		 case '9' : a = 9; break;
		 default: a = -1;
		 }
		if ( a == -1 ) {
			break;
		}
		returnVal = returnVal * 10 + a;
		i++;
	}
	return(returnVal);
}

/**
 * @brief: Reverses the given string
 * @param: s String to be reversed
 * @param: length The length of the string
 */
VOID reverse(CHAR *s, int length)
{
	CHAR c;
	int i = 0;
	int j = length - 2;

	while (i < j)	
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;

		i++;
		j--;
	}
}


CHAR* itoa2( int i ) {
    int count = 0;
    CHAR *out = "";
    CHAR out_rev = "";
    CHAR out_rev2 = "";
    CHAR *out_track = out;
    CHAR *out_track2 = out;
    int val = 0;

    while( i != 0 ) {
        val = i % 10;
        val = val + 48;
        *out = (CHAR *) val;
        *out++;
        i = i / 10;
        count++;
    }

    count = count / 2;

    *out = '\0';
    *out--;

    while( count != 0 ) {
        out_rev = *out;
        out_rev2 = *out_track;
        *out_track = out_rev;
        *out = out_rev2;
        *out--;
        *out_track++;
        count--;
    }

    return out_track2;
}

