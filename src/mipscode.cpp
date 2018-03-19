#include "stdafx.h"

mips mipstab[5000];
int mipsp;
int textbase = 0x00003000;
int textpoint = textbase;

// add, addu, sub, sll, sra, slt, sllv, srav
void mips1(int ordernum, int rd, int rs, int rt){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].rd = rd;
	mipstab[mipsp].rs = rs;
	mipstab[mipsp].rt = rt;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// addi, addiu, slti, ori
void mips2(int ordernum, int rt, int rs, int immediate){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].immediate = immediate;
	mipstab[mipsp].rs = rs;
	mipstab[mipsp].rt = rt;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// mult, div
void mips3(int ordernum, int rs, int rt){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].rs = rs;
	mipstab[mipsp].rt = rt;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// beq, bne
void mips4(int ordernum, int rs, int rt, int offset){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].offset = offset;
	mipstab[mipsp].rs = rs;
	mipstab[mipsp].rt = rt;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// bgez, bgtz, blez, bltz
void mips5(int ordernum, int rs, int offset){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].offset = offset;
	mipstab[mipsp].rs = rs;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// mfhi, mflo
void mips6(int ordernum, int rd){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].rd = rd;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// sw, lw
void mips7(int ordernum, int rt, int base, int offset){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].rt = rt;
	mipstab[mipsp].base = base;
	mipstab[mipsp].offset = offset;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// j, jal
void mips8(int ordernum, int target){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].target = target;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// jr
void mips_jr(int ordernum, int rs){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].rs = rs;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// syscall
void mips_syscall(int ordernum){
	mipstab[mipsp].ordernum = ordernum;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

// setlab
void mips_setlab(int ordernum, int target){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].target = target;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
}

void mips_func(int ordernum, int target){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].target = target;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
}

void mips_lui(int ordernum, int rt, int immediate){
	mipstab[mipsp].ordernum = ordernum;
	mipstab[mipsp].immediate = immediate;
	mipstab[mipsp].rt = rt;
	mipsp = mipsp + 1;
	if(mipsp == 5000){
		fatal(6);
	}
	textpoint = textpoint + 4;
}

void savesite(){
	int i;
	//int base;
	// addi $s0,$zero,immediate(base)
	//mips2(1, 16, 0, 4*stackt);
	for(i = 0; i < 8; i++){
		// sw $ti,offset($s7)
		mips7(23, 8+i, 23, 0);
		// addi $s7, $s7, 4
		mips2(1, 23, 23, 4);
	}
	// addi $s0,$zero,immediate(base)
	//mips2(1, 16, 0, 4*stack[stackb+2]);
	// sw $ra,offset,immediate(base)
	//mips7(23, 31, 22, 4);
	//stackt = stackt + 8;
}

void recoversite(){
	int i;
	//int base;
	// addi $s0,$zero,immediate(base)
	//mips2(1, 16, 0, 4*stackt);
	for(i = 0; i < 8; i++){
		// addi $s7, $s7, -4
		mips2(1, 23, 23, -4);
		// lw $ti,offset($s7)
		mips7(24, 15-i, 23, 0);
	}
	//stackt = stackt - 8;
}

