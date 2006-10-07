
#ifndef _INCLUDES_MAP_PRESETS_H_
#define _INCLUDES_MAP_PRESETS_H_

#define M(x) (-1000-x)

const int MAP_ALLOW_ALL[257] = 
{
	/*       0,   1,   2,   3,   4,   5,   6,   7,   8    9,   A,   B,   C,   D,   E,   F */
	/*0x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*1x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*2x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*3x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*4x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*5x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*6x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*7x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*8x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*9x*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*Ax*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*Bx*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*Cx*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*Dx*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*Ex*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	/*Fx*/ -747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,
	256 /* Valid character count... */
};

const int MAP_ALLOW_AZ[257] = 
{
	/*    0,   1,   2,   3,  4,   5,   6,   7,   8    9,   A,   B,   C,   D,   E,   F */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*1x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*2x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*3x*/ /* 0 = 0x30, 9 = 0x39 */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*4x*/ /* A = 0x41 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*5x*/ /* Z = 0x5a */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*6x*/ /* a = 0x61 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*7x*/ /* z = 0x7a */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*8x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*9x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ax*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Bx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Cx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Dx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ex*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Fx*/
	52 /* Valid character count... */
};

const int MAP_ALLOW_AZ_CI[257] = 
{
	/*   0,     1,     2,     3,     4,     5,     6,     7,     8      9,     A,     B,     C,     D,     E,     F */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*0x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*1x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*2x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*3x*/ /* 0=0x30 */
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*4x*/ /* A = 0x41 */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000, /*5x*/ /* Z = 0x5a */
	0x0000,M('A'),M('B'),M('C'),M('D'),M('E'),M('F'),M('G'),M('H'),M('I'),M('J'),M('K'),M('L'),M('M'),M('N'),M('O'), /*6x*/ /* a = 0x61 */
	M('P'),M('Q'),M('R'),M('S'),M('T'),M('U'),M('V'),M('W'),M('X'),M('Y'),M('Z'),0x0000,0x0000,0x0000,0x0000,0x0000, /*7x*/ /* z = 0x7a */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*8x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*9x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ax*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Bx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Cx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Dx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ex*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Fx*/
	26 /* Valid character count... */
};

const int MAP_ALLOW_AZ_SPACE[257] = 
{
	/* 0,   1,   2,   3,  4,   5,   6,   7,   8    9,   A,   B,   C,   D,   E,   F */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*1x*/
	-747,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*2x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*3x*/ /* 0 = 0x30, 9 = 0x39 */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*4x*/ /* A = 0x41 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*5x*/ /* Z = 0x5a */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*6x*/ /* a = 0x61 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*7x*/ /* z = 0x7a */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*8x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*9x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ax*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Bx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Cx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Dx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ex*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Fx*/
	53 /* Valid character count... */
};

const int MAP_ALLOW_AZ_SPACE_CI[257] = 
{
	/*   0,     1,     2,     3,     4,     5,     6,     7,     8      9,     A,     B,     C,     D,     E,     F */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*0x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*1x*/
	-747  ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*2x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*3x*/ /* 0=0x30 */
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*4x*/ /* A = 0x41 */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000, /*5x*/ /* Z = 0x5a */
	0x0000,M('A'),M('B'),M('C'),M('D'),M('E'),M('F'),M('G'),M('H'),M('I'),M('J'),M('K'),M('L'),M('M'),M('N'),M('O'), /*6x*/ /* a = 0x61 */
	M('P'),M('Q'),M('R'),M('S'),M('T'),M('U'),M('V'),M('W'),M('X'),M('Y'),M('Z'),0x0000,0x0000,0x0000,0x0000,0x0000, /*7x*/ /* z = 0x7a */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*8x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*9x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ax*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Bx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Cx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Dx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ex*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Fx*/
	27 /* Valid character count... */
};

const int MAP_ALLOW_AZ09[257] = 
{
	/*    0,   1,   2,   3,  4,   5,   6,   7,   8    9,   A,   B,   C,   D,   E,   F */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*1x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*2x*/
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00,0x00, /*3x*/ /* 0 = 0x30, 9 = 0x39 */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*4x*/ /* A = 0x41 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*5x*/ /* Z = 0x5a */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*6x*/ /* a = 0x61 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*7x*/ /* z = 0x7a */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*8x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*9x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ax*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Bx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Cx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Dx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ex*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Fx*/
	62 /* Valid character count... */
};

const int MAP_ALLOW_AZ09_CI[257] = 
{
	/*   0,     1,     2,     3,     4,     5,     6,     7,     8      9,     A,     B,     C,     D,     E,     F */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*0x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*1x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*2x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*3x*/ /* 0 = 0x30*/
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*4x*/ /* A = 0x41 */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000, /*5x*/ /* Z = 0x5a */
	0x0000,M('A'),M('B'),M('C'),M('D'),M('E'),M('F'),M('G'),M('H'),M('I'),M('J'),M('K'),M('L'),M('M'),M('N'),M('O'), /*6x*/ /* a = 0x61 */
	M('P'),M('Q'),M('R'),M('S'),M('T'),M('U'),M('V'),M('W'),M('X'),M('Y'),M('Z'),0x0000,0x0000,0x0000,0x0000,0x0000, /*7x*/ /* z = 0x7a */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*8x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*9x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ax*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Bx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Cx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Dx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ex*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Fx*/
	36 /* Valid character count... */
};

