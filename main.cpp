#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <sys/time.h>
#include "computer.h"

using namespace std;

void init ()
{
	sc_memoryInit ();
	sc_comms_init ();
	sc_regInit ();
	icounter = 0;
	accumulator = 0;
}

void sghandler (int sign)
{
	switch (sign)
	{
		case SIGALRM:
			int val;
			sc_regGet (T, &val);
			if (!val)
			{
				cu();
				drawData ();
			}
			break;
		case SIGUSR1:
			init ();
			drawConsole ();
			drawData ();
			break;
		default: break;
	}

}
int main ()
{
	if (!checkWin ())
	{
		cout << "Too small window size!\n";
		return 0;
	}
	setvbuf (stdout, NULL, _IONBF, 0);
	struct itimerval nval, oval;
	signal (SIGALRM, sghandler);
	signal (SIGUSR1, sghandler);
	nval.it_interval.tv_sec = 1;
	nval.it_interval.tv_usec  = 0;
	nval.it_value.tv_sec = 1;
	nval.it_value.tv_usec = 0;
	enum keys key = K_UNDEF;
	char filename[fileNameLen];
	int value, comm, oper, reg, t_reg;
	drawConsole ();
	init ();
	drawData ();

	setitimer (ITIMER_REAL, &nval, &oval);
    rk_mytermsave();
    rk_mytermregime (0,0,1,0,1);
		sc_regSet(T,1);
    while (key != K_ESC)
    {
		sc_regGet (T, &t_reg);
		key = K_UNDEF;
		rk_readkey(key);
		switch (key)
		{
			case K_UP:
				icounter -= rowWidth;
				if (icounter < 0) icounter += memSize;
				drawData ();
				break;
			case K_DOWN:
				icounter += rowWidth;
				if (icounter >= memSize) icounter -= memSize;
				drawData ();
				break;
			case K_LEFT:
				icounter -= 1;
				if (icounter < 0) icounter += memSize;
				drawData ();
				break;
			case K_RIGHT:
				icounter += 1;
				if (icounter >= memSize) icounter -= memSize;
				drawData ();
				break;
			case K_F5:
				break;
			case K_F6:
				if (t_reg)
				{
					int num;
					clearIO ();
					mt_cursorVisible (1);
					rk_mytermregime (1,0,1,1,1);
					cout  << "Memory number: "; cin >> num; clearIO ();
					rk_mytermregime (0,0,1,0,1);
					mt_cursorVisible (0);
					clearIO ();
					if (cin.fail())
					{
						cout << "cin failed, it's not int";
						cin.clear();
						cin.ignore(INT_MAX,'\n');
						continue;
					}
					else icounter = num - 1;
					drawData ();
				}
				break;
			case K_ESC:
				break;
			case K_R:
				sc_regGet (T, &reg);
				if (reg) sc_regSet (T, 0);
				else sc_regSet (T, 1);
				drawData ();
				break;
			case K_I:
				raise (SIGUSR1);
				break;
			case K_Q:
				drawConsole ();
				drawData ();
				break;
			case K_ENTER:
				if (t_reg)
				{
					input("Command:",&comm,true);
					input("Operand:",&oper,true);
					if (cin.fail())
					{
						cout << "cin failed, it's not int";
						cin.clear();
						cin.ignore(INT_MAX,'\n');
						continue;
					}
					int value;
					sc_commandEncode(comm,oper,&value);
					sc_memorySet(icounter,value);
					drawData ();
				}
				break;

			case K_L:
				if (t_reg)
				{
					clearIO ();
					cout << "filename (load): ";
					mt_cursorVisible (1);
					rk_mytermregime (1,0,1,1,1);
					cin >> filename;
					rk_mytermregime (0,0,1,0,1);
					clearIO ();
					mt_cursorVisible (0);
					if (sc_memoryLoad (filename))
						cout << "memory load from " << filename;
					else cout << "memory don't load";
					drawData ();
				}
				break;

			case K_S:
				if (t_reg)
				{
					clearIO ();
					cout << "filename (save): ";
					mt_cursorVisible (1);
					rk_mytermregime (1,0,1,1,1);
					cin >> filename;
					rk_mytermregime (0,0,1,0,1);
					clearIO ();
					mt_cursorVisible (0);
					if (sc_memorySave (filename))
						cout << "memory saved in " << filename;
					else cout << "memory don't saved";
				}
				break;

			default: break;
		}
    }
    rk_mytermload ();
    mt_cursorVisible (1);

	return 0;
}
