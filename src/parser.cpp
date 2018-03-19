#include "stdafx.h"
void term();
void factor();
void callretstatement();
void statement();

int labelnum = 0;
int tpos;
int dx;
int fctnum;
mcfield x;
mcfield y;
mcfield z;
mcfield tempitem;
types switchtyp;
mcfield switchexpr;
int caseendmc[CSMAX];
int casenum;
bool is_array = false;
types arraytyp = notyp;
bool hasend = false;

// ���������壾 ::= int����ʶ��������������{,����ʶ��������������}
//                  |char����ʶ���������ַ���{,����ʶ���������ַ���}
void constdef(){
	types tp;
	insymbol();
	if(sy != intsy && sy != charsy){
		error(1);
	}
	if(sy == intsy || sy == charsy){
		tp = (sy == intsy) ? ints : chars;
		insymbol();
		if(sy != idsy){
			error(2);
		}
		if(sy == idsy){
			insymbol();
			if(sy != assignsy){
				error(3);
			}
			if(sy == assignsy){
				insymbol();
				int sign = 1;
				if(sy == plussy || sy == minussy){
					sign = (sy == plussy) ? 1 : -1; 
					insymbol();
				}
				if(sy != intcon && sy != charcon){
					error(4);
				}
				if((tp == ints && sy == charcon) || (tp == chars && sy == intcon)){
					error(5);
				}
				if(sy == intcon || sy == charcon){
					inum = inum * sign;
					entertab(token, konstant, tp, 0, f, inum);
					fctnum = 0; // const
					x.type = 2;
					x.data.i = (sy == intcon) ? 2 : 3; // int/char
					y.type = 1;
					strcpy(y.data.name, token); // identifier
					z.type = (sy == intcon) ? 0 : 6;
					z.data.i = inum; // value
					emitmidcode(fctnum, x, y, z);
					insymbol();
				}
			}
		}
		fprintf(fpout, "this is a constant defination\n");
		// printf("this is a constant defination\n");
		while(sy == commasy){
			insymbol();
			if(sy != idsy){
				error(2);
			}
			if(sy == idsy){
				insymbol();
				if(sy != assignsy){
					error(3);
				}
				if(sy == assignsy){
					insymbol();
					int sign = 1;
					if(sy == plussy || sy == minussy){
						sign = (sy == plussy) ? 1 : -1; 
						insymbol();
					}
					if(sy != intcon && sy != charcon){
						error(4);
					}
					if((tp == ints && sy == charcon) || (tp == chars && sy == intcon)){
						error(5);
					}
					if(sy == intcon || sy == charcon){
						inum = inum * sign;
						entertab(token, konstant, tp, 0, f, inum);
						fctnum = 0; // const
						x.type = 2;
						x.data.i = (sy == intcon) ? 2 : 3; // int/char
						y.type = 1;
						strcpy(y.data.name, token); // identifier
						z.type = (sy == intcon) ? 0 : 6;
						z.data.i = inum; // value
						emitmidcode(fctnum, x, y, z);
						insymbol();
					}
				}
			}
		}
	}
}

// ������˵���� ::= const���������壾;{ const���������壾;}
void constdeclaration(){
	if(sy == constsy){
		while(sy == constsy){
			constdef();
			if(sy != semicolonsy){
				error(6);
			}
			if(sy == semicolonsy){
				insymbol();
			}
		}
		fprintf(fpout, "this is constant declaration\n");
		// printf("this is constant declaration\n");
	}
}

// ���������壾 ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]��)}
// �����ͱ�ʶ���� ::= int|char
void vardef(){
	types tp;
	if(sy == intsy || sy == charsy){
		tp = (sy == intsy) ? ints : chars;
		do{
			insymbol();
			if(sy != idsy){
				error(2);
			}
			if(sy == idsy){
				char tempid[ALNG+1];
				strcpy(tempid, token);
				insymbol();
				if(sy == lbracketsy){
					insymbol();
					if(sy != intcon || (sy == intcon && inum <= 0)){ // ���ܻ���bug
						error(7);
						inum = 0;
					}
					//if(sy == intcon){
						// ��¼���ű������������Ϣ��, adr
						enteratab(inum, tp);
						entertab(token, vvariable, (tp==ints)?intarrays:chararrays, a, f, dx);
						dx = dx + inum;
						fctnum = 1; // var
						x.type = 2;
						x.data.i = (tp == ints) ? 2 : 3; // int/char
						y.type = 1;
						strcpy(y.data.name, token); // identifier
						z.type = 0;
						z.data.i = inum; // length of array
						emitmidcode(fctnum, x, y, z);
						insymbol();
						if(sy != rbracketsy){
							error(8);
						}
						if(sy == rbracketsy){
							insymbol();
						}
					//}
				}
				else{
					// ��¼���ű�, adr��Ҫ����ȷ��
					entertab(tempid, vvariable, tp, 0, f, dx);
					dx = dx + 1;
					fctnum = 1; // var
					x.type = 2;
					x.data.i = (tp == ints) ? 2 : 3; // int/char
					y.type = 1;
					strcpy(y.data.name, tempid); // identifier
					z.type = 3;
					emitmidcode(fctnum, x, y, z);
				}
			}
			// else
		}while(sy == commasy);
		if(sy != semicolonsy){
			error(41);
		}
		fprintf(fpout, "this is a var defination\n");
		// printf("this is a var defination\n");
	}
}

// ������˵���� ::= ���������壾;{���������壾;}
void vardeclaration(){
	vardef();
	if(sy == semicolonsy){
		insymbol();
	}
	while(sy == intsy || sy == charsy){
		vardef();
		if(sy == semicolonsy){
			insymbol();
		}
	}
	/*do{
		insymbol();
		vardef();
	}while(sy == semicolonsy);*/
	ftab[f].vsize = dx;
	fprintf(fpout, "this is var declaration\n");
	// printf("this is var declaration\n");
}

