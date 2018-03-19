#pragma once
extern void block();
extern void optimize_dag();
extern void printoptmctab();
extern void emitoptmips();
extern void constdelete(int mode);
extern void peephole();