void emitmips(){
	// $s7: 运行栈栈顶指针
	// $s6: 当前活动记录基址指针
	// $s5: 运行栈基址
	// $v1: 函数返回值
	int labelpos[100];
	bool in_condition = false;
	int condition_op = -1;
	int nowf = 0;
	int i;
	int funcbegin;
	int mainbegin = textbase;
	mipsp = 0;
	// lui $s5, 0x00001001
	mips_lui(29, 21, 0x00001001);
	// add $s6,$zero,$s5
	mips1(0, 22, 0, 21);
	// add $s7,$zero,$s5
	mips1(0, 23, 0, 21);
	// addi $s0, $zero, 4*ftab[0].vsize
	mips2(1, 16, 0, 4*ftab[0].vsize);
	// add $s7, $s0, $s7
	mips1(0, 23, 16, 23);
	// sw $s0, offset($s6)
	mips7(23, 16, 22, 12);
	for(i = 0; i < mc; i++){
		if(mctab[i].fctnum == 5){  // func, 更新tab表中函数的起始地址adr
			int ytpos;
			ytpos = loc(mctab[i].y.data.name, 0);
			tab[ytpos].adr = textpoint;
			nowf = tab[ytpos].ref;
			// set func begin label
			mips_func(28, ytpos);
			//if(strcmp(mctab[i].y.data.name, key[7]) != 0){
				savesite();
				// sw $ra,offset($s6)
				mips7(23, 31, 22, 4);
			//}
			/*else{
				// sw $s6,offset($s7)
				mips7(23, 22, 23, 8);
				// addi $s0,$zero,immediate
				char tempkey[ALNG + 1] = "main"; 
				int xtpos = loc(tempkey, 0);
				mips2(1, 16, 0, xtpos);
				// sw $s0,offset($s7)
				mips7(23, 16, 23, 16);

				// addi $s0,$zero,immediate
				mips2(1, 16, 0, 4*ftab[tab[xtpos].ref].vsize);
				// sw $s0,offset($s7)
				mips7(23, 16, 23, 12);
				
				// add $s6,$s7,$zero
				mips1(0, 22, 23, 0);
				// add $s7,$s7,$s0
				mips1(0, 23, 23, 16);
			}*/
		}
		if(mctab[i].fctnum == 8){  // =
			if(mctab[i].x.type == 1){
				int xtpos;
				xtpos = loc(mctab[i].x.data.name, nowf);
				if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].y.data.i);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*tab[xtpos].adr);
				}
				else if(mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $t0,offset(base)
					mips7(23, 8, base, 4*tab[xtpos].adr);
				}
				else if(mctab[i].y.type == 1){
					int ytpos;
					ytpos = loc(mctab[i].y.data.name, nowf);
					int base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*tab[xtpos].adr);
				}
			}
			else if(mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				if(mctab[i].y.type == 5){
					// sw $v1,offset($s6)
					mips7(23, 3, 22, 4*tab[xtpos].adr);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[xtpos].adr);
					int base = 23;
					// sw $t0,offset(base)
					mips7(23, 8, base, 0);
				}
			}
		}
		if(mctab[i].fctnum == 9){  // []=
			int xtpos = loc(mctab[i].x.data.name, nowf);
			if(mctab[i].y.type == 0){
				if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].z.data.i);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*(tab[xtpos].adr+mctab[i].y.data.i));
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					int base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset($base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*(tab[xtpos].adr+mctab[i].y.data.i));
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $t0,offset(base)
					mips7(23, 8, base, 4*(tab[xtpos].adr+mctab[i].y.data.i));
				}
			}
			else if(mctab[i].y.type == 1){
				int ytpos = loc(mctab[i].y.data.name, nowf);
				int base = (tab[ytpos].findex == 0) ? 21 : 22;
				// lw $s1,offset(base)
				mips7(24, 17, base, 4*tab[ytpos].adr);
				// addi $s2,$zero,4
				mips2(1, 18, 0, 4);
				// mult $s1,$s2
				mips3(5, 17, 18);
				// mflo $s1
				mips6(7, 17);
				// addi $s1,$s1,immediate
				mips2(1, 17, 17, 4*tab[xtpos].adr);
				// add $s1,$s1,$base
				base = (tab[xtpos].findex == 0) ? 21 : 22;
				mips1(0, 17, 17, base);
				if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].z.data.i);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sw $t0,offset($s1)
					mips7(23, 8, 17, 0);
				}
			}
			else if(mctab[i].y.type == 4){
				int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[ytpos].adr);
				// addi $s1,$zero,4
				mips2(1, 17, 0, 4);
				// mult $s1,$t0
				mips3(5, 17, 8);
				// mflo $s1
				mips6(7, 17);
				// addi $s1,$s1,immediate
				mips2(1, 17, 17, 4*tab[xtpos].adr);
				int base = (tab[xtpos].findex == 0) ? 21 : 22;
				// add $s1,$s1,$base
				mips1(0, 17, 17, base);
				if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].z.data.i);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sw $t0,offset($s1)
					mips7(23, 8, 17, 0);
				}
			}
		}
		else if(mctab[i].fctnum == 10){  // =[]
			int base;
			int ytpos = loc(mctab[i].y.data.name, nowf);
			if(mctab[i].x.type == 1){
				int xtpos = loc(mctab[i].x.data.name, nowf);
				if(mctab[i].z.type == 0){
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips2(24, 16, base, 4*(tab[ytpos].adr+mctab[i].z.data.i));
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// addi $s2,$zero,4
					mips2(1, 18, 0, 4);
					// mult $s0,$s2
					mips3(5, 16, 18);
					// mflo $s0
					mips6(7, 16);
					// addi $s0,$s0,immediate
					mips2(1, 16, 16, 4*tab[ytpos].adr);
					// add $s0,$s0,$base
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					mips1(0, 16, 16, base);
					// lw $s0,offset($s0)
					mips7(24, 16, 16, 0);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// addi $s0,$zero,4
					mips2(1, 16, 0, 4);
					// mult $s0,$t0
					mips3(5, 16, 8);
					// mflo $s0
					mips6(7, 16);
					// addi $s0,$s0,immediate
					mips2(1, 16, 16, 4*tab[ytpos].adr);
					// add $s0,$s0,$base
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					mips1(0, 16, 16, base);
					// lw $s0,offset($s0)
					mips7(24, 16, 16, 0);
				}
				base = (tab[xtpos].findex == 0) ? 21 : 22;
				// sw $s0,offset(base)
				mips7(23, 16, base, 4*tab[xtpos].adr);
			}
			else if(mctab[i].x.type == 4){
				int base;
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				if(mctab[i].z.type == 0){
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $t0,offset(base)
					mips7(24, 8, base, 4*(tab[ytpos].adr+mctab[i].z.data.i));
					// sw $t0,offset($s6)
					mips7(23, 8, 22, 4*tab[xtpos].adr);
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// addi $s1,$zero,4
					mips2(1, 17, 0, 4);
					// mult $s0,$s1
					mips3(5, 16, 17);
					// mflo $s0
					mips6(7, 16);
					// addi $s0,$s0,immediate
					mips2(1, 16, 16, 4*tab[ytpos].adr);
					// add $s0,$s0,base
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					mips1(0, 16, 16, base);
					// lw $t0,offset($s0)
					mips7(24, 8, 16, 0);
					// sw $t0,offset($s6)
					mips7(23, 8, 22, 4*tab[xtpos].adr);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// addi $s0,$zero,4
					mips2(1, 16, 0, 4);
					// mult $s0,$t0
					mips3(5, 16, 8);
					// mflo $s0
					mips6(7, 16);
					// addi $s0,$s0,immediate
					mips2(1, 16, 16, 4*tab[ytpos].adr);
					// add $s0,$s0,base
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					mips1(0, 16, 16, base);
					// lw $t0,offset($s0)
					mips7(24, 8, 16, 0);
					// sw $t0,offset($s6)
					mips7(23, 8, 22, 4*tab[xtpos].adr);
				}
			}
		}
		else if(mctab[i].fctnum == 11 || mctab[i].fctnum == 12){  // x / 
			int base;
			if(mctab[i].x.type == 1){
				int xtpos = loc(mctab[i].x.data.name, nowf);
				if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].y.data.i);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s2,$zero,immediate
						mips2(1, 18, 0, mctab[i].z.data.i);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$t0
							mips3(5, 16, 8);
						}
						else{
							// div $s0,$t0
							mips3(8, 16, 8);
						}
					}
					// mflo $s0
					mips6(7, 16);
				}
				else if(mctab[i].y.type == 1){
					int ytpos = loc(mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s2,$zero,immediate
						mips2(1, 18, 0, mctab[i].z.data.i);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$t0
							mips3(5, 16, 8);
						}
						else{
							// div $s0,$t0
							mips3(8, 16, 8);
						}
					}
					// mflo $s0
					mips6(7, 16);
				}
				else if(mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s0,$zero,immediate
						mips2(1, 16, 0, mctab[i].z.data.i);
						if(mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t1,offset($s6)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $t0,$t1
							mips3(5, 8, 9);
						}
						else{
							// div $t0,$t1
							mips3(8, 8, 9);
						}
					}
					// mflo $s0
					mips6(7, 16);
				}
				base = (tab[xtpos].findex == 0) ? 21 : 22;
				// sw $s0,offset(base)
				mips7(23, 16, base, 4*tab[xtpos].adr);
			}
			else if(mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].y.data.i);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s1,$zero,immediate
						mips2(1, 17, 0, mctab[i].z.data.i);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$t0
							mips3(5, 16, 8);
						}
						else{
							// div $s0,$t0
							mips3(8, 16, 8);
						}
					}
				}
				else if(mctab[i].y.type == 1){
					int ytpos = loc(mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s1,$zero,immediate
						mips2(1, 17, 0, mctab[i].z.data.i);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $s0,$t0
							mips3(5, 16, 8);
						}
						else{
							// div $s0,$t0
							mips3(8, 16, 8);
						}
					}
				}
				else if(mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s0,$zero,immediate
						mips2(1, 16, 0, mctab[i].z.data.i);
						if(mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t1,offset($s6)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 11){
							// mult $t0,$t1
							mips3(5, 8, 9);
						}
						else{
							// div $t0,$t1
							mips3(8, 8, 9);
						}
					}
				}
				// mflo $t0
				mips6(7, 8);
				// sw $t0,offset($s6)
				mips7(23, 8, 22, 4*tab[xtpos].adr);
			}
		}
		else if(mctab[i].fctnum == 13 || mctab[i].fctnum == 14){  // + - 
			int base;
			if(mctab[i].x.type == 1){
				int xtpos = loc(mctab[i].x.data.name, nowf);
				if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].y.data.i);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s0,$s0,immediate
						if(mctab[i].fctnum == 13){
							mips2(1, 16, 16, mctab[i].z.data.i);
						}
						else{
							mips2(1, 16, 16, -mctab[i].z.data.i);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $s0,$s0,$s2
							mips1(0, 16, 16, 18);
						}
						else{
							// sub $s0,$s0,$s2
							mips1(4, 16, 16, 18);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $s0,$s0,$t0
							mips1(0, 16, 16, 8);
						}
						else{
							// sub $s0,$s0,$t0
							mips1(4, 16, 16, 8);
						}
					}
				}
				else if(mctab[i].y.type == 1){
					int ytpos = loc(mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s0,$s0,immediate
						if(mctab[i].fctnum == 13){
							mips2(1, 16, 16, mctab[i].z.data.i);
						}
						else{
							mips2(1, 16, 16, -mctab[i].z.data.i);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $s0,$s0,$s2
							mips1(0, 16, 16, 18);
						}
						else{
							// sub $s0,$s0,$s2
							mips1(4, 16, 16, 18);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $s0,$s0,$t0
							mips1(0, 16, 16, 8);
						}
						else{
							// sub $s0,$s0,$t0
							mips1(4, 16, 16, 8);
						}
					}
				}
				else if(mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $s0,$t0,immediate
						if(mctab[i].fctnum == 13){
							mips2(1, 16, 8, mctab[i].z.data.i);
						}
						else{
							mips2(1, 16, 8, -mctab[i].z.data.i);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $s0,$t0,$s0
							mips1(0, 16, 8, 16);
						}
						else{
							// sub $s0,$t0,$s0
							mips1(4, 16, 8, 16);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t1,offset($s6)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $s0,$t0,$t1
							mips1(0, 16, 8, 9);
						}
						else{
							// sub $s0,$t0,$t1
							mips1(4, 16, 8, 9);
						}
					}
				}
				base = (tab[xtpos].findex == 0) ? 21 : 22;
				// sw $s0,offset(base)
				mips7(23, 16, base, 4*tab[xtpos].adr);
			}
			else if(mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].y.data.i);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $t0,$s0,immediate
						if(mctab[i].fctnum == 13){
							mips2(1, 8, 16, mctab[i].z.data.i);
						}
						else{
							mips2(1, 8, 16, -mctab[i].z.data.i);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $t0,$s0,$s1
							mips1(0, 8, 16, 17);
						}
						else{
							// sub $t0,$s0,$s1
							mips1(4, 8, 16, 17);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $t0,$s0,$t0
							mips1(0, 8, 16, 8);
						}
						else{
							// sub $t0,$s0,$t0
							mips1(4, 8, 16, 8);
						}
					}
				}
				else if(mctab[i].y.type == 1){
					int ytpos = loc(mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $t0,$s0,immediate
						if(mctab[i].fctnum == 13){
							mips2(1, 8, 16, mctab[i].z.data.i);
						}
						else{
							mips2(1, 8, 16, -mctab[i].z.data.i);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $t0,$s0,$s1
							mips1(0, 8, 16, 17);
						}
						else{
							// sub $t0,$s0,$s1
							mips1(4, 8, 16, 17);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $t0,$s0,$t0
							mips1(0, 8, 16, 8);
						}
						else{
							// sub $t0,$s0,$t0
							mips1(4, 8, 16, 8);
						}
					}
				}
				else if(mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
						// addi $t0,$t0,immediate
						if(mctab[i].fctnum == 13){
							mips2(1, 8, 8, mctab[i].z.data.i);
						}
						else{
							mips2(1, 8, 8, -mctab[i].z.data.i);
						}
					}
					else if(mctab[i].z.type == 1){
						int ztpos = loc(mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $t0,$t0,$s0
							mips1(0, 8, 8, 16);
						}
						else{
							// sub $t0,$t0,$s0
							mips1(4, 8, 8, 16);
						}
					}
					else if(mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
						// lw $t1,offset(base)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(mctab[i].fctnum == 13){
							// add $t0,$t0,$t1
							mips1(0, 8, 8, 9);
						}
						else{
							// sub $t0,$t0,$t1
							mips1(4, 8, 8, 9);
						}
					}
				}
				// sw $t0,offset($s6)
				mips7(23, 8, 22, 4*tab[xtpos].adr);
			}
		}
		else if(mctab[i].fctnum >= 15 && mctab[i].fctnum <= 20){
			in_condition = true;
			condition_op = mctab[i].fctnum;
			int base;
			int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
			if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
				// addi $s0,$zero,immediate
				mips2(1, 16, 0, mctab[i].y.data.i);
				if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
					// addi $t0,$s0,-immediate
					mips2(1, 8, 16, -mctab[i].z.data.i);
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s1,offset(base)
					mips7(24, 17, base, 4*tab[ztpos].adr);
					// sub $t0,$s0,$s1
					mips1(4, 8, 16, 17);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sub $t0,$s0,$t0
					mips1(4, 8, 16, 8);
				}
			}
			else if(mctab[i].y.type == 1){
				int ytpos = loc(mctab[i].y.data.name, nowf);
				base = (tab[ytpos].findex == 0) ? 21 : 22;
				// lw $s0,offset(base)
				mips7(24, 16, base, 4*tab[ytpos].adr);
				if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
					// addi $t0,$s0,-immediate
					mips2(1, 8, 16, -mctab[i].z.data.i);
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s1,offset(base)
					mips7(24, 17, base, 4*tab[ztpos].adr);
					// sub $t0,$s0,$s1
					mips1(4, 8, 16, 17);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sub $t0,$s0,$t0
					mips1(4, 8, 16, 8);
				}
			}
			else if(mctab[i].y.type == 4){
				int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
				//  lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[ytpos].adr);
				if(mctab[i].z.type == 0 || mctab[i].z.type == 6){
					// addi $t0,$t0,-immediate
					mips2(1, 8, 8, -mctab[i].z.data.i);
				}
				else if(mctab[i].z.type == 1){
					int ztpos = loc(mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// sub $t0,$t0,$s0
					mips1(4, 8, 8, 16);
				}
				else if(mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[mctab[i].z.data.i], nowf);
					// lw $t1,offset($s6)
					mips7(24, 9, 22, 4*tab[ztpos].adr);
					// sub $t0,$t0,$t1
					mips1(4, 8, 8, 9);
				}
			}
			// sw $t0,offset($s6)
			mips7(23, 8, 22, 4*tab[xtpos].adr);
		}
		else if(mctab[i].fctnum == 22){
			if(in_condition){
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[xtpos].adr);
				if(condition_op == 15){  // <  大于等于则转移
					// bgez $t0,label
					mips5(14, 8, mctab[i].y.data.i);
				}
				else if(condition_op == 16){  // <= 大于则转移
					// bgtz $t0,label
					mips5(15, 8, mctab[i].y.data.i);
				}
				else if(condition_op == 17){  // > 小于等于则转移
					// blez $t0,label
					mips5(16, 8, mctab[i].y.data.i);
				}
				else if(condition_op == 18){  // >= 小于则转移
					// bltz $t0,label
					mips5(17, 8, mctab[i].y.data.i);
				}
				else if(condition_op == 19){  // != 等于则转移
					// beq $t0,$zero,label
					mips4(13, 8, 0, mctab[i].y.data.i);
				}
				else if(condition_op == 20){  // == 不等于则转移
					// bne $t0,$zero,label
					mips4(18, 8, 0, mctab[i].y.data.i);
				}
				in_condition = false;
				condition_op = -1;
			}
			else{
				if(mctab[i].x.type == 0 || mctab[i].x.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, mctab[i].x.data.i);
					// beg $s0,$zero,label
					mips4(13, 16, 0, mctab[i].y.data.i);
				}
				else if(mctab[i].x.type == 1){
					int xtpos = loc(mctab[i].x.data.name, nowf);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[xtpos].adr);
					// beg $s0,$zero,label
					mips4(13, 16, 0, mctab[i].y.data.i);
				}
				else if(mctab[i].x.type == 4){
					// 等于0则转移
					int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[xtpos].adr);
					// beq $t0,$zero,label
					mips4(13, 8, 0, mctab[i].y.data.i);
				}
			}
		}
		else if(mctab[i].fctnum == 23){  // jmp
			// j target
			mips8(19, mctab[i].x.data.i);
		}
		else if(mctab[i].fctnum == 25){  // setlab
			// setlab target
			mips_setlab(27, mctab[i].x.data.i);
			labelpos[mctab[i].x.data.i] = textpoint;
		}
		else if(mctab[i].fctnum == 26){  // return
			if(mctab[i].x.type == 3){
				recoversite();
				// add $s7,$s6,$zero
				mips1(0, 23, 22, 0);
				//stackt = stackb;
				// lw $s6,offset($s7)
				mips7(24, 22, 23, 8);
				//stackb = stack[stackb+2];
				// jr $ra
				mips_jr(20, 31);
			}
			else{
				if(mctab[i].y.type == 0 || mctab[i].y.type == 6){
					// addi $v1,$zero,0
					mips2(1, 3, 0, mctab[i].y.data.i);
					// sw $v1,offset($s6)
					mips7(23, 3, 22, 0);
				}
				else if(mctab[i].y.type == 1){
					int ytpos = loc(mctab[i].y.data.name, nowf);
					int base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $v1,offset(base)
					mips7(24, 3, base, 4*tab[ytpos].adr);
					// sw $v1,offset($s6)
					mips7(23, 3, 22, 0);
				}
				else if(mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					// add $v1,$t0,$zero
					mips1(0, 3, 8, 0);
					// sw $v1,offset($s6)
					mips7(23, 3, 22, 0);
				}
				recoversite();
				// add $s7,$s6,$zero
				mips1(0, 23, 22, 0);
				//stackt = stackb;
				// lw $s6,offset($s7)
				mips7(24, 22, 23, 8);
				// sw $v1, offset($s7)
				mips7(23, 3, 23, 0);
				// jr $ra
				mips_jr(20, 31);
			}
		}
		else if(mctab[i].fctnum == 27){  // call
			int xtpos = loc(mctab[i].x.data.name, 0);
			if(ftab[tab[xtpos].ref].psize > 5){
				// addi $s7,$s7,immediate
				mips2(1, 23, 23, -4*ftab[tab[xtpos].ref].psize);
			}
			// sw $s6,offset($s7)
			mips7(23, 22, 23, 8);
			// addi $s0,$zero,immediate
			mips2(1, 16, 0, xtpos);
			// sw $s0,offset($s7)
			mips7(23, 16, 23, 16);

			// addi $s0,$zero,immediate
			mips2(1, 16, 0, 4*ftab[tab[xtpos].ref].vsize);
			// sw $s0,offset($s7)
			mips7(23, 16, 23, 12);
			
			// add $s6,$s7,$zero
			mips1(0, 22, 23, 0);
			// add $s7,$s7,$s0
			mips1(0, 23, 23, 16);

			// jal target
			mips8(21, xtpos);
			// lw $ra,offset($s6)
			mips7(24, 31, 22, 4);
		}
		else if(mctab[i].fctnum == 28){  // read
			int xtpos = loc(mctab[i].x.data.name, nowf);
			if(tab[xtpos].typ == ints){
				// addiu $v0,$zero,5
				mips2(3, 2, 0, 5);
				// syscall
				mips_syscall(22);
				int base = (tab[xtpos].findex == 0) ? 21 : 22;
				// sw $v0,offset(base)
				mips7(23, 2, base, 4*tab[xtpos].adr);
			}
			else if(tab[xtpos].typ == chars){
				// addiu $v0,$zero,12
				mips2(3, 2, 0, 12);
				// syscall
				mips_syscall(22);
				int base = (tab[xtpos].findex == 0) ? 21 : 22;
				// sw $v0,offset(base)
				mips7(23, 2, base, 4*tab[xtpos].adr);
				// addiu $v0,$zero,12
				//mips2(3, 2, 0, 12);
				// syscall
				//mips_syscall(22);
			}
		}
		else if(mctab[i].fctnum == 29){  // write
			if(mctab[i].y.type == 7){
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[xtpos].adr);
				// addiu $v0,$zero,11
				mips2(3, 2, 0, 11);
				// add $a0,$zero,$t0
				mips1(0, 4, 0, 8);
			}
			else{
				if(mctab[i].x.type == 6){
					// addiu $v0,$zero,11
					mips2(3, 2, 0, 11);
					// addi $a0,$zero,immediate
					mips2(1, 4, 0, mctab[i].x.data.i);
				}
				else if(mctab[i].x.type == 0){
					// addiu $v0,$zero,1
					mips2(3, 2, 0, 1);
					// addi $a0,$zero,immediate
					mips2(1, 4, 0, mctab[i].x.data.i);
				}
				else if(mctab[i].x.type == 1){
					int xtpos = loc(mctab[i].x.data.name, nowf);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					if(tab[xtpos].typ == ints){
						// addiu $v0,$zero,1
						mips2(3, 2, 0, 1);
					}
					else if(tab[xtpos].typ == chars){
						// addiu $v0,$zero,11
						mips2(3, 2, 0, 11);
					}
					// lw $a0,offset(base)
					mips7(24, 4, base, 4*tab[xtpos].adr);
				}
				else if(mctab[i].x.type == 4){
					int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[xtpos].adr);
					// addiu $v0,$zero,1
					mips2(3, 2, 0, 1);
					// add $a0,$zero,$t0
					mips1(0, 4, 0, 8);
				}

			}
			// syscall
			mips_syscall(22);
		}
		else if(mctab[i].fctnum == 30){  // push
			int base;
			if(mctab[i].x.type == 0 || mctab[i].x.type == 6){
				// addi $s0,$zero,immediate
				mips2(1, 16, 0, mctab[i].x.data.i);
			}
			else if(mctab[i].x.type == 1){
				int xtpos = loc(mctab[i].x.data.name, nowf);
				base = (tab[xtpos].findex == 0) ? 21 : 22;
				// lw $s0,offset(base)
				mips7(24, 16, base, 4*tab[xtpos].adr);
			}
			else if(mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[mctab[i].x.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[xtpos].adr);
				// add $s0,$zero,$t0
				mips1(0, 16, 0, 8);
			}
			int ztpos = loc(mctab[i].z.data.name, tab[mctab[i].y.data.i].ref);
			//fprintf(fpout,"nowf = %d, ztpos = %d ", nowf, ztpos);
			//exit(0);
			base = (tab[ztpos].findex == 0) ? 21 : 23;
			//fprintf(fpout,"base = %d, stackt = %d\n", base, stackt);
			if(tab[ztpos].adr == 5){
				// addi $s7,$s7,20
				mips2(1, 23, 23, 20);
			}
			// sw $s0,offset(base)
			//mips7(23, 16, base, 4*tab[ztpos].adr);
			mips7(23, 16, 23, 0);
			// addi $s7,$s7,4
			mips2(1, 23, 23, 4);
		}
	}
}

