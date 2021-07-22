// Operating System from Scratch Tutorials - ThatOSDev ( 2021 )
// https://github.com/ThatOSDev/ThatOS64

#define BITS8   7
#define BITS16 15
#define BITS32 31

#define HSPACE 1
#define VSPACE 1
                  // A R G B             
#define ORANGE     0xffffa500
#define CYAN       0xff00ffff
#define RED        0xffff0000
#define GREEN      0xff00ff00
#define BLUE       0xff0000ff
#define GRAY       0xff888888
#define WHITE      0xffffffff
#define BLACK      0xff000000
#define BADCYAN    0xff00aaaa
#define BADORANGE  0xff5a1100

// From the UEFI PDF Page 170
typedef struct EFI_MEMORY_DESCRIPTOR
{
	unsigned int        type;
	void*               PhysicalAddress;
	void*               VirtualAddress;
	unsigned long long  NumberOfPages;
	unsigned long long  Attributes;
} EFI_MEMORY_DESCRIPTOR;

typedef struct BLOCKINFO
{
    unsigned long long*     BaseAddress;
    unsigned long long      BufferSize;
    unsigned int            ScreenWidth;
    unsigned int            ScreenHeight;
    unsigned int            PixelsPerScanLine;
	unsigned long long*     LoaderFileSize;
	EFI_MEMORY_DESCRIPTOR*  MMap;
	unsigned long long      MMapSize;
	unsigned long long      MMapDescriptorSize;
} BLOCKINFO;

unsigned long long strlen(char* str);
void* memchr(const void* string, int chr, unsigned long long numberOfBytes);
void* memmove(void* destination, const void* source, unsigned long long numberOfBytes);
void *memset(void* str, int chr, unsigned long long numberOfBytes);
int memcmp(const void* src1, const void* src2, unsigned long long numberOfBytes);
void* memcpy(void* dst, const void* src, unsigned long long size);

void Print(BLOCKINFO* bli, char* str, const int screenWidth, const int a, const int b, const unsigned int FontSize, unsigned int c);
void PutCharacter(BLOCKINFO* bli, unsigned short chrNum, const int a, const int b, const unsigned int FontSize, unsigned int c);
void MakeRectangle(BLOCKINFO* bli, int a, int b, int w, int h, unsigned int c);

void main(BLOCKINFO* bi)
{
	Print(bi, "FuN", bi->ScreenWidth, 1, 1, 2, BADCYAN); // 70 117  78
	
	while(1){__asm__ ("hlt");}
}

void Print(BLOCKINFO* bli, char* str, const int screenWidth, const int a, const int b, const unsigned int FontSize, unsigned int c)
{
	// THIS IS NOT WORKING. NOT SURE WHY.
	// COMPARE TO main2.c in the testcode folder.
	
	#define DEBUGME 1    // TOGGLE THIS BETWEEN 0 AND 1
	                     // 1 shows the upper code
						 // 0 shows the lower code
	if(DEBUGME)
	{
		char* nrStr = str;   // F = 70, u = 117,  N = 78
		int i = 0;
		unsigned short l = 0;
		do{
			l = nrStr[i];
			i++;
			if(l == 117) {PutCharacter(bli, 117, 1, 1, FontSize, c);}
			if(i > 16){break;}
		} while(l != 0);
		// This rectangle is our way of saying "Done"
		MakeRectangle(bli, 1, 200, 20, 40, GREEN);
	} else {
		char* nrStr = str;   // F = 70, u = 117,  N = 78
		int i = 0;
		char l = 0;
		int x = 3;
		unsigned int fs = (((FontSize * FontSize) + (FontSize * FontSize) + 6) + HSPACE);
		do{
			l = nrStr[i];
			i++;
			if(l == 117) {PutCharacter(bli, 117, 1, 1, FontSize, c);}
			PutCharacter(bli, l, x, 50, FontSize, c);
			x += fs;
			if(i > 16){break;}
		} while(l != 0);
		// This rectangle is our way of saying "Done"
		MakeRectangle(bli, 1, 200, 20, 40, GREEN);
		PutCharacter(bli, i, 3, 203, FontSize, c);
	}
}

