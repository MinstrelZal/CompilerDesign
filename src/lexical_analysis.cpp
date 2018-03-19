#include "stdafx.h"

bool isSpace(){
	return ch == ' ';
}

bool isTab(){
	return ch == '\t';
}

bool isNewline(){
	return ch == '\n';
}

bool isNZDigit(){
	return (ch > '0' && ch <= '9');
}

bool isZero(){
	return ch == '0';
}

bool isDigit(){
	return (isZero() || isNZDigit());
}

bool isLetter(){
	return ((ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

bool isPlus(){
	return ch == '+';
}

bool isMinus(){
	return ch == '-';
}

bool isMulti(){
	return ch == '*';
}

bool isDivi(){
	return ch == '/';
}

bool isLess(){
	return ch == '<';
}

bool isGreat(){
	return ch == '>';
}

bool isExclamatory(){
	return ch == '!';
}

bool isAssign(){
	return ch == '=';
}

bool isLegalASCII(){
	return (ch == ' ' || ch == '!' || (ch >= '#' && ch <= '~'));
}

bool isLbracket(){
	return ch == '[';
}

bool isRbracket(){
	return ch == ']';
}

bool isLparent(){
	return ch == '(';
}

bool isRparent(){
	return ch == ')';
}

bool isLbrace(){
	return ch == '{';
}

bool isRbrace(){
	return ch == '}';
}

bool isSemicolon(){
	return ch == ';';
}

bool isComma(){
	return ch == ',';
}

bool isColon(){
	return ch == ':';
}

bool isSingleQ(){
	return ch == '\'';
}

bool isDoubleQ(){
	return ch == '"';
}

void nextch(){
	if(cc == ll){
		ch = fgetc(fpin);
		if(ch == EOF){
			if(!hasend){
				error(48);
			}
			fprintf(fpout, "文件读完了，该写输出文件了\n");
			fprintf(fpout, "调用错误提示信息输出程序errormsg\n\n");
			// printf("文件读完了，该写输出文件了\n");
			// printf("调用错误提示信息输出程序errormsg\n\n");
			errormsg();
			printtables();
			printf("\nsource code line = %d\n", linenum);
			exit(0);
		}
		if(errpos != 0){
			fprintf(fpout, "errpos != 0,说明有错误，开始处理错误\n");
			fprintf(fpout, "根据skipflag可以调用跳过错误代码的函数endskip\n\n");
			// printf("errpos != 0,说明有错误，开始处理错误\n");
			// printf("根据skipflag可以调用跳过错误代码的函数endskip\n\n");
			errpos = 0;
		}
		linenum = linenum + 1;
		ll = 0;
		cc = 0;
		while(!isNewline() && ch != EOF){
			fprintf(fpout, "%c", ch);
			// printf("%c", ch);
			line[ll++] = ch;
			ch = fgetc(fpin);
		}
		line[ll++] = ' ';
		fprintf(fpout, "\n");
		// printf("\n");
	}
	ch = line[cc++];
}

void transCh(){
	if(ch >= 'A' && ch <= 'Z'){
		ch = ch + 32;
	}
}

void clearToken(){
	for(int i = 0; i < ALNG + 1; i++){
		token[i] = '\0';
	}
	tkl = 0;
}

void catToken(){
	token[tkl++] = ch;
}

int reserver(){  // binary search key word
	int i, j, k;
	i = 0;
	j = NKW - 1;
	while(i <= j){
		k = (i + j) / 2;
		if(strcmp(token, key[k]) <= 0){
			j = k - 1;
		}
		if(strcmp(token, key[k]) >= 0) {
			i = k + 1;
		}
	}
	if(i - 1 > j){
		return k; // is a reserved word
	}
	return -1; // is an identifier
}

void insymbol(){
	//clearToken();
	while(isSpace() || isTab()){     // ignore all ' ' or '\t' or '\n'
		nextch();
	}
	if(isLetter()){     // identifier
		clearToken();
		transCh();
		catToken();
		nextch();
		while(isLetter() || isDigit()){
			if(tkl < ALNG){
				transCh();
				catToken();
				nextch();
			}
			else{
				// error(0);
				while(isLetter() || isDigit()){
					nextch();
				}
				break;
			}
		}
		int resultValue = reserver();
		if(resultValue != -1){   // key word
			sy = ksy[resultValue];
		}
		else{    // user-defined identifier
			sy = idsy;
		}
	}
	else if(isNZDigit()){      // unsigned integer
		int k = 0;
		inum = 0;
		sy = intcon;
		while(isDigit()){
			inum = inum * 10 + ch - '0';
			k = k + 1;
			nextch();
		}
		if(k > KMAX || inum > NMAX){
			// error(34);
			inum = 0;
			k = 0;
		}
	}
	else if(isZero()){  // 0
		//catToken();
		inum = 0;
		sy = intcon;
		nextch();
	}
	else if(isPlus() || isMinus()){      // integer
		if(sy == plussy || sy == minussy){
			//catToken();
			int sign = isPlus() ? 1 : -1;
			nextch();
			if(isDigit()){
				int k = 0;
				inum = 0;
				sy = intcon;
				while(isDigit()){
					inum = inum * 10 + ch - '0';
					k = k + 1;
					nextch();
				}
				if(k > KMAX || inum > NMAX){
					// error(34);
					inum = 0;
					k = 0;
				}
				inum = inum * sign;
			}
			else{
				sy = (sign == 1) ? plussy : minussy;
			}
		}
		else{
			sy = isPlus() ? plussy : minussy;  // + -
			nextch();
		}
	}
	else if(isMulti()){
		sy = multisy;
		nextch();
	}
	else if(isDivi()){
		sy = divisy;
		nextch();
	}
	else if(isLess()){
		nextch();
		if(isAssign()){
			sy = leoeqsy;
			nextch();
		}
		else{
			sy = lesssy;
		}
	}
	else if(isGreat()){
		nextch();
		if(isAssign()){
			sy = groeqsy;
			nextch();
		}
		else{
			sy = gresy;
		}
	}
	else if(isExclamatory()){    // !=
		nextch();
		if(isAssign()){
			sy = neqsy;
			nextch();
		}
		else{
			// error(35);
		}
	}
	else if(isAssign()){
		nextch();
		if(isAssign()){
			sy = eqsy;
			nextch();
		}
		else{
			sy = assignsy;
		}
	}
	else if(isLbracket()){
		sy = lbracketsy;
		nextch();
	}
	else if(isRbracket()){
		sy = rbracketsy;
		nextch();
	}
	else if(isLparent()){
		sy = lparentsy;
		nextch();
	}
	else if(isRparent()){
		sy = rparentsy;
		nextch();
	}
	else if(isLbrace()){
		sy = lbracesy;
		nextch();
	}
	else if(isRbrace()){
		sy = rbracesy;
		nextch();
	}
	else if(isSemicolon()){
		sy = semicolonsy;
		nextch();
	}
	else if(isComma()){
		sy = commasy;
		nextch();
	}
	else if(isColon()){
		sy = colonsy;
		nextch();
	}
	else if(isSingleQ()){     // char const
		sy = charcon;
		nextch();
		if(isPlus() || isMinus() || 
		   isMulti() || isDivi() || 
		   isLetter() || isDigit()){
			inum = ch;
			nextch();
		}
		else{
			error(36);
			inum = ch;
			nextch();
		}
		if(isSingleQ()){
			nextch();
		}
		else{
			// error(37);
		}
	}
	else if(isDoubleQ()){     // string const
		sy = stringcon;
		int k = 0;
		nextch();
		if(isDoubleQ()){
			if(sx + k == SMAX){
				fatal(1);  // stab表溢出
			}
			stab[sx + k] = '\0';
			k = k + 1;
			inum = sx;
			sleng = 0;
			sx = sx + k;
			nextch();
		}
		else{
			while(true){
				if(cc == 1){
					// error(38);
					break;
				}
				if(isDoubleQ()){
					nextch();
					break;
				}
				if(!isLegalASCII()){
					error(39);
				}
				if(sx + k == SMAX){
					fatal(1);  // stab表溢出
				}
				stab[sx + k] = ch;
				k = k + 1;
				nextch();
			}
			inum = sx;
			sleng = k;
			sx = sx + k;
		}
	}
}

void output(int i){
	if(sy >= casesy && sy <= whilesy){
		fprintf(fpout, "%d %ssy %s\n", i, key[sy], token);
	}
	else{
		switch(sy){
			case plussy:
				fprintf(fpout, "%d plussy +\n", i);
				break;
			case minussy:
				fprintf(fpout,"%d minussy -\n", i);
				break;
			case multisy:
				fprintf(fpout, "%d multisy *\n", i);
				break;
			case divisy:
				fprintf(fpout, "%d divisy /\n", i);
				break;
			case lesssy:
				fprintf(fpout, "%d lesssy <\n", i);
				break;
			case leoeqsy:
				fprintf(fpout, "%d leoeqsy <=\n", i);
				break;
			case gresy:
				fprintf(fpout, "%d gresy >\n", i);
				break;
			case groeqsy:
				fprintf(fpout, "%d groeqsy >=\n", i);
				break;
			case neqsy:
				fprintf(fpout, "%d neqsy !=\n", i);
				break;
			case eqsy:
				fprintf(fpout, "%d eqsy ==\n", i);
				break;
			case lbracketsy:
				fprintf(fpout, "%d lbracketsy [\n", i);
				break;
			case rbracketsy:
				fprintf(fpout, "%d rbracketsy ]\n", i);
				break;
			case lparentsy:
				fprintf(fpout, "%d lparentsy (\n", i);
				break;
			case rparentsy:
				fprintf(fpout, "%d rparentsy )\n", i);
				break;
			case lbracesy:
				fprintf(fpout, "%d lbracesy {\n", i);
				break;
			case rbracesy:
				fprintf(fpout, "%d rbracesy }\n", i);
				break;
			case semicolonsy:
				fprintf(fpout, "%d semicolonsy ;\n", i);
				break;
			case commasy:
				fprintf(fpout, "%d commasy ,\n", i);
				break;
			case assignsy:
				fprintf(fpout, "%d assignsy =\n", i);
				break;
			case colonsy:
				fprintf(fpout, "%d colonsy :\n", i);
				break;
			case idsy:
				fprintf(fpout, "%d idsy %s\n", i, token);
				break;
			case intcon:
				fprintf(fpout, "%d intcon %s\n", i, token);
				break;
			case charcon:
				fprintf(fpout, "%d charcon \'%c\'\n", i, inum);
				break;
			case stringcon:
				fprintf(fpout, "%d stringcon ", i);
				fprintf(fpout, "%c",'"');
				for(int j = inum; j < inum + sleng; j++){
					fprintf(fpout, "%c", stab[j]);
				}
				fprintf(fpout, "%c",'"');
				fprintf(fpout, "\n");
				break;
			default:
				break;
		}
	}
}

