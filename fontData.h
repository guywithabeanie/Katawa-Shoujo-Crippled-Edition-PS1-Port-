#ifndef FONT_DATA
#define FONT_DATA

#include "font.h"

static CharData playtimeCharData[] = {
	{  0, 0, 5 },
	{  6, 0, 4 },
	{ 12, 0, 5 },
	{ 19, 0, 6 },
	{ 27, 0, 6 },
	{ 34, 0, 6 },
	{ 41, 0, 6 },
	{ 49, 0, 6 },
	{ 56, 0, 6 },
	{ 64, 0, 5 },
	{ 71, 0, 5 },
	{ 78, 0, 5 },
	{ 84, 0, 5 },
	{ 91, 0, 5 },
	{ 97, 0, 5 },
	{ 104, 0, 5 },
	{ 110, 0, 5 },
	{ 117, 0, 5 },
	{ 123, 0, 3 },
	{ 127, 0, 4 },
	{ 133, 0, 5 },
	{ 139, 0, 3 },
	{ 0, 20, 6 },
	{ 7, 20, 5 },
	{ 14, 20, 5 },
	{ 21, 20, 5 },
	{ 28, 20, 5 },
	{ 35, 20, 4 },
	{ 41, 20, 5 },
	{ 47, 20, 5 },
	{ 54, 20, 4 },
	{ 60, 20, 5 },
	{ 66, 20, 7 },
	{ 74, 20, 5 },
	{ 81, 20, 5 },
	{ 87, 20, 5 },
	{ 94, 20, 6 },
	{ 102, 20, 6 },
	{ 110, 20, 6 },
	{ 118, 20, 6 },
	{ 127, 20, 5 },
	{ 134, 20, 4 },
	{ 140, 20, 7 },
	{ 149, 20, 6 },
	{ 0, 39, 2 },
	{ 4, 39, 5 },
	{ 11, 39, 6 },
	{ 19, 39, 4 },
	{ 25, 39, 7 },
	{ 35, 39, 6 },
	{ 43, 39, 6 },
	{ 51, 39, 6 },
	{ 59, 39, 6 },
	{ 67, 39, 6 },
	{ 75, 39, 5 },
	{ 82, 39, 5 },
	{ 89, 39, 6 },
	{ 97, 39, 6 },
	{ 104, 39, 8 },
	{ 114, 39, 6 },
	{ 121, 39, 6 },
	{ 129, 39, 6 },
	{ 137, 39, 3 },
	{ 142, 39, 3 },
	{ 147, 39, 5 },
	{ 154, 39, 3 }
};

FontData playtimeFontData = {
    .charCount = sizeof( playtimeCharData ) / sizeof( CharData ),
	.charHeight = 19,

    .charData = (CharData*) &playtimeCharData,
    .indices = {
		//Symbol set 1.
		NOT_PRESENT, 65, 66, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT,
		NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, 63, NOT_PRESENT, 62, NOT_PRESENT,
		//Numbers.
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		//Symbol set 2.
		NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, 64, NOT_PRESENT,
		//Alphabet (Uppercase)
		36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 
		49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
		//Symbol set 3.
		NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT,
		//Alphabet (Lowercase)
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
		//Symbol set 4.
		NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, NOT_PRESENT, 
    }
};

#endif
