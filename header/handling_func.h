#pragma once
extern void entertab(char id[ALNG + 1], objecttyp obj, types typ, int ref, int findex, int adr);
extern void enterftab();
extern void enteratab(int len, types tp);
extern int loc(char id[ALNG + 1], int findex);
extern void printtables();
extern void skip(int n);
extern void error(int n);
extern void errormsg();
extern void fatal(int n);