#pragma once

union data{
	int i;
	char name[ALNG + 1];
};

typedef struct mcfield{
	int type;  // 0: int, 1:char[], 2: fct, 3:not exist, 4:tempterm, 5:RET, 6:char, 7:chararrays
	union data data;
}mcfield;

typedef struct mctabitem{
	int fctnum;
	mcfield x;
	mcfield y;
	mcfield z;
	int blocknum;
}mctabitem;

extern char zaltemp[10][ALNG + 1];

const char fct[31][7] = {"const ", "var   ", "int   ", "char  ", "void  ", 
	                     "func  ", "para  ", "()    ", "=     ", "[]=   ", 
					     "=[]   ", "*     ", "/     ", "+     ", "-     ", 
					     "<     ", "<=    ", ">     ", ">=    ", "!=    ", 
					     "==    ", "bnz   ", "bz    ", "jmp   ", "genlab", 
					     "setlab", "return", "call  ", "read  ", "write ",
                         "push  "};

const char retreg[ALNG + 1] = "RET";

extern mctabitem mctab[MCMAX];
extern int mc;
extern bool reginuse[10];

extern void inittempreg();
extern int regid();
extern void emitmidcode(int fctnum, mcfield x, mcfield y, mcfield z);
extern void printmctab();