void printmips(int mode){
	//fprintf(mipsfp, "\n\nmipstab:\n");
	//printf("\n\mipsftab:\n");
	FILE *mipsfp;
	if(mode == 0){
		mipsfp = fopen("15061075_张安澜_优化前目标代码.txt","w");
	}
	else{
		mipsfp = fopen("15061075_张安澜_优化后目标代码.txt","w");
	}
	int i;
	for(i = 0; i < mipsp; i++){
		if(mipstab[i].ordernum == 0 || mipstab[i].ordernum == 2 || mipstab[i].ordernum == 4){
			fprintf(mipsfp, "%s %s, %s, %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rd], mipsreg[mipstab[i].rs], mipsreg[mipstab[i].rt]);
			// printf("%s %s, %s, %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rd], mipsreg[mipstab[i].rs], mipsreg[mipstab[i].rt]);
		}
		else if(mipstab[i].ordernum == 1 || mipstab[i].ordernum == 3){
			fprintf(mipsfp, "%s %s, %s, %d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rt], mipsreg[mipstab[i].rs], mipstab[i].immediate);
			// printf("%s %s, %s, %d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rt], mipsreg[mipstab[i].rs], mipstab[i].immediate);
		}
		else if(mipstab[i].ordernum == 5 || mipstab[i].ordernum == 8){
			fprintf(mipsfp, "%s %s, %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs], mipsreg[mipstab[i].rt]);
			// printf("%s %s, %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs], mipsreg[mipstab[i].rt]);
		}
		else if(mipstab[i].ordernum == 6 || mipstab[i].ordernum == 7){
			fprintf(mipsfp, "%s %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rd]);
			// printf("%s %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rd]);
		}
		else if(mipstab[i].ordernum == 13 || mipstab[i].ordernum == 18){
			fprintf(mipsfp, "%s %s, %s, label%d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs], mipsreg[mipstab[i].rt], mipstab[i].offset);
			// printf("%s %s, %s, label%d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs], mipsreg[mipstab[i].rt], mipstab[i].offset);
		}
		else if(mipstab[i].ordernum >= 14 && mipstab[i].ordernum <= 17){
			fprintf(mipsfp, "%s %s, label%d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs], mipstab[i].offset);
			// printf("%s %s, label%d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs], mipstab[i].offset);
		}
		else if(mipstab[i].ordernum == 19){
			fprintf(mipsfp, "%s label%d\n",order[mipstab[i].ordernum], mipstab[i].target);
			// printf("%s label%d\n",order[mipstab[i].ordernum], mipstab[i].target);
		}
		else if(mipstab[i].ordernum == 20){
			fprintf(mipsfp, "%s %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs]);
			// printf("%s %s\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rs]);
		}
		else if(mipstab[i].ordernum == 21){
			fprintf(mipsfp, "%s %s\n",order[mipstab[i].ordernum], tab[mipstab[i].target].name);
			// printf("%s %s\n",order[mipstab[i].ordernum], tab[mipstab[i].target].name);
		}
		else if(mipstab[i].ordernum == 22){
			fprintf(mipsfp, "%s\n",order[mipstab[i].ordernum]);
			// printf("%s\n",order[mipstab[i].ordernum]);
		}
		else if(mipstab[i].ordernum == 23 || mipstab[i].ordernum == 24){
			fprintf(mipsfp, "%s %s, %d(%s)\n", order[mipstab[i].ordernum], mipsreg[mipstab[i].rt], mipstab[i].offset, mipsreg[mipstab[i].base]);
			// printf("%s %s, %d(%s)\n", order[mipstab[i].ordernum], mipsreg[mipstab[i].rt], mipstab[i].offset, mipsreg[mipstab[i].base]);
		}
		else if(mipstab[i].ordernum == 27){
			fprintf(mipsfp, "label%d:\n", mipstab[i].target);
			// printf("label%d:\n", mipstab[i].target);
		}
		else if(mipstab[i].ordernum == 28){
			fprintf(mipsfp, "%s:\n", tab[mipstab[i].target].name);
			// printf("%s:\n", tab[mipstab[i].target].name);
		}
		else if(mipstab[i].ordernum == 29){
			fprintf(mipsfp, "%s %s, %d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rt], mipstab[i].immediate);
			// printf("%s %s, %d\n",order[mipstab[i].ordernum], mipsreg[mipstab[i].rt], mipstab[i].immediate);
		}
	}
	if(mode == 0){
		printf("\nmips code line = %d\n", i);
	}
	else{
		printf("\nopt mips code line = %d\n", i);
	}
	fclose(mipsfp);
}