// �����ʽ�� ::=�ۣ������ݣ��{���ӷ�����������}
void expression(){
	switchtyp = notyp;
	int tempfct;
	insymbol();
	if(sy == plussy || sy == minussy){
		tempfct = (sy == minussy) ? 14 : 13;
		insymbol();
		term();
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		y.type = 0;
		y.data.i = 0;
		z = tempitem;
		int reg = regid();
		int zaltemppos = loc(zaltemp[reg],f);
		if(zaltemppos == 0){
			entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
			dx = dx + 1;
		}
		x.type = 4;
		x.data.i = reg;
		emitmidcode(tempfct, x, y, z); // x��ֵ
		tempitem = x;
		if(y.type == 4){
			reginuse[y.data.i] = false;
		}
		if(z.type == 4){
			reginuse[z.data.i] = false;
		}
		switchtyp = ints;
	}
	else{
		term();
	}
	if(sy == plussy || sy == minussy){
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		while(sy == plussy || sy == minussy){
			mcfield opnum;
			tempfct = (sy == plussy) ? 13 : 14;
			opnum = tempitem;
			insymbol();
			term();
			y = opnum;
			z = tempitem;
			int reg = regid();
			int zaltemppos = loc(zaltemp[reg],f);
			if(zaltemppos == 0){
				entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
				dx = dx + 1;
			}
			x.type = 4;
			x.data.i = reg;
			emitmidcode(tempfct, x, y, z);  // x��ֵ
			tempitem = x;
			if(y.type == 4){
				reginuse[y.data.i] = false;
			}
			if(z.type == 4){
				reginuse[z.data.i] = false;
			}
		}
		switchtyp = ints;
	}
	fprintf(fpout, "this is expression\n");
	// printf("this is expression\n");
}

// ��� ::= �����ӣ�{���˷�������������ӣ�}
void term(){
	factor();
	int tempfct;
	mcfield opnum;
	if(sy == multisy || sy == divisy){
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		while(sy == multisy || sy == divisy){
			tempfct = (sy == multisy) ? 11 : 12;
			opnum = tempitem;
			insymbol();
			factor();
			y = opnum;
			z = tempitem;
			int reg = regid();
			int zaltemppos = loc(zaltemp[reg],f);
			if(zaltemppos == 0){
				entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
				dx = dx + 1;
			}
			x.type = 4;
			x.data.i = reg;
			emitmidcode(tempfct, x, y, z);  // x��ȡֵ
			tempitem = x;
			if(y.type == 4){
				reginuse[y.data.i] = false;
			}
			if(z.type == 4){
				reginuse[z.data.i] = false;
			}
		}
		switchtyp = ints;
	}
	fprintf(fpout, "this is term\n");
	// printf("this is term\n");
}

// �����ӣ� ::= ����ʶ����������ʶ������[�������ʽ����]��|��(�������ʽ����)������������|���ַ��������з���ֵ����������䣾
void factor(){
	if(sy != idsy && sy != lparentsy && sy != intcon && sy != charcon){
		error(9);
	}
	if(sy == idsy){
		// ����ű��ر�ʶ����λ��
		tpos = loc(token,f);
		if(tpos == 0){  // ���ܻ���Bug
			while(!(sy == plussy || sy == minussy || sy == multisy || sy == divisy || 
				    sy == lesssy || sy == leoeqsy || sy == gresy || sy == groeqsy || sy == eqsy || sy == neqsy ||
					sy == semicolonsy || sy == rparentsy)){
				insymbol();
			}
			return ;
		}
		if(tab[tpos].obj == konstant){
			switchtyp = tab[tpos].typ;
			tempitem.type = (tab[tpos].typ == ints) ? 0 : 6;
			tempitem.data.i = tab[tpos].adr;
			insymbol();
		}
		else if(tab[tpos].obj == vvariable){
			if(tab[tpos].typ == ints || tab[tpos].typ == chars){
				switchtyp = tab[tpos].typ;
				tempitem.type = 1;
				strcpy(tempitem.data.name, token);
				insymbol();
				if(sy == lbracketsy){  // ���ܻ���bug
					error(40);
					while(sy != rbracketsy){
						insymbol();
					}
					insymbol();
				}
			}
			else if(tab[tpos].typ == intarrays || tab[tpos].typ == chararrays){
				switchtyp = (tab[tpos].typ == intarrays) ? ints : chars;
				int alen = atab[tab[tpos].ref].len;
				is_array = true;
				arraytyp = tab[tpos].typ;
				char arrayname[ALNG + 1];
				strcpy(arrayname, token);
				insymbol();
				if(sy != lbracketsy){
					error(10);
				}
				bool tempis_array = is_array;
				types temparraytyp = arraytyp;
				expression();
				if(tempitem.type == 0 || tempitem.type == 6){
					if(tempitem.data.i >= alen || tempitem.data.i < 0){
						error(11);
					}
				}
				if(is_array){
					is_array = false;
					arraytyp = notyp;
				}
				is_array = tempis_array;
				arraytyp = temparraytyp;
				z = tempitem;
				fctnum = 10;  // =[]
				y.type = 1;
				strcpy(y.data.name, arrayname);
				int reg = regid();
				int zaltemppos = loc(zaltemp[reg],f);
				if(zaltemppos == 0){
					entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
					dx = dx + 1;
				}
				x.type = 4;
				x.data.i = reg;
				emitmidcode(fctnum, x, y, z); // x��ֵ
				tempitem = x;
				if(y.type == 4){
					reginuse[y.data.i] = false;
				}
				if(z.type == 4){
					reginuse[z.data.i] = false;
				}
				if(sy != rbracketsy){
					error(8);
				}
				if(sy == rbracketsy){
					insymbol();
				}
			}
		}
		else if(tab[tpos].obj == funktion){ // tab[tpos].obj == funktion
			if(tab[tpos].typ != ints && tab[tpos].typ != chars){
				error(12);
			}
			switchtyp = tab[tpos].typ;
			callretstatement();
			fctnum = 8; // =
			int reg = regid();
			int zaltemppos = loc(zaltemp[reg],f);
			if(zaltemppos == 0){
				entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
				dx = dx + 1;
			}
			x.type = 4;
			x.data.i = reg;
			y.type = 5;
			strcpy(y.data.name, retreg);  // RET
			z.type = 3;
			emitmidcode(fctnum, x, y, z); // x, y��ֵ
			tempitem = x;
		}
		fprintf(fpout, "this is factor\n");
		// printf("this is factor\n");
	}
	else if(sy == lparentsy){
		expression();
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){
			insymbol();
		}
		fprintf(fpout, "this is factor\n");
		// printf("this is factor\n");
	}
	else if(sy == intcon || sy == charcon){
		// ������Ԫʽ
		switchtyp = (sy == intcon) ? ints : chars;
		tempitem.type = (sy == intcon) ? 0 : 6;
		tempitem.data.i = inum;
		insymbol();
		fprintf(fpout, "this is factor\n");
		// printf("this is factor\n");
	}
}

