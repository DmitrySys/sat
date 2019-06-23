#include "mySimpleComputer.h"
#include "myBigChars.h"
#include "myReadKey.h"
#include <iostream>
#define defColor 30
#define defBgColor 104
#define selColor 34
#define selBgColor 40
#define fontBold 0

#define rowWidth 10
#define fileNameLen 30

#define iorow 23
#define minRows 24
#define minCols 80
bool isDrowNow = false;

int printEl (int value, int row, int col, int color, int bgcolor)
{
	int comm, oper, k;
	sc_commandDecode(value, &comm, &oper);
	k = value >> 14;
	mt_gotoXY(row, col);

	if (color != -1) mt_setFont (fontBold, color, bgcolor);
	printf("%c%02X%02X",(k == 0) ? '+' : '-',comm,oper);
	if (color != -1) mt_setFont (fontBold, defColor, defBgColor);

	return 0;
}

int printMemory ()
{
    int i, j = 1;
	int value;
	for(i = 0; i < 100; i++)
	{
		if(i % rowWidth == 0) j++;
		sc_memoryGet(i, &value);
		if (i != icounter)
			printEl (value, j, (i % rowWidth) * 6 + 2, -1, -1);
		else
			printEl (value, j, (i % rowWidth) * 6 + 2, selColor, selBgColor);
	}
	return 0;
}
int printBigMemory (int address, int row, int col, int margin)
{
	int value, comm, oper, i;
	sc_memoryGet (address, &value);
	sc_commandDecode (value, &comm, &oper);

	int k = value >> 14;
	k == 0 ? bc_printbigchar (bigChar[16], row, col) :
				bc_printbigchar (bigChar[17], row, col);
			col += 8 + margin;
	for (i=1; i>=0; i--)
	{
		bc_printbigchar (bigChar[comm >> (i*4) & 15], row, col);
		col += 8 + margin;
	}
	for (i=1; i>=0; i--)
	{
		bc_printbigchar (bigChar[oper >> (i*4) & 15], row, col);
		col += 8 + margin;
	}
	return 0;
}
void drawConsole ()
{
	mt_setFont (fontBold, defColor, defBgColor);
	mt_clrscr ();

	bc_box (1, 1, 12, 61);
	bc_box (13, 1, 10, 52);
	bc_box (13, 53, 10, 27);
	for (int i=0; i<4; i++)
		bc_box (1+i*3, 62, 3, 18);

	int tmp = 55;
	mt_gotoXY(13, tmp);	printf("Keys:");
	mt_gotoXY(14, tmp); printf("l  - load");
	mt_gotoXY(15, tmp); printf("s  - save");
	mt_gotoXY(16, tmp); printf("r  - run");
	mt_gotoXY(17, tmp); printf("t  - step");
	mt_gotoXY(18, tmp); printf("i  - reset");
	mt_gotoXY(19, tmp); printf("F5 - accumulator");
	mt_gotoXY(20, tmp); printf("F6 - instr-n counter");
	mt_gotoXY(21, tmp); printf("q - redraw all");
  mt_gotoXY (0, 2); 	printf("\tMemory");
  mt_gotoXY (1, 64); 	printf("Accumulator");
  mt_gotoXY (4, 64); 	printf("Inst-n Counter");
	mt_gotoXY (7, 64); 	printf("Operation");
	mt_gotoXY (10, 64); printf("Flags");

	mt_cursorVisible (0);
}

void drawHud ()
{
    int flag = 0;

    mt_gotoXY (11, 64); sc_regGet (P, &flag);
		if ( flag == 1 ) cout << "P ";
		else cout << " ";
	mt_gotoXY (11, 66); sc_regGet (O, &flag);
		if ( flag == 1 ) cout << "O ";
		else cout << " ";
	mt_gotoXY (11, 68); sc_regGet (M, &flag);
		if ( flag == 1 ) cout << "M ";
		else cout << " ";
	mt_gotoXY (11, 70); sc_regGet (T, &flag);
		if ( flag == 1 ) cout << "T ";
		else cout << " ";
	mt_gotoXY (11, 72); sc_regGet (E, &flag);
		if ( flag == 1 ) cout << "E ";
		else cout << " ";

	printEl (icounter, 5, 68, -1, -1);
	printEl (accumulator, 2, 68, -1, -1);
	mt_gotoXY(8,68);
	int value,com=0,op=0;
	sc_memoryGet(icounter,&value);
	if(sc_commandDecode(value,&com,&op))
		printf("%2d : %2d",com,op);
	else
		printf("0 : 0");
}

void drawData ()
{
	isDrowNow = true;
	printMemory();
	printBigMemory (icounter, 14, 3, 2);

	int val;
	sc_regGet (M, &val);
	if (val)
	{
		if (icounter >= 0 && icounter < memSize)
			sc_regSet (M, 0);
	}

	drawHud ();
	mt_gotoXY (iorow, 1);
	isDrowNow = false;
}

void clearIO ()
{
	mt_gotoXY(iorow, 1);
	mt_clearLine ();
}

int checkWin ()
{
	int rows, cols;
	mt_getscreensize (rows, cols);
	if (rows < minRows || cols < minCols)
		return 0;
	return 1;
}
int input(char *msg,int *value,bool clr)
{
	if(clr)
		clearIO ();
	mt_cursorVisible (1);
	rk_mytermregime (1,0,1,1,1);
	int res = 0;
	std::cout  << msg;
	std::cin >> *value;
	if(std::cin.fail())
	{
		std::cout<<"Incorrect input type!";
		cin.clear();
		cin.ignore(INT_MAX,'\n');
		res = -1;
	} else{
		if(clr)
		  clearIO ();
	}
	rk_mytermregime (0,0,1,0,1);
	mt_cursorVisible (0);
	return res;
}
