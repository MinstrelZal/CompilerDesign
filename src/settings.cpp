#include "stdafx.h"

char ch; // last character read from source program	
long long int inum; // integer from insymbol
int sleng; // string length
int cc; // character counter
int ll; // length of current line
int linenum; // line number
int errpos; // last error postion
int t, a, f, sx, e; // indices to tables: t: tab, a: atab, f: ftab, sx: stab, e: errortab
symbol sy; // last symbol read by insymbol
char token[ALNG+1]; // identifier from insymbol
int tkl; // length of token
char line[LLNG]; // one line source code
tabitem tab[TMAX]; // identifier table
atabitem atab[AMAX]; // array table
ftabitem ftab[FMAX]; // function table
char stab[SMAX]; // string table
erroritem errortab[ERMAX]; // error table

FILE *fpin; // the file to be compiled
FILE *fpout; // the result file