// ��ֵ������ ::= �����ʽ��{,�����ʽ��}
void callpara(char funcname[ALNG + 1]){
	int ftpos = loc(funcname, 0);
	int parabegin = ftab[tab[ftpos].ref].lastpar;
	int paranum = ftab[tab[ftpos].ref].psize - 5;
	if(paranum == 0){
		error(14);
	}
	while(tab[parabegin].link != 0){
		parabegin = tab[parabegin].link;
	}
	// inittempreg();
	expression();
	paranum = paranum - 1;
	fctnum = 30;  // push
	x = tempitem;
	if(is_array){
		if(arraytyp == chararrays){
			y.type = 7;
		}
		is_array = false;
		arraytyp = notyp;
	}
	else{
		y.type = 3;
	}
	y.data.i = ftpos;
	z.type = 1;
	strcpy(z.data.name, tab[parabegin].name);
	parabegin = parabegin + 1;
	emitmidcode(fctnum, x, y, z);
	if(x.type == 4){
		reginuse[x.data.i] = false;
	}
	while(sy == commasy){
		// inittempreg();
		expression();
		paranum = paranum - 1;
		fctnum = 30;  // push
		x = tempitem;
		if(is_array){
			if(arraytyp == chararrays){
				y.type = 7;
			}
			is_array = false;
			arraytyp = notyp;
		}
		else{
			y.type = 3;
		}
		y.data.i = ftpos;
		z.type = 1;
		strcpy(z.data.name, tab[parabegin].name);
		parabegin = parabegin + 1;
		emitmidcode(fctnum, x, y, z);
		if(x.type == 4){
			reginuse[x.data.i] = false;
		}
	}
	if(paranum > 0){
		error(16);
	}
	else if(paranum < 0){
		error(15);
	}
	fprintf(fpout, "this is call parameter\n");
	// printf("this is call parameter\n");
}

// ���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��|<��ʶ��>
void callretstatement(){
	char funcname[ALNG + 1];
	strcpy(funcname, token);
	int funcpos = loc(funcname, 0);  // ���ܻ���bug
	bool haspara = true;
	if(ftab[tab[funcpos].ref].lastpar == funcpos){
		haspara = false;
	}
	bool enterpara = false;
	insymbol();
	if(sy == lparentsy){
		enterpara = true;
		callpara(funcname);
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){
			insymbol();
		}
	}
	if(!((haspara && enterpara) || (!haspara && !enterpara))){
		error(42);
	}
	fctnum = 27;  // call
	x.type = 1;
	strcpy(x.data.name, funcname);
	y.type = 3;
	z.type = 3;
	emitmidcode(fctnum, x, y, z);
	fprintf(fpout, "this is return function call\n");
	// printf("this is return function call\n");
}

// ���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��|<��ʶ��>
void callnonstatement(){
	char funcname[ALNG + 1];
	strcpy(funcname, token);
	int funcpos = loc(funcname, 0);  // ���ܻ���bug
	bool haspara = true;
	if(ftab[tab[funcpos].ref].lastpar == funcpos){
		haspara = false;
	}
	bool enterpara = false;
	insymbol();
	if(sy == lparentsy){
		enterpara = true;
		callpara(funcname);
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){
			insymbol();
		}
	}
	if(!((haspara && enterpara) || (!haspara && !enterpara))){
		error(42);
	}
	fctnum = 27;  // call
	x.type = 1;
	strcpy(x.data.name, funcname);
	y.type = 3;
	z.type = 3;
	emitmidcode(fctnum, x, y, z);
	fprintf(fpout, "this is no return function call\n");
	// printf("this is no return function call\n");
}

// �������� ::= �����ʽ������ϵ������������ʽ���������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
void condition(){
	int tempfct;
	mcfield expr1;
	// inittempreg();
	expression();
	if(is_array){
		is_array = false;
		arraytyp = notyp;
	}
	if(sy == lesssy || sy == leoeqsy || sy == gresy || sy == groeqsy || sy == neqsy || sy == eqsy){
		if(sy == lesssy){
			tempfct = 15;
		}
		else if(sy == leoeqsy){
			tempfct = 16;
		}
		else if(sy == gresy){
			tempfct = 17;
		}
		else if(sy == groeqsy){
			tempfct = 18;
		}
		else if(sy == neqsy){
			tempfct = 19;
		}
		else if(sy == eqsy){
			tempfct = 20;
		}
		expr1 = tempitem;
		// inittempreg();
		expression();
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		z = tempitem;
		y = expr1;
		int reg = regid();
		int zaltemppos = loc(zaltemp[reg],f);
		if(zaltemppos == 0){
			entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
			dx = dx + 1;
		}
		x.type = 4;
		x.data.i = reg;
		emitmidcode(tempfct, x, y, z); // x��ֵ
		tempitem = x;
		if(y.type == 4){
			reginuse[y.data.i] = false;
		}
		if(z.type == 4){
			reginuse[z.data.i] = false;
		}
	}
	fprintf(fpout, "this is condition\n");
	// printf("this is condition\n");
}

// ��������䣾::= if��(������������)������䣾else����䣾
void ifstatement(){
	int elsebeginlab = 0;
	int elseendlab = 0;
	int tempmc;
	insymbol();
	if(sy != lparentsy){
		error(17);
	}
	// if(sy == lparentsy){
		condition();
		elsebeginlab = labelnum++;
		tempmc = mc;
		fctnum = 22; // bz
		x = tempitem;
		y.type = 0;
		//y.data.i = 0;  // label
		y.data.i = elsebeginlab;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		if(x.type == 4){
			reginuse[x.data.i] = false;
		}
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){  // ���ܻ���bug
			insymbol();
		}
		statement();
	//}  // ���ܻ���bug
			elseendlab = labelnum++;
			fctnum = 23;  // jmp
			x.type = 0;
			x.data.i = elseendlab;
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			fctnum = 25;  // setlab
			x.type = 0;
			x.data.i = elsebeginlab;
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			if(sy != elsesy){
				 error(18);
			}
			if(sy == elsesy){
				insymbol();
				statement();
				fctnum = 25;  // setlab
				x.type = 0;
				x.data.i = elseendlab;
				y.type = 3;
				z.type = 3;
				emitmidcode(fctnum, x, y, z);
			}
		//}
		fprintf(fpout, "this is if statement\n");
		// printf("this is if statement\n");
	//}
}

