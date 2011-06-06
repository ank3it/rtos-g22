/**
 * @file: util.c
 * @group: S11-G22
 * @brief: Utility functions
 * @date: 2011/05/26
 */

#include "util.h"

/**
 * @brief: Converts an integer to its ASCII representation
 * @param: n The integer to convert
 * @return: CHAR* Char pointer to the ASCII string
 */
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
	CHAR s[num_digits];

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

/*
CHAR* itoa( int i ) {
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
*/