void PutCharacter(BLOCKINFO* bli, unsigned short chrNum, const int a, const int b, const unsigned int FontSize, unsigned int c)
{
	// I'm not sure why, but splitting this up into groups of arrays works.
	// My guess is, this is a stack issue, and splitting it up solves the stack issue.
unsigned int asciifont[64] = {
9490608,2415924244,337379332,67372800,       // 000  --  NUL
126,2175107457,3180953985,2113929216,        // 001  --  Start of Heading
126,4292607999,3286761471,2113929216,        // 002  --  Start of Text
102,4294967295,4294934076,402653184,         // 003  --  End of Text

0,406617855,4286463000,0,                    // 004  --  End of Transmission
0,406600935,3890681880,1006632960,           // 005  --  Enquiry
0,406617855,4286453784,1006632960,           // 006  --  Acknowledge
0,6204,1008205824,0,                         // 007  --  Bell

4294967295,4294961091,3286761471,4294967295, // 008  --  Backspace                  
0,3958338,1113996288,0,                      // 009  --  Horizontal Tab             
4294967295,4291008957,3180971007,4294967295, // 010  --  New Line ( Line Feed )   \n
7694,439515340,3435973752,0,                 // 011  --  Vertical Tab               

60,1717986918,1008238104,402653184,          // 012  --  Form Feed         
16179,1060122672,812708064,0,                // 013  --  Carriage Return \r
32611,2137219939,1743251136,0,               // 014  --  Shift Out         
24,417021159,1020991512,0                    // 015  --  Shift In          
};

unsigned int asciifont2[64] = {
32960,3773888766,4176535744,2147483648,      // 016  --  Data Link Escape
518,236863230,1042157062,33554432,           // 017  --  Device Control 1
24,1014896664,410926104,0,                   // 018  --  Device Control 2
27756,1819044972,1819017324,1811939328,      // 019  --  Device Control 3

32731,3688594203,454761243,0,                // 020  --  Device Control 4      
8177248,946652870,1815612614,2080374784,     // 021  --  Negative Ack          
0,255,4294967040,0,                          // 022  --  Synchronous Idle      
24,1014896664,410926104,2113929216,          // 023  --  End Transmission Block

24,1014896664,404232216,402653184,           // 024  --  Cancel              
24,404232216,404258364,402653184,            // 025  --  End of Medium       
0,1576190,202899456,0,                       // 026  --  Substitute          
0,3170558,1613758464,0,                      // 027  --  Escape ( ESC / FSC )

0,49344,3237871616,0,                        // 028  --  File Separator  
0,2649342,1814560768,0,                      // 029  --  Group Separator 
0,272119932,2097085952,0,                    // 030  --  Record Separator
0,4278090876,943198208,0                     // 031  --  Unit Separator  
};

unsigned int asciifont3[64] = {
0,0,0,0,                                     // 032  --  SPACE
6204,1010571288,402659352,0,                 // 033  --  !
102,1717969920,0,0,                          // 034  --  "
108,1828613228,1828613228,0,                 // 035  --  #

1579132,3334652028,101091014,2081953792,     // 036  --  $
0,3267759128,811648646,0,                    // 037  --  %
14444,1814853878,3704409292,1979711488,      // 038  --  &
1579032,805306368,0,0,                       // 039  --  '

12,405811248,808464408,201326592,            // 040  --  (
48,403442700,202116120,805306368,            // 041  --  )
0,26172,4282148352,0,                        // 042  --  *
0,1579134,404226048,0,                       // 043  --  +

0,0,1579032,805306368,                       // 044  --  ,
0,126,0,0,                                   // 045  --  -
0,0,24,402653184,                            // 046  --  .
0,33950744,811647104,0                       // 047  --  /
};

unsigned int asciifont4[64] = {
56,1824966358,3603351148,939524096,          // 048  --  0
24,947394584,404232216,2113929216,           // 049  --  1
60,1724255256,811647174,4261412864,          // 050  --  2
124,3322283580,101058246,2080374784,         // 051  --  3

12,473722060,4262202380,503316480,           // 052  --  4
254,3233857788,101058246,2080374784,         // 053  --  5
56,1623245052,3334915782,2080374784,         // 054  --  6
254,3322283532,405811248,805306368,          // 055  --  7

124,3334915708,3334915782,2080374784,        // 056  --  8
124,3334915710,101058060,2013265920,         // 057  --  9
0,1579008,1579008,0,                         // 058  --  :
0,1579008,1579056,0,                         // 059  --  ;

1548,405823680,1613764620,100663296,         // 060  --  <
0,32256,8257536,0,                           // 061  --  =
49248,806882310,202911840,3221225472,        // 062  --  >
124,3334867992,404226072,402653184           // 063  --  ?
};

unsigned int asciifont5[64] = {
124,3334921950,3739147456,2080374784,        // 064  --  @
16,946652870,4274439878,3321888768,          // 065  --  A
252,1717986940,1717986918,4227858432,        // 066  --  B
60,1724039360,3233858150,1006632960,         // 067  --  C

248,1818650214,1717986924,4160749568,        // 068  --  D
254,1717725304,1751147110,4261412864,        // 069  --  E
254,1717725304,1751146592,4026531840,        // 070  --  F
60,1724039360,3737568870,973078528,          // 071  --  G

198,3334915838,3334915782,3321888768,        // 072  --  H
60,404232216,404232216,1006632960,           // 073  --  I
30,202116108,214748364,2013265920,           // 074  --  J
230,1717988472,2020370022,3858759680,        // 075  --  K

240,1616928864,1616929382,4261412864,        // 076  --  L
198,4009688790,3334915782,3321888768,        // 077  --  M
198,3874946782,3469133510,3321888768,        // 078  --  N
124,3334915782,3334915782,2080374784         // 079  --  O
};

unsigned int asciifont6[64] = {
252,1717986940,1616928864,4026531840,        // 080  --  P
124,3334915782,3334919902,2081164800,        // 081  --  Q
252,1717986940,1818650214,3858759680,        // 082  --  R
124,3334889528,201770694,2080374784,         // 083  --  S

126,2119833624,404232216,1006632960,         // 084  --  T
198,3334915782,3334915782,2080374784,        // 085  --  U
198,3334915782,3334892600,268435456,         // 086  --  V
198,3334915798,3604414190,1811939328,        // 087  --  W

198,3328994360,946629830,3321888768,         // 088  --  X
102,1717986918,1008211992,1006632960,        // 089  --  Y
254,3330673688,811647686,4261412864,         // 090  --  Z
60,808464432,808464432,1006632960,           // 091  --  [

0,2160091184,403441154,0,                    // 092  --  "\"
60,202116108,202116108,1006632960,           // 093  --  ]
16,946652672,0,0,                            // 094  --  ^
0,0,0,4278190080                             // 095  --  _
};

unsigned int asciifont7[64] = {
48,806879232,0,0,                            // 096  --  `
0,30732,2093796556,1979711488,               // 097  --  a
224,1616935020,1717986918,2080374784,        // 098  --  b
0,15462,1616928870,1006632960,               // 099  --  c

28,202128492,3435973836,1979711488,          // 100  --  d
0,31942,4274045126,2080374784,               // 101  --  e
28,909258872,808464432,2013265920,           // 102  --  f
0,30412,3435973756,214726656,                // 103  --  g

224,1616931958,1717986918,3858759680,        // 104  --  h
0,404226104,404232216,1006632960,            // 105  --  i
6,100666886,101058150,1715208192,            // 106  --  j
224,1616930412,2021157990,3858759680,        // 107  --  k

48,808464432,808464432,939524096,            // 108  --  l
0,60670,3604403926,3321888768,               // 109  --  m
0,56422,1717986918,1711276032,               // 110  --  n
0,30924,3435973836,2013265920                // 111  --  o
};

unsigned int asciifont8[64] = {
0,15462,1719427168,1610612736,               // 112  --  p
0,30412,3435973756,202120704,                // 113  --  q
0,56438,1717592160,4026531840,               // 114  --  r
0,31942,1614286022,2080374784,               // 115  --  s

0,271593724,808464438,469762048,             // 116  --  t
0,52428,3435973836,1979711488,               // 117  --  u
0,27756,1819044920,268435456,                // 118  --  v
0,50902,3604403966,1811939328,               // 119  --  w

0,50796,943208556,3321888768,                // 120  --  x
0,50886,3334915710,101513216,                // 121  --  y
0,65228,405823686,4261412864,                // 122  --  z
28,808464608,808464432,469762048,            // 123  --  {

0,808464432,3158064,805306368,               // 124  --  |
224,808464412,808464432,3758096384,          // 125  --  }
118,3690987520,0,0,                          // 126  --  ~
16,946652870,3334929920,0                    // 127  --  Delete
};

unsigned int asciifont9[4] = {
45733,15462,1719427168,1610612736,           // 128 ^
};

    char togglearray = 0;
         if(chrNum >  15 && chrNum <   32){togglearray = 1; chrNum -=  16;}
	else if(chrNum >  31 && chrNum <   48){togglearray = 2; chrNum -=  32;}
	else if(chrNum >  47 && chrNum <   64){togglearray = 3; chrNum -=  48;}
	else if(chrNum >  63 && chrNum <   80){togglearray = 4; chrNum -=  64;}
	else if(chrNum >  79 && chrNum <   96){togglearray = 5; chrNum -=  80;}
	else if(chrNum >  95 && chrNum <  112){togglearray = 6; chrNum -=  96;}
	else if(chrNum > 111 && chrNum <  128){togglearray = 7; chrNum -= 112;}
	else if(chrNum > 127){chrNum = 0;}

    unsigned long character = (chrNum * 4);
	int x = a;
	int y = b;
	int temp = a;
	int xPos = 0;
	for(unsigned long fc = character; fc < (character + 4); fc++)
	{
		for(long t = BITS32; t >= 0; t--)
		{
			if(togglearray == 0)
			{
				if(asciifont[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 1){
				if(asciifont2[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 2){
				if(asciifont3[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 3){
				if(asciifont4[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 4){
				if(asciifont5[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 5){
				if(asciifont6[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 6){
				if(asciifont7[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else if(togglearray == 7){
				if(asciifont8[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			} else {
				if(asciifont9[fc] & (1 << t))
				{
					MakeRectangle(bli, x, y, FontSize, FontSize, c);
				}
			}
			x += FontSize;
			xPos++;
			if(xPos > 7)
			{
				x = temp;
				y += FontSize;
				xPos = 0;
			}
		}
	}
}

void MakeRectangle(BLOCKINFO* bli, int a, int b, int w, int h, unsigned int c)
{
	int width = (w + a);
	int height = (h + b);
	
	for(int y = b; y < height; y++)
    {
        for(int x = a; x < width; x++)
        {
            *(unsigned int*)(x + (y * bli->PixelsPerScanLine) + (unsigned int*)(bli->BaseAddress)) = c;
        }
    }
}

unsigned long long strlen(char* str)
{
	char* strCount = str;

	while (*strCount++);
	return strCount - str - 1;
}

void* memcpy(void* dst, const void* src, unsigned long long size)
{
    for ( unsigned long long i = 0; i < size; i++ )
        ((unsigned char*) dst)[i] = ((const unsigned char*) src)[i];
    return dst;
}

int memcmp(const void* src1, const void* src2, unsigned long long numberOfBytes)
{
  unsigned char const* str1 = src1;
  unsigned char const* str2 = src2;

  while (numberOfBytes--) {
    if (*str1 != *str2)
      return (int)*str1 - (int)*str2;
    str1++;
    str2++;
  }

  return 0;
}

void *memset(void* str, int chr, unsigned long long numberOfBytes)
{
  unsigned char* dest = str;

  while (numberOfBytes--)
    *dest++ = (unsigned char)chr;

  return str;
}

void* memmove(void* destination, const void* source, unsigned long long numberOfBytes)
{
  unsigned char* dest = destination;
  unsigned char const* src = source;

  if (dest < src) {
    while (numberOfBytes--)
      *dest++ = *src++;
  } else {
    dest += numberOfBytes;
    src += numberOfBytes;
    while (numberOfBytes--)
      *--dest = *--src;
  }

  return destination;
}

void* memchr(const void* string, int chr, unsigned long long numberOfBytes)
{
	unsigned long long   i = 0;
	unsigned char*       str       = (unsigned char*)string;
	unsigned char        asciiChar = (unsigned char)chr;
	while (i < numberOfBytes)
	{
		if (*str == asciiChar)
		{
			return (str);
		}
		str++;
		i++;
	}
	return (void*)0;
}

/*
	int x = a;
	int y = b;
    unsigned int fs = (((FontSize * FontSize) + (FontSize * FontSize) + 6) + HSPACE);
	for(unsigned long long t = 0; t < 128; t++)
	{
		PutCharacter(bli, t, x, y, FontSize, c);
		x+=fs;
		if(x > (screenWidth - fs))
		{
			x = a;
			y += (fs * 2);
		}
	}
*/
	/*
	for(unsigned long long t = 0; t < 128; t++)
	{
		PutCharacter(bli, t, x, y, FontSize, c);
		x+=fs;
		if(x > (screenWidth - fs))
		{
			x = a;
			y += (fs * 2);
		}
	}
	*/
	
// -Wno-unused-variable -ansi -masm=intel -std=c99 -O0 -nostdinc -nostdlib -ffreestanding

// __asm__ __volatile__


/*   // PSEUDO EXAMPLE
 // 000  --  NULL 
00000000100100001101000010110000 ---> 9490608
10010000000000000001010000010100 ---> 2415924244
00010100000111000000000000000100 ---> 337379332
00000100000001000000011100000000 ---> 67372800

	for(long t = 31; t >= 0; t--)
    {
      	if(155 & (1<<t))
        {
            Bit number is SET
        } else
        {
            Bit number is not SET
        }
     }
*/
/*
	int x = a;
	int y = b;
	long lngth = strlen(&q);

	for(long t = 0; t < lngth; t++)
	{
		MakeRectangle(bli, x, 1, 20, 40, GREEN);
		x+=24;
	}
	x = a;
	for(long t = 0; t < c; t++)
	{
		char l = q;
		PutCharacter(bli, l, x, y, FontSize, c);
		x+=fs;
		if(x > (screenWidth - fs))
		{
			x = a;
			y += (fs * 2);
		}
	}
*/

/*
	unsigned int fs = (((FontSize * FontSize) + (FontSize * FontSize) + 6) + HSPACE);
	int i = 0;
	int x = 1;
	while(st[i] != '\0')
	{
		PutCharacter(bli, st[i], x, 6, FontSize, c);
		i++;
		if(i > 16){break;}
		x+=fs;
	}
	
	MakeRectangle(bli, 1, 300, 20, 40, ORANGE);
	PutCharacter(bli, i, 3, 303, FontSize, c);
*/