// ��ѭ����䣾 ::= while��(������������)������䣾
void whilestatement(){
	int whilebeginlab;
	int whileendlab;
	int tempmc;
	insymbol();
	if(sy != lparentsy){
		error(17);
	}
	// if(sy == lparentsy){  // ���ܻ���bug
		whilebeginlab = labelnum++;
		fctnum = 25; // setlab
		x.type = 0;
		x.data.i = whilebeginlab;
		y.type = 3;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		condition();
		whileendlab = labelnum++;
		tempmc = mc;
		fctnum = 22; // bz
		y.type = 0;
		y.data.i = whileendlab;  // label
		x = tempitem;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		if(x.type == 4){
			reginuse[x.data.i] = false;
		}
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){
			insymbol();
			statement();
			fctnum = 23;  // jmp
			x.type = 0;
			x.data.i = whilebeginlab;
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			fctnum = 25;  // setlab
			x.type = 0;
			x.data.i = whileendlab;
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
		}
		fprintf(fpout, "this is while statement\n");
		// printf("this is while statement\n");
	// }
}

// ����ֵ��䣾 ::= ����ʶ�����������ʽ��|����ʶ������[�������ʽ����]��=�����ʽ��
void assignstatement(){
	char temptoken[ALNG + 1];
	if(tab[tpos].typ == ints || tab[tpos].typ == chars){
		types temptyp = tab[tpos].typ;
		strcpy(temptoken, token);
		insymbol();
		if(sy != assignsy){
			error(3);
		}
		if(sy == assignsy){
			// inittempreg();
			expression();
			if(is_array){
				is_array = false;
				arraytyp = notyp;
			}
			y = tempitem;
			x.type = 1;
			strcpy(x.data.name, temptoken);
			z.type = 3;
			fctnum = 8;  // =
			emitmidcode(fctnum, x, y, z);
			if(y.type == 4){
				reginuse[y.data.i] = false;
			}
		}
	}	
	else if(tab[tpos].typ == intarrays || tab[tpos].typ == chararrays){
		types temptyp = tab[tpos].typ;
		char arrayname[ALNG + 1];
		int atpos = loc(token, f);
		int alen = atab[tab[atpos].ref].len;
		mcfield offset;
		strcpy(arrayname, token);
		insymbol();
		if(sy != lbracketsy){
			error(10);
		}
		// if(sy == lbracketsy){  // ���ܻ���bug
			// inittempreg();
			expression();
			if(tempitem.type == 0 || tempitem.type == 6){
				if(tempitem.data.i >= alen || tempitem.data.i < 0){
					error(11);
				}
			}
			if(is_array){
				is_array = false;
				arraytyp = notyp;
			}
			offset = tempitem;
			if(sy != rbracketsy){
				error(8);
			}
			if(sy == rbracketsy){
				insymbol();
			}
			if(sy != assignsy){
				error(3);
			}
			if(sy == assignsy){
				// inittempreg();
				expression();
				if(is_array){
					is_array = false;
					arraytyp = notyp;
				}
				z = tempitem;
				y = offset;
				x.type = 1;
				strcpy(x.data.name, arrayname);
				fctnum = 9; // []=
				emitmidcode(fctnum, x, y, z);
				if(y.type == 4){
					reginuse[y.data.i] = false;
				}
				if(z.type == 4){
					reginuse[z.data.i] = false;
				}
			}
		// }
	}
	fprintf(fpout, "this is assign statement\n");
	// printf("this is assign statement\n");
}

// ������䣾 ::= scanf��(������ʶ����{,����ʶ����}��)��
void scanfstatement(){
	insymbol();
	if(sy != lparentsy){
		error(17);
	}
	if(sy == lparentsy){  // ���ܻ���bug
		insymbol();
	}
		if(sy != idsy){
			error(19);
		}
		if(sy == idsy){
			// ��Ԫʽ
			fctnum = 28; // read
			x.type = 1;
			strcpy(x.data.name, token);
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			insymbol();
		}
		while(sy == commasy){
			insymbol();
			if(sy != idsy){   // ���ܻ���bug
				error(19);
			}
			if(sy == idsy){
				// ��Ԫʽ
				fctnum = 28; // read
				x.type = 1;
				strcpy(x.data.name, token);
				y.type = 3;
				z.type = 3;
				emitmidcode(fctnum, x, y, z);
				insymbol();
			}
		}
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){
			insymbol();
		}
		fprintf(fpout, "this is scanf statement\n");
		// printf("this is scanf statement\n");
	// }
}

// ��д��䣾 ::= printf��(�����ַ�����,�����ʽ����)��| printf��(�����ַ�������)��| printf��(�������ʽ����)��
void printfstatement(){
	insymbol();
	if(sy != lparentsy){
		error(17);
	}
	if(sy == lparentsy){  // ���ܻ���bug
		insymbol();
	}
		if(sy == stringcon){
			// ��Ԫʽ
			fctnum = 29;  // write
			x.type = 6;
			y.type = 3;
			z.type = 3;
			for(int j = inum; j < inum + sleng; j++){
				x.data.i = stab[j];
				emitmidcode(fctnum, x, y, z);
			}
			fprintf(fpout, "this is string\n");
			// printf("this is string\n");
			insymbol();
			if(sy == commasy){
				// inittempreg();
				expression();
				fctnum = 29;  // write
				x = tempitem;
				if(is_array){
					if(arraytyp == chararrays){
						y.type = 7;
					}
					is_array = false;
					arraytyp = notyp;
				}
				else{
					y.type = 3;
				}
				z.type = 3;
				emitmidcode(fctnum, x, y, z);
				if(x.type == 4){
					reginuse[x.data.i] = false;
				}
			}
			if(sy != rparentsy){
				error(13);
			}
			if(sy == rparentsy){
				insymbol();
			}
		}
		else{
			// inittempreg();
			int tempfct;
			if(sy == plussy || sy == minussy){
				tempfct = (sy == minussy) ? 14 : 13;
				insymbol();
				term();
				if(is_array){
					is_array = false;
					arraytyp = notyp;
				}
				y.type = 0;
				y.data.i = 0;
				z = tempitem;
				int reg = regid();
				int zaltemppos = loc(zaltemp[reg],f);
				if(zaltemppos == 0){
					entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
					dx = dx + 1;
				}
				x.type = 4;
				x.data.i = reg;
				emitmidcode(tempfct, x, y, z); // x��ֵ
				tempitem = x;
				if(y.type == 4){
					reginuse[y.data.i] = false;
				}
				if(z.type == 4){
					reginuse[z.data.i] = false;
				}
			}
			else{
				term();
			}
			while(sy == plussy || sy == minussy){
				if(is_array){
					is_array = false;
					arraytyp = notyp;
				}
				mcfield opnum;
				tempfct = (sy == plussy) ? 13 : 14;
				opnum = tempitem;
				insymbol();
				term();
				y = opnum;
				z = tempitem;
				int reg = regid();
				int zaltemppos = loc(zaltemp[reg],f);
				if(zaltemppos == 0){
					entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
					dx = dx + 1;
				}
				x.type = 4;
				x.data.i = reg;
				emitmidcode(tempfct, x, y, z);  // x��ֵ
				tempitem = x;
				if(y.type == 4){
					reginuse[y.data.i] = false;
				}
				if(z.type == 4){
					reginuse[z.data.i] = false;
				}
			}
			if(sy != rparentsy){
				error(13);
			}
			fctnum = 29;  // write
			x = tempitem;
			if(is_array){
				if(arraytyp == chararrays){
					y.type = 7;
				}
				is_array = false;
				arraytyp = notyp;
			}
			else{
				y.type = 3;
			}
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			if(x.type == 4){
				reginuse[x.data.i] = false;
			}
			if(sy == rparentsy){
				insymbol();
			}
			fprintf(fpout, "this is expression\n");
			// printf("this is expression\n");
		}
		fctnum = 29;
		x.type = 6;
		x.data.i = 10;
		y.type = 3;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		fprintf(fpout, "this is printf statement\n");
		// printf("this is printf statement\n");
	//}
}

