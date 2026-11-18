#include <stdio.h>

typedef unsigned char chr;
typedef unsigned int uint;
typedef float flt;

flt lbff(flt x);
flt lgff(flt x);

chr upper(chr *s, uint c)
{	while(c--)
		*s++ &= 0xDF;

	return 0;
}

chr cprint(chr *s, uint c)
{	while(c--)
		putchar(*s++);

	return 0;
}

chr cprintln(chr *s, uint c)
{	cprint(s, c);
	printf("\n");

	return 0;
}

chr find256(chr key, chr *table, chr *flag)
{	chr idx = 0;
	chr carry = 0;
	chr f = 1;

	while(idx && !carry || f)
	{	if(table[idx] == key)
			break;

		idx++;
		if(!idx)
			carry |= 1;

		if(f)
			f = 0;
	} if(carry)
		*flag |= 1;

	return idx;
}

chr mncpy(chr *d, chr *s, uint c)
{	while(c--)
		*d++ = *s++;

	return 0;
}

chr mscpy(chr *d, chr *s, uint c, uint size)
{	while(c--)
	{	for(uint i = 0; i < size; i++)
			*d++ = *s++;
	} return 0;
}

uint lbuu(uint n)
{	uint lb = 0;

	while(n >> ++lb);

	return --lb;
}

flt lbuf(uint n)
{	return lbff((flt)n);
}

flt lbff(flt x)
{	flt lb = 0.0;
	flt m = 0.0;
	flt fx = (float)x;
	uint b = 0;
	uint mb = 0;
	int xpn = 0;

	mncpy((chr *)&b, (chr *)&fx, sizeof(b));
	xpn = (int)((b >> 23) & 0xFF) - 127;
	mb = (b & 0x007FFFFF) | 0x3F800000;
	mncpy((chr *)&m, (chr *)&mb, sizeof(m));
	if(1.41421356 < m)
	{	m *= 0.5;
		xpn++;
	} lb = -0.31351576 * m * m * m * m + 1.77077500 * m * m * m - 4.15880724 * m * m + 5.70104299 * m - 2.99942497;

	return (flt)xpn + lb;
}

uint lguu(uint n)
{	return (uint)lgff((flt)n);
}

uint lgfu(flt x)
{	return (uint)lgff(x);
}

flt lgff(flt x)
{	if(x)
		return 0.30103000 * lbff(x);

	return 0;
}

uint pwr(uint b, uint n)
{	if(b | n)
	{	uint k = 1;

		while(n--)
			k *= b;

		return k;
	} return 0;
}

int main(void)
{	chr c[5] = "";
	chr table[13] = "BWROYGUVAHDSC";
	chr si[11] = "\0KMGTPEZYRQ";
	chr idx = 0;
	chr flag = 0;
	uint dig = 0;
	flt rv = 0;
	flt mtable[12] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 100000000, 0.1, 0.01};
	flt ttable[13] = {-1, 1, 2, 3, 4, 0.5, 0.25, 0.1, 0.05, -1, 5, 10, 20};

	printf("黒: B\n茶: W\n赤: R\n橙: O\n黄: Y\n緑: G\n青: U\n紫: V\n灰: A\n白: H\n金: D\n銀: S\n無: C\n\n");
	printf("色配列を入力: ");
	scanf("%4s", c);

	upper(c, 5);
	c[4] = 0;

	for(chr i = 0; i < 4; i++)
	{	idx = find256(c[i], table, &flag);
		if(flag)
		{	printf("Invalid input\n");

			return 1;
		} switch(i)
		{	case 0: // 1st digit
				if(idx < 9)
				{	rv += idx;
					rv *= 10;
				} else
				{	printf("Invalid color: %c\n", table[idx]);

					return 2;
				} break;

			case 1: // 2nd digit
				if(idx < 9)
					rv += idx;

				else
				{	printf("Invalid color: %c\n", table[idx]);

					return 3;
				} break;

			case 2: // multiplier
				if(idx != 13)
					rv *= mtable[idx];

				break;

			case 3: // tolerance
				break;


			default:
				break;
		}
	} if(idx == 0 | idx == 9)
	{	printf("Invalid color: %c\n", table[idx]);

		return 4;
	} dig = 3 * (lgfu(rv) / 3);
	printf("%.4f%c (Ohm) [Range: %.1f%]\n", rv / pwr(10, dig), si[dig / 3], 2 * ttable[idx]);

	return 0;
}

// 0111 1111 1011 1

/*

B	Black
W	broWn
R	Red
O	Orange
Y	Yellow
G	Green
U	blUe
V	Violet
A	grAy
H	wHite
D	GolD
S	Silver
C	Clear

*/
/*

W	1
R	2
O	3
Y	4
G	0.5
U	0.25
V	0.1
A	0.05
D	5
S	10
C	20

*/
