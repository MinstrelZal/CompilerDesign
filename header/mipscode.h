#pragma once
const char order[31][8] = {"add    ", "addi   ", "addu   ", "addiu  ", "sub    ", 
                           "mult   ", "mfhi   ", "mflo   ", "div    ", "sll    ",
                           "sra    ", "slt    ", "slti   ", "beq    ", "bgez   ",
                           "bgtz   ", "blez   ", "bltz   ", "bne    ", "j      ",
                           "jr     ", "jal    ", "syscall", "sw     ", "lw     ", 
						   "sllv   ", "srav   ", "setlab ", "func   ", "lui    ",
                           "ori    "};

const char mipsreg[35][6] = {"$zero", "$at  ", "$v0  ", "$v1  ", "$a0  ",
                             "$a1  ", "$a2  ", "$a3  ", "$t0  ", "$t1  ",
                             "$t2  ", "$t3  ", "$t4  ", "$t5  ", "$t6  ",
                             "$t7  ", "$s0  ", "$s1  ", "$s2  ", "$s3  ",
                             "$s4  ", "$s5  ", "$s6  ", "$s7  ", "$t8  ",
                             "$t9  ", "$k0  ", "$k1  ", "$gp  ", "$sp  ",
                             "$fp  ", "$ra  ", "pc   ", "hi   ", "lo   "};

typedef struct mips{
	int ordernum;
	int rd;
	int rs;   // or s
	int rt;
	int offset;
	int base;
	int immediate;
	int target;  // or label id or funcname postion in tab
}mips;

extern mips mipstab[5000];
extern int mipsp;
extern int textbase;
extern int textpoint;

extern void mips1(int ordernum, int rd, int rs, int rt);
extern void mips2(int ordernum, int rt, int rs, int immediate);
extern void mips3(int ordernum, int rs, int rt);
extern void mips4(int ordernum, int rs, int rt, int offset);
extern void mips5(int ordernum, int rs, int offset);
extern void mips6(int ordernum, int rd);
extern void mips7(int ordernum, int rt, int base, int offset);
extern void mips8(int ordernum, int target);
extern void mips_jr(int ordernum, int rs);
extern void mips_syscall(int ordernum);
extern void mips_setlab(int ordernum, int target);
extern void mips_func(int ordernum, int target);
extern void mips_lui(int ordernum, int rt, int immediate);
extern void savesite();
extern void recoversite();

extern void emitmips();
extern void printmips(int mode);