// ���������䣾 ::= case��������������䣾
void casestatement(types switchtyp){
	int caseendlab;
	// if(sy == casesy){
		// insymbol();
		int sign = 1;
		if(sy == plussy || sy == minussy){
			sign = (sy == plussy) ? 1 : -1;
			insymbol();
		}
		if(sy != intcon && sy != charcon){
			error(4);
		}
		if((sy == intcon && switchtyp == chars) || (sy == charcon && switchtyp == ints)){
			error(5);
		}
		inum = inum * sign;
		int reg = regid();
		int zaltemppos = loc(zaltemp[reg],f);
		if(zaltemppos == 0){
			entertab(zaltemp[reg], vvariable, ints, 0, f, dx);
			dx = dx + 1;
		}
		fctnum = 20;  // ==
		x.type = 4;
		x.data.i = reg;
		y = switchexpr;
		z.type = (sy == intcon) ? 0 : 6;
		z.data.i = inum;
		emitmidcode(fctnum, x, y, z);
		tempitem = x;
		fctnum = 22;  //bz
		y.type = 0;
		caseendlab = labelnum++;
		y.data.i = caseendlab;
		z.type = 3;
		x = tempitem;
		emitmidcode(fctnum, x, y, z);
		if(x.type == 4){
			reginuse[x.data.i] = false;
		}
		insymbol();
		if(sy != colonsy){
			error(20);
		}
		if(sy == colonsy){
			insymbol();
		}
		statement();
		caseendmc[casenum] = mc;
		fctnum = 23;  // jmp
		x.type = 0;
		x.data.i = 0;
		y.type = 3;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		fctnum = 25; // setlab
		x.type = 0;
		x.data.i = caseendlab;
		y.type = 3;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		casenum = casenum + 1;
		fprintf(fpout, "this is case statement\n");
		// printf("this is case statement\n");
	// }
}

// ������� ::= ���������䣾{���������䣾}
void caseliststatement(types switchtyp){
	if(sy != casesy){
		insymbol();
	}
	if(sy != casesy){
		error(21);
	}
	if(sy == casesy){
		insymbol();
	}
	casestatement(switchtyp);
	while(sy == casesy){
		insymbol();
		casestatement(switchtyp);
	}
	fprintf(fpout, "this is case list statement\n");
	// printf("this is case list statement\n");
}

// ��ȱʡ�� ::= default:����䣾
void defaultstatement(){
	insymbol();
	if(sy != colonsy){
		error(20);
	}
	if(sy == colonsy){
		insymbol();
	}
	statement();
	fprintf(fpout, "this is default statement\n");
	// printf("this is default statement\n");
}

// �������䣾 ::= switch��(�������ʽ����)����{���������[��ȱʡ��]��}��
void switchstatement(){
	// �����ֳ�
	mcfield tempswitchexpr;
	int tempcaseendmc[CSMAX];
	int tempcasenum;
	tempswitchexpr = switchexpr;
	for(int i = 0; i < casenum; i++){
		tempcaseendmc[i] = caseendmc[i];
	}
	tempcasenum = casenum;

	int switchendlab;
	casenum = 0;
	insymbol();
	if(sy != lparentsy){
		error(17);
	}
	// if(sy == lparentsy){  //���ܻ���bug
		// inittempreg();
		expression();
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		switchexpr = tempitem;
		if(sy != rparentsy){
			error(13);
		}
		if(sy == rparentsy){
			insymbol();
		}
		if(sy != lbracesy){
			error(22);
		}
		// if(sy == lbracesy){
			caseliststatement(switchtyp);
			if(sy == defaultsy){
				defaultstatement();
			}
			if(sy != rbracesy){
				error(23);
				// printf("here\n");
				//exit(0);
			}
			switchendlab = labelnum++;
			fprintf(fpout, "switchendlab=%d\n", switchendlab);
			fctnum = 25;
			x.type = 0;
			x.data.i = switchendlab;
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			for(int i = 0; i < casenum; i++){
				mctab[caseendmc[i]].x.data.i = switchendlab;
			}
		// }
		// ��ԭ�ֳ�
		switchexpr = tempswitchexpr;
		for(int i = 0; i < tempcasenum; i++){
			caseendmc[i] = tempcaseendmc[i];
		}
		casenum = tempcasenum;
		fprintf(fpout, "this is switch statement\n");
		// printf("this is switch statement\n");
	// }
}