const int MAP_ALLOW_AZ09_SPACE[257] = 
{
	/*    0,   1,   2,   3,  4,   5,   6,   7,   8    9,   A,   B,   C,   D,   E,   F */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*0x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*1x*/
	-747,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*2x*/
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00,0x00, /*3x*/ /* 0 = 0x30, 9 = 0x39 */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*4x*/ /* A = 0x41 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*5x*/ /* Z = 0x5a */
	0x00,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747, /*6x*/ /* a = 0x61 */
	-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,-747,0x00,0x00,0x00,0x00,0x00, /*7x*/ /* z = 0x7a */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*8x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*9x*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ax*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Bx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Cx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Dx*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Ex*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, /*Fx*/
	63 /* Valid character count... */
};

const int MAP_ALLOW_AZ09_SPACE_CI[257] = 
{
	/*   0,     1,     2,     3,     4,     5,     6,     7,     8      9,     A,     B,     C,     D,     E,     F */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*0x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*1x*/
	-747  ,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*2x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*3x*/ /* 0 = 0x30*/
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*4x*/ /* A = 0x41 */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000, /*5x*/ /* Z = 0x5a */
	0x0000,M('A'),M('B'),M('C'),M('D'),M('E'),M('F'),M('G'),M('H'),M('I'),M('J'),M('K'),M('L'),M('M'),M('N'),M('O'), /*6x*/ /* a = 0x61 */
	M('P'),M('Q'),M('R'),M('S'),M('T'),M('U'),M('V'),M('W'),M('X'),M('Y'),M('Z'),0x0000,0x0000,0x0000,0x0000,0x0000, /*7x*/ /* z = 0x7a */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*8x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*9x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ax*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Bx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Cx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Dx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ex*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Fx*/
	37 /* Valid character count... */
};

/* Chars we want to allow ... lemmie know if i missed one
 *  45 0x2D - 
 *  91 0x5B [   92 0x5C \   93 0x5D ]   94 0x5E ^   95 0x5F _
 *  96 0x60 `
 * 123 0x7B {  124 0x7C |  125 0x7D } 126 0x7E ~ <--- Lower case of [|]^ respectively
 */

const int ALLOWED_NICK_CHARS[257] = 
{
	/*   0,     1,     2,     3,     4,     5,     6,     7,     8      9,     A,     B,     C,     D,     E,     F */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*0x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*1x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,  -747,0x0000,0x0000, /*2x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*3x*/ /* 0 = 0x30*/
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*4x*/ /* A = 0x41 */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*5x*/ /* Z = 0x5a */
	-747  ,M('A'),M('B'),M('C'),M('D'),M('E'),M('F'),M('G'),M('H'),M('I'),M('J'),M('K'),M('L'),M('M'),M('N'),M('O'), /*6x*/ /* a = 0x61 */
	M('P'),M('Q'),M('R'),M('S'),M('T'),M('U'),M('V'),M('W'),M('X'),M('Y'),M('Z'),M('['),M('\\'),M(']'),M('^'),0x0000, /*7x*/ /* z = 0x7a */
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*8x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*9x*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ax*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Bx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Cx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Dx*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Ex*/
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000, /*Fx*/
	43 /* Valid character count... */
};

/* Disallowed:
 *  Null   0 0x00
 *  Bell   7 0x07
 *  LF    10 0x0A
 *  CR    13 0x0D
 *  Space 32 0x20
 *  comma 46 0x2C
 */


const int ALLOWED_CHAN_CHARS[257] = 
{
	/*   0,     1,     2,     3,     4,     5,     6,     7,     8      9,     A,     B,     C,     D,     E,     F */
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,  -747,  -747,0x0000,  -747,  -747,0x0000,  -747,  -747, /*0x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*1x*/
	0x0000,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,0x0000,  -747,  -747,  -747, /*2x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*3x*/ /* 0 = 0x30*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*4x*/ /* A = 0x41 */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*5x*/ /* Z = 0x5a */
	-747  ,M('A'),M('B'),M('C'),M('D'),M('E'),M('F'),M('G'),M('H'),M('I'),M('J'),M('K'),M('L'),M('M'),M('N'),M('O'), /*6x*/ /* a = 0x61 */
	M('P'),M('Q'),M('R'),M('S'),M('T'),M('U'),M('V'),M('W'),M('X'),M('Y'),M('Z'),M('['),M('\\'),M(']'),M('^'),  -747, /*7x*/ /* z = 0x7a */
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*8x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*9x*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*Ax*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*Bx*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*Cx*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*Dx*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*Ex*/
	-747  ,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747,  -747, /*Fx*/
	220 /* Valid character count... */
};

#undef M

#endif