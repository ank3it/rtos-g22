/**
 * @file: util.c
 * @group: S11-G22
 * @brief: Utility functions
 * @date: 2011/05/26
 */

#include "util.h"

CHAR* itoa( int i ) {
    int count = 0;
    CHAR *out = "";
    CHAR *out_rev = "";
    CHAR *out_rev2 = "";
    CHAR *out_track = out;
    CHAR *out_track2 = out;
    int val = 0;

    while( i != 0 ) {
        val = i % 10;
        val = val + 48;
        *out = val;
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