// ��������䣾 ::= return[��(�������ʽ����)��]
void retstatement(){
	int ftpos = ftab[f].lastpar;
	if(tab[ftpos].findex != 0){
		while(tab[ftpos].link != 0){
			ftpos = tab[ftpos].link;
		}
		ftpos = ftpos - 1;
	}
	insymbol();
	//printf("%s\n",tab[ftpos].name);
	//exit(0);
	if(sy == lparentsy){
		// inittempreg();
		expression();
		if(tab[ftpos].typ != ints && tab[ftpos].typ != chars){
			error(24);
		}
		else if(tab[ftpos].typ == ints){
			if(tempitem.type == 6){
				error(24);
			}
			else if(tempitem.type == 1){
				int temppos = loc(tempitem.data.name, f);
				if(tab[temppos].typ == chars){
					error(24);
				}
			}
		}
		else if(tab[ftpos].typ == chars){
			if(tempitem.type == 0){
				error(24);
			}
			else if(tempitem.type == 1){
				int temppos = loc(tempitem.data.name, f);
				if(tab[temppos].typ == ints){
					error(24);
				}
			}
		}
		if(is_array){
			is_array = false;
			arraytyp = notyp;
		}
		if(sy != rparentsy){
			error(13);
		}
		fctnum = 26;  // return
		x.type = 5;
		strcpy(x.data.name, retreg);
		y = tempitem;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
		if(y.type == 4){
			reginuse[y.data.i] = false;
		}
		if(sy == rparentsy){
			insymbol();
		}
	}
	else if(sy == semicolonsy){
		if(tab[ftpos].typ != notyp){
			error(24);
		}
		fctnum = 26;  // return
		x.type =3;
		y.type = 3;
		z.type = 3;
		emitmidcode(fctnum, x, y, z);
	}
	else{
		error(44);
	}
	fprintf(fpout, "this is return statement\n");
	// printf("this is return statement\n");
}

// ������У� ::= {����䣾}
void statements(){
	while(sy != rbracesy){
		statement();
	}
	fprintf(fpout, "this is statements\n");
	// printf("this is statements\n");
}

// ����䣾 ::= ��������䣾����ѭ����䣾|��{��������У���}�������з���ֵ����������䣾; 
//             |���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;|�������䣾����������䣾;
void statement(){
	if(sy != ifsy && sy != whilesy && sy != lbracesy && sy != idsy && sy != scanfsy && 
		sy != printfsy && sy != switchsy && sy != returnsy && sy != semicolonsy){
		error(26);
	}
	if(sy == ifsy){
		ifstatement();
	}
	else if(sy == whilesy){
		whilestatement();
	}
	else if(sy == lbracesy){
		insymbol();
		statements();
		if(sy != rbracesy){
			error(23);
		}
		if(sy == rbracesy){
			insymbol();
		}
	}
	else if(sy == idsy){
		// ����ű�
		tpos = loc(token, f);
		if(tpos == 0){  // ������bug
			while(sy != semicolonsy){
				insymbol();
			}
		}
		if(tab[tpos].obj == vvariable){
			assignstatement();
			if(sy != semicolonsy){
				error(45);
			}
			if(sy == semicolonsy){
				insymbol();
			}
		}
		else if(tab[tpos].obj == funktion && (tab[tpos].typ == ints || tab[tpos].typ == chars)){
			callretstatement();
			if(sy != semicolonsy){
				error(45);
			}
			if(sy == semicolonsy){
				insymbol();
			}
		}
		else if(tab[tpos].obj == funktion && tab[tpos].typ == notyp){
			callnonstatement();
			if(sy != semicolonsy){
				error(45);
			}
			if(sy == semicolonsy){
				insymbol();
			}
		}
		else{
			error(25);
			insymbol();
		}
	}
	else if(sy == scanfsy){
		scanfstatement();
		if(sy != semicolonsy){
			error(45);
		}
		if(sy == semicolonsy){
			insymbol();
		}
	}
	else if(sy == printfsy){
		printfstatement();
		if(sy != semicolonsy){
			error(45);
		}
		if(sy == semicolonsy){
			insymbol();
		}
	}
	else if(sy == switchsy){
		switchstatement();
		insymbol();
	}
	else if(sy == returnsy){
		for(int tempk = 0; tempk < t; tempk++){
			if(tab[tempk].obj == funktion && tab[tempk].ref == f && tab[tempk].typ == notyp){
				error(51);
			}
		}
		retstatement();
		if(sy != semicolonsy){
			error(45);
		}
		if(sy == semicolonsy){
			insymbol();
		}
	}
	else if(sy == semicolonsy){
		// <��>
		insymbol();
	}
	fprintf(fpout, "this is statement\n");
	// printf("this is statement\n");
}

// ��������䣾 ::=�ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
void compoundstatement(){
	insymbol();
	constdeclaration();
	if(sy == intsy || sy == charsy){
		vardeclaration();
	}
	else{
		ftab[f].vsize = dx;
	}
	ftab[f].last = t;
	statements();
	ftab[f].vsize = dx;
	fprintf(fpout, "this is compound statement\n");
	// printf("this is compound statement\n");
}

// ������ͷ���� ::= int����ʶ����|char����ʶ����
void declarationhead(){
	types tp;
	if((sy == intsy || sy == charsy)){
		tp = (sy == intsy) ? ints : chars;
		insymbol();
		if(sy != idsy){
			error(2);
		}
		if(sy == idsy){
			// ��¼���ű�, adr
			enterftab();
			entertab(token, funktion, tp, f, 0, 0);
			insymbol();
		}
		fprintf(fpout, "this is declaration head\n");
		// printf("this is declaration head\n");
	}
}

// �������� ::= �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}
void parameterlist(){
	types tp = ints;
	int countpara = 0;
	insymbol();
	if(sy != intsy && sy != charsy){
		error(27);
	}
	if(sy == intsy || sy == charsy){  // ���ܻ���bug
		countpara = countpara + 1;
		tp = (sy == intsy) ? ints : chars;
		insymbol();
	}
		if(sy != idsy){
			error(2);
		}
		if(sy == idsy){
			// ��¼tab��, adr��Ҫ����ȷ��
			entertab(token, vvariable, tp, f, f, dx);
			dx = dx + 1;
			fctnum = 6; // para
			x.type = 2;
			x.data.i = (tp == ints) ? 2 : 3; // int/char
			y.type = 1;
			strcpy(y.data.name, token); // identifier
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
		}
		insymbol();
	// }
	while(sy == commasy){
		insymbol();
		if(sy != intsy && sy != charsy){
			error(27);
		}
		if(sy == intsy || sy == charsy){
			countpara = countpara + 1;
			tp = (sy == intsy) ? ints : chars;
			insymbol();
		}
			if(sy != idsy){
				error(2);
			}
			if(sy == idsy){
				// ��¼tab��, adr��Ҫ����ȷ��
				entertab(token, vvariable, tp, f, f, dx);
				dx = dx + 1;
			}
			insymbol();
		// }
	}
	if(countpara < 1){
		error(28);
	}
	fprintf(fpout, "this is parameter list\n");
	// printf("this is parameter list\n");
}

// �������� ::= ��������
void parameter(){
	parameterlist();
	fprintf(fpout, "this is parameter\n");
	// printf("this is parameter\n");
}

// ���з���ֵ�������壾 ::= ������ͷ������(������������)����{����������䣾��}��|������ͷ������{����������䣾��}��
void retfuncdeclaration(){
	bool haspara = false;
	types tp;
	// if(sy == intsy || sy == charsy){
		tp = (sy == intsy) ? ints : chars;
		declarationhead();
		if(sy == lparentsy){
			haspara = true;
			fctnum = 5; // func
			x.type = 2;
			x.data.i = (tp == ints) ? 2 : 3; // int/char
			y.type = 1;
			strcpy(y.data.name, token); // identifier
			z.type = 2;
			z.data.i = 7; // ()
			emitmidcode(fctnum, x, y, z);
			parameter();
			if(sy != rparentsy){
				error(13);
			}
			if(sy == rparentsy){
				insymbol();
			}
		}
		/*ftab[f].lastpar = t; // renew ftab
		ftab[f].psize = dx;*/
		if(sy != lbracesy){
			error(46);
		}
		// if(sy == lbracesy){
			if(!haspara){
				fctnum = 5; // func
				x.type = 2;
				x.data.i = (tp == ints) ? 2 : 3; // int/char
				y.type = 1;
				strcpy(y.data.name, token); // identifier
				z.type = 3;
				emitmidcode(fctnum, x, y, z);
			}
			ftab[f].lastpar = t; // renew ftab
			ftab[f].psize = dx;
			compoundstatement();
			if(sy != rbracesy){
				error(23);
			}
			if(sy == rbracesy){
				insymbol();
			}
		// }
		fprintf(fpout, "this is return function defination\n");
		// printf("this is return function defination\n");
	// }
}

// ���޷���ֵ�������壾 ::= void����ʶ����(������������)����{����������䣾��}��| void����ʶ����{����������䣾��}��
void nonfuncdeclaration(){
	bool haspara = false;
	// if(sy == idsy){
		// ��¼ftab��tab, adr
		enterftab();
		entertab(token, funktion, notyp, f, 0, 0);
		insymbol();
		if(sy == lparentsy){
			haspara = true;
			fctnum = 5; // func
			x.type = 2;
			x.data.i = 4; // void
			y.type = 1;
			strcpy(y.data.name, token); // identifier
			z.type = 2;
			z.data.i = 7; // ()
			emitmidcode(fctnum, x, y, z);
			parameter();
			if(sy != rparentsy){
				error(13);
			}
			if(sy == rparentsy){
				insymbol();
			}
		}
		/*ftab[f].lastpar = t; // renew ftab
		ftab[f].psize = dx;*/
		if(sy != lbracesy){
			error(46);
		}
		// if(sy == lbracesy){
			if(!haspara){
				fctnum = 5; // func
				x.type = 2;
				x.data.i = 4; // void
				y.type = 1;
				strcpy(y.data.name, token); // identifier
				z.type = 3;
				emitmidcode(fctnum, x, y, z);
			}
			ftab[f].lastpar = t; // renew ftab
			ftab[f].psize = dx;
			compoundstatement();
			fctnum = 26;  // return
			x.type = 3;
			y.type = 3;
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			if(sy != rbracesy){
				error(23);
			}
			if(sy == rbracesy){
				insymbol();
			}
		// }
		fprintf(fpout, "this is no return function defination\n");
		// printf("this is no return function defination\n");
	// }
}

// ���������� ::= void main��(����)����{����������䣾��}��
void mainfunc(){
	if(sy == mainsy){
		// ��¼ftab��tab, adr
		enterftab();
		entertab(token, funktion, notyp, f, 0, 0);
		ftab[f].lastpar = t;
		insymbol();
		if(sy != lparentsy){
			error(47);
		}
		if(sy == lparentsy){
			insymbol();
		}
		if(sy != rparentsy){
			error(13);
		}
		ftab[f].psize = 5;
		if(sy == rparentsy){
			insymbol();
		}
		if(sy != lbracesy){
			error(46);
		}
		// if(sy == lbracesy){
			fctnum = 5; // func
			x.type = 2;
			x.data.i = 4; // void
			y.type = 1;
			strcpy(y.data.name, token); // identifier
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			compoundstatement();
			if(sy != rbracesy){  // �޷�����main������}
				error(23);
			}
			if(sy == rbracesy){
				hasend = true;
			}
		// }
		fprintf(fpout, "this is main function\n");
		// printf("this is main function\n");
	}
}

// ������ ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program(){
	fctnum = 27;  // call
	x.type = 1;
	char tempmain[ALNG + 1] = "main";
	strcpy(x.data.name, tempmain);
	y.type = 3;
	z.type = 3;
	emitmidcode(fctnum, x, y, z);
	int endlabel = labelnum++;
	fctnum = 23;  // jmp
	x.type = 0;
	x.data.i = endlabel;
	y.type = 3;
	z.type = 3;
	emitmidcode(fctnum, x, y, z);
	dx = 5;
	bool hasmain = false;
	bool hasenter = false;
	types tp;
	// init f, ftab[0]
	f = 0;
	ftab[f].last = 0;
	ftab[f].lastpar = 0;
	ftab[f].psize = 5;
	ftab[f].vsize = 5;

	insymbol();
	constdeclaration();

	while(sy == intsy || sy == charsy){
		hasenter = false;
		tp = (sy == intsy) ? ints : chars;
		insymbol();
		if(sy != idsy){
			error(49);
		}
		if(sy == idsy){
			insymbol();
		}
		if(sy == commasy){
			hasenter = true;
			// ��¼���ű�, adr��Ҫ����ȷ��
			entertab(token, vvariable, tp, 0, f, dx);
			dx = dx + 1;
			fctnum = 1; // var
			x.type = 2;
			x.data.i = (tp == ints) ? 2 : 3; // int/char
			y.type = 1;
			strcpy(y.data.name, token); // identifier
			z.type = 3;
			emitmidcode(fctnum, x, y, z);
			while(sy == commasy){
				insymbol();
				if(sy != idsy){
					error(49);
				}
				if(sy == idsy){
					insymbol();
					if(sy == lbracketsy){
						insymbol();
						if(sy != intcon || (sy == intcon && inum <= 0)){  // ���ܻ���bug
							error(7);
						}
						// if(sy == intcon){
							// ��¼���ű������������Ϣ��, adr��Ҫ����ȷ��
							enteratab(inum, tp);
							entertab(token, vvariable, (tp==ints)?intarrays:chararrays, a, f, dx);
							dx = dx + inum;
							fctnum = 1; // var
							x.type = 2;
							x.data.i = (tp == ints) ? 2 : 3; // int/char
							y.type = 1;
							strcpy(y.data.name, token); // identifier
							z.type = 0;
							z.data.i = inum; // length of array
							emitmidcode(fctnum, x, y, z);
							insymbol();
							if(sy != rbracketsy){
								error(8);
							}
							if(sy == rbracketsy){
								insymbol();
							}
						// }
					}
					else{
						// ��¼���ű�, adr��Ҫ����ȷ��
						entertab(token, vvariable, tp, 0, f, dx);
						dx = dx + 1;
						fctnum = 1; // var
						x.type = 2;
						x.data.i = (tp == ints) ? 2 : 3; // int/char
						y.type = 1;
						strcpy(y.data.name, token); // identifier
						z.type = 3;
						emitmidcode(fctnum, x, y, z);
					}
				}
			}
		}
		else if(sy == lbracketsy){
			hasenter = true;
			insymbol();
			if(sy != intcon || (sy == intcon && inum <= 0)){  // ���ܻ���bug
				error(7);
			}
			// if(sy == intcon){
				// ��¼���ű������������Ϣ��
				enteratab(inum, tp);
				entertab(token, vvariable, (tp==ints)?intarrays:chararrays, a, f, dx);
				dx = dx + inum;
				fctnum = 1; // var
				x.type = 2;
				x.data.i = (tp == ints) ? 2 : 3; // int/char
				y.type = 1;
				strcpy(y.data.name, token); // identifier
				z.type = 0;
				z.data.i = inum; // length of array
				emitmidcode(fctnum, x, y, z);
				insymbol();
				if(sy != rbracketsy){
					error(8);
				}
				if(sy == rbracketsy){
					insymbol();
				}
					while(sy == commasy){
						insymbol();
						if(sy != idsy){
							error(49);
						}
						if(sy == idsy){
							insymbol();
							if(sy == lbracketsy){
								insymbol();
								if(sy == intcon){
									// ��¼���ű������������Ϣ��, adr��Ҫ����ȷ��
									enteratab(inum, tp);
									entertab(token, vvariable, (tp==ints)?intarrays:chararrays, a, f, dx);
									dx = dx + inum;
									fctnum = 1; // var
									x.type = 2;
									x.data.i = (tp == ints) ? 2 : 3; // int/char
									y.type = 1;
									strcpy(y.data.name, token); // identifier
									z.type = 0;
									z.data.i = inum; // length of array
									emitmidcode(fctnum, x, y, z);
									insymbol();
									if(sy != rbracketsy){
										error(8);
									}
									if(sy == rbracketsy){
										insymbol();
									}
								}
							}
							else{
								// ��¼���ű�, adr��Ҫ����ȷ��
								entertab(token, vvariable, tp, 0, f, dx);
								dx = dx + 1;
								fctnum = 1; // var
								x.type = 2;
								x.data.i = (tp == ints) ? 2 : 3; // int/char
								y.type = 1;
								strcpy(y.data.name, token); // identifier
								z.type = 3;
								emitmidcode(fctnum, x, y, z);
							}
						}
					}
					fprintf(fpout, "this is a var defination\n");
					// printf("this is a var defination\n");
				// }
			// }
		}
		else{
			if(sy != semicolonsy){
				ftab[f].vsize = dx;
				dx = 5;
				bool haspara = false;
				if(sy == lparentsy){
					haspara = true;
					// ��¼ftab���tab��, adr��Ҫ����ȷ��
					enterftab();
					entertab(token, funktion, tp, f, 0, 0);
					fctnum = 5; // func
					x.type = 2;
					x.data.i = (tp == ints) ? 2 : 3; // int/char
					y.type = 1;
					strcpy(y.data.name, token); // identifier
					z.type = 2;
					z.data.i = 7; // ()
					emitmidcode(fctnum, x, y, z);
					parameter();
					if(sy != rparentsy){
						error(13);
					}
					if(sy == rparentsy){
						insymbol();
					}
				}
				/*ftab[f].lastpar = t; // renew ftab
				ftab[f].psize = dx;*/
				if(sy != lbracesy){
					error(46);
				}
				// if(sy == lbracesy){
					if(!haspara){
						// ��¼ftab���tab��, adr��Ҫ����ȷ��
						enterftab();
						entertab(token, funktion, tp, f, 0, 0);
						fctnum = 5; // func
						x.type = 2;
						x.data.i = (tp == ints) ? 2 : 3; // int/char
						y.type = 1;
						strcpy(y.data.name, token); // identifier
						z.type = 3;
						emitmidcode(fctnum, x, y, z);
					}
					ftab[f].lastpar = t; // renew ftab
					ftab[f].psize = dx;
					// ��¼���ű�
					compoundstatement();
					if(sy != rbracesy){
						error(23);
					}
					if(sy == rbracesy){
						insymbol();
					}
				// }
				break;
			}
			fprintf(fpout, "this is a var defination\n");
			// printf("this is a var defination\n");
		}
		if(sy != semicolonsy){
			error(45);
		}
		if(sy == semicolonsy){
			if(!hasenter){
				entertab(token, vvariable, tp, 0, f, dx);
				dx = dx + 1;
				fctnum = 1; // var
				x.type = 2;
				x.data.i = (tp == ints) ? 2 : 3; // int/char
				y.type = 1;
				strcpy(y.data.name, token); // identifier
				z.type = 3;
				emitmidcode(fctnum, x, y, z);
			}
			insymbol();
		}
	}

	if(!(sy == intsy || sy == charsy || sy == voidsy)){
		error(50);
	}

	ftab[f].vsize = dx;
	while(sy == intsy || sy == charsy || sy == voidsy){
		dx = 5;
		if(sy == intsy || sy == charsy){
			retfuncdeclaration();
		}
		else{ // sy == voidsy
			insymbol();
			if(sy != idsy && sy != mainsy){
				error(29);
				nonfuncdeclaration();
			}
			else if(sy == idsy){
				nonfuncdeclaration();
			}
			else if(sy == mainsy){
				hasmain=true;
				mainfunc();
				break;
			}
		}
		if(!(sy == intsy || sy == charsy || sy == voidsy)){
			error(50);
		}
	}

	if(!hasmain){
		error(30);
	}
	fctnum = 25; // setlab
	x.type = 0;
	x.data.i = endlabel;
	y.type = 3;
	z.type = 3;
	emitmidcode(fctnum, x, y, z);
	emitmips();
	fprintf(fpout, "this is program\n");
	// printf("this is program\n");

	insymbol();
	error(31);
	while(1){
		insymbol();
	}
}