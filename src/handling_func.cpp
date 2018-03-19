#include "stdafx.h"

void entertab(char id[ALNG + 1], objecttyp obj, types typ, int ref, int findex, int adr){
	int j,k;
	if(t == TMAX){
		fatal(0);
	}
	strcpy(tab[0].name, id);
	j = ftab[findex].last;
	k = j;
	while(strcmp(tab[j].name, id) != 0){
		j = tab[j].link;
	}
	if(j != 0){
		error(32);
	}
	else{
		t = t + 1;
		strcpy(tab[t].name, id);
		tab[t].link = k;
		tab[t].obj = obj;
		tab[t].typ = typ;
		tab[t].ref = ref;
		tab[t].findex = findex;
		tab[t].adr = adr;
		ftab[findex].last = t; // renew ftab
	}
}

void enterftab(){
	if(f == FMAX){
		fatal(3);
	}
	f = f + 1;
	ftab[f].last = 0;
	ftab[f].lastpar = 0;
	ftab[f].psize = 0;
	ftab[f].vsize = 0;
}

void enteratab(int len, types tp){
	if(a == AMAX){
		fatal(2);
	}
	a = a + 1;
	atab[a].len = len;
	atab[a].elsize = (tp == ints) ? sizeof(int) : sizeof(char);
	atab[a].size = atab[a].len * atab[a].elsize;
}

int loc(char id[ALNG + 1], int findex){
	int i, j;
	i = findex;
	strcpy(tab[0].name, id);
	do{
		j = ftab[i].last;
		while(strcmp(tab[j].name, id) != 0){
			j = tab[j].link;
		}
		if(i == 0){
			i--;
		}
		else{
			i = 0;
		}
	}while(!(i < 0 || j != 0));
	if(j == 0){
		if(!(id[0] == 'z' && id[1] == 'a' && id[2] == 'l' && id[3] == 't' &&
			id[4] == 'e' && id[5] == 'm' && id[6] == 'p')){
			error(33);
		}
	}
	return j;
}

void printtables(){
	fprintf(fpout, "\n\ntab:\n");
	// printf("\n\ntab:\n");
	fprintf(fpout, "     name  link  obj  typ  ref  findex  adr\n");
	for(int i = 1; i <= t; i++){
		fprintf(fpout, "%d: %s  %d  %d  %d  %d  %d  %d\n", i, tab[i].name, tab[i].link, tab[i].obj, tab[i].typ, tab[i].ref, tab[i].findex, tab[i].adr);
		// printf("%d: %s  %d  %d  %d  %d  %d  %d\n", i, tab[i].name, tab[i].link, tab[i].obj, tab[i].typ, tab[i].ref, tab[i].findex, tab[i].adr);
	}
	fprintf(fpout, "\n\nftab:\n");
	// printf("\n\nftab:\n");
	fprintf(fpout, "     last  lastpar  psize  vsize\n");
	for(int i = 0; i <= f; i++){
		fprintf(fpout, "%d: %d  %d  %d  %d\n", i, ftab[i].last, ftab[i].lastpar, ftab[i].psize, ftab[i].vsize);
		// printf("%d: %d  %d  %d  %d\n", i, ftab[i].last, ftab[i].lastpar, ftab[i].psize, ftab[i].vsize);
	}
	fprintf(fpout, "\n\natab:\n");
	// printf("\n\natab:\n");
	fprintf(fpout, "     len  elsize  size\n");
	for(int i = 1; i <= a; i++){
		fprintf(fpout, "%d: %d  %d  %d\n", i, atab[i].len, atab[i].elsize, atab[i].size);
		// printf("%d: %d  %d  %d\n", i, atab[i].len, atab[i].elsize, atab[i].size);
	}
	if(e == 0){
		block();
		constdelete(0);
		printmctab();
		peephole();
		printmips(0);
		optimize_dag();
		// constdelete(1);
		printoptmctab();
		emitoptmips();
		printmips(1);
	}
}

void skip(int n){
	if(n == 1){  // const后应该是int或char
		while(!(sy == intsy || sy == charsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 2){  //int或char或void或,后应该是标识符
		while(!(sy == idsy || sy == semicolonsy || sy == lbracesy || sy == lparentsy || sy == rparentsy)){
			insymbol();
		}
	}
	else if(n == 3){  // 应该为=
		while(!(sy == assignsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 4){  //应该为整数或字符常量
		while(!(sy == commasy || sy == semicolonsy || sy == colonsy)){
			insymbol();
		}
	}
	else if(n == 6){  // 常量定义结束时应该为;
		while(!(sy == semicolonsy || sy == constsy || sy == intsy || sy == charsy || sy == voidsy)){
			insymbol();
		}
	}
	else if(n == 7){  // 应该为无符号整数
		while(!(sy == intcon || sy == rbracketsy)){
			insymbol();
		}
	}
	/*else if(n == 8){  // 应该为]
		while(!(sy == semicolonsy || sy == commasy)){
			insymbol();
		}
	}*/
	else if(n == 9){  // 表达式中不是合法的因子首符号(标识符、(、整数、字符常量)
		while(!(sy == idsy || sy == lparentsy || sy == intcon || sy == charcon)){
			insymbol();
		}
	}
	else if(n == 10){  // 表达式中应该为[
		while(!(sy == idsy || sy == lparentsy || sy == intcon || sy == charcon)){
			insymbol();
		}
	}
	else if(n == 13){  // 应该为)
		while(!(sy == semicolonsy || sy == lbracesy)){
			insymbol();
		}
	}
	else if(n == 14){  // 不应该有(
		while(!(sy == rparentsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 17){  // 应该为(
		while(!(sy == idsy || sy == lparentsy || sy == intcon || sy == charcon || sy == semicolonsy || sy == stringcon)){
			insymbol();
		}
	}
	else if(n == 19){  // scanf(后应该为标识符
		while(!(sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 20){  // 应该为:
		while(!(sy == ifsy || sy == whilesy || sy == lbracesy || sy == idsy || sy == scanfsy || sy == printfsy ||
				sy == switchsy || sy == returnsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 21){  // 应该为case // 无法解决嵌套switch
		while(!(sy == intcon || sy == charcon || sy == colonsy || sy == casesy)){
			insymbol();
		}
	}
	else if(n == 26){  // 不是合法的语句开头(if、while、{、标识符、scanf、printf、switch、return、;)
		while(!(sy == ifsy && sy == whilesy && sy == lbracesy && sy == idsy && sy == scanfsy && 
			sy == printfsy && sy == switchsy && sy == returnsy && sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 41){  // 变量定义结束时应该为;
		while(!(sy == intsy || sy == charsy || sy == voidsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 49){  // 应该为标识符
		while(!(sy == semicolonsy || sy == intsy || sy == charsy || sy == voidsy)){
			insymbol();
		}
	}
	else if(n == 50){  // 应该为int或char或void
		while(!(sy == intsy || sy == charsy || sy == voidsy)){
			insymbol();
		}
	}
}

void error(int n){
	erroritem newerror;
	newerror.errornum = n;
	newerror.linenum = linenum;
	newerror.charnum = cc;
	errortab[e++] = newerror;
	skip(n);
	if(e == ERMAX){
		fatal(8);
	}
}

void errormsg(){
	int i;
	fprintf(fpout, "\n************** error message **************\n");
	printf("\n************** error message **************\n");
	for(i = 0; i < e; i++){
		fprintf(fpout, "line %d, character %d: ", errortab[i].linenum, errortab[i].charnum);
		printf("line %d, character %d: ", errortab[i].linenum, errortab[i].charnum);
		if(errortab[i].errornum == 0){
			fprintf(fpout, "标识符长度溢出\n");
			printf("标识符长度溢出\n");
		}
		else if(errortab[i].errornum == 1){
			fprintf(fpout, "const后应该是int或char\n");
			printf("const后应该是int或char\n");
		}
		else if(errortab[i].errornum == 2){
			fprintf(fpout, "int或char或void或,后应该是标识符\n");
			printf("int或char或void或,后应该是标识符\n");
		}
		else if(errortab[i].errornum == 3){
			fprintf(fpout, "应该为=\n");
			printf("应该为=\n");
		}
		else if(errortab[i].errornum == 4){
			fprintf(fpout, "应该为整数或字符常量\n");
			printf("应该为整数或字符常量\n");
		}
		else if(errortab[i].errornum == 5){
			fprintf(fpout, "定义与赋值类型不一致\n");
			printf("定义与赋值类型不一致\n");
		}
		else if(errortab[i].errornum == 6){
			fprintf(fpout, "常量定义结束时应该为;\n");
			printf("常量定义结束时应该为;\n");
		}
		else if(errortab[i].errornum == 7){
			fprintf(fpout, "应该为无符号整数\n");
			printf("应该为无符号整数\n");
		}
		else if(errortab[i].errornum == 8){
			fprintf(fpout, "应该为]\n");
			printf("应该为]\n");
		}
		else if(errortab[i].errornum == 9){
			fprintf(fpout, "不是合法的因子首符号(标识符、[、整数、字符常量)\n");
			printf("不是合法的因子首符号(标识符、[、整数、字符常量)\n");
		}
		else if(errortab[i].errornum == 10){
			fprintf(fpout, "应该为[\n");
			printf("应该为[\n");
		}
		else if(errortab[i].errornum == 11){
			fprintf(fpout, "数组下标越界\n");
			printf("数组下标越界\n");
		}
		else if(errortab[i].errornum == 12){
			fprintf(fpout, "函数类型不应该是void，应该是int或char\n");
			printf("函数类型不应该是void，应该是int或char\n");
		}
		else if(errortab[i].errornum == 13){
			fprintf(fpout, "应该为)\n");
			printf("应该为)\n");
		}
		else if(errortab[i].errornum == 14){
			fprintf(fpout, "不应该有(\n");
			printf("不应该有(\n");
		}
		else if(errortab[i].errornum == 15){
			fprintf(fpout, "参数数量太多\n");
			printf("参数数量太多\n");
		}
		else if(errortab[i].errornum == 16){
			fprintf(fpout, "参数数量太少\n");
			printf("参数数量太少\n");
		}
		else if(errortab[i].errornum == 17){
			fprintf(fpout, "应该为(\n");
			printf("应该为(\n");
		}
		else if(errortab[i].errornum == 18){
			fprintf(fpout, "应该为else\n");
			printf("应该为else\n");
		}
		else if(errortab[i].errornum == 19){
			fprintf(fpout, "scanf(后应该为标识符\n");
			printf("scanf(后应该为标识符\n");
		}
		else if(errortab[i].errornum == 20){
			fprintf(fpout, "应该为:\n");
			printf("应该为:\n");
		}
		else if(errortab[i].errornum == 21){
			fprintf(fpout, "应该为case\n");
			printf("应该为case\n");
		}
		else if(errortab[i].errornum == 22){
			fprintf(fpout, "应该为{\n");
			printf("应该为{\n");
		}
		else if(errortab[i].errornum == 23){
			fprintf(fpout, "应该为}\n");
			printf("应该为}\n");
		}
		else if(errortab[i].errornum == 24){
			fprintf(fpout, "返回值与定义不一致致\n");
			printf("返回值与定义不一致\n");
		}
		else if(errortab[i].errornum == 25){
			fprintf(fpout, "标识符种类错误\n");
			printf("标识符种类错误\n");
		}
		else if(errortab[i].errornum == 26){
			fprintf(fpout, "不是合法的语句开头(if、while、{、标识符、scanf、printf、switch、return、;)\n");
			printf("不是合法的语句开头(if、while、{、标识符、scanf、printf、switch、return、;)\n");
		}
		else if(errortab[i].errornum == 27){
			fprintf(fpout, "参数类型应该为int或char\n");
			printf("参数类型应该为int或char\n");
		}
		else if(errortab[i].errornum == 28){
			fprintf(fpout, "无参函数不应该有括号\n");
			printf("无参函数不应该有括号\n");
		}
		else if(errortab[i].errornum == 29){
			fprintf(fpout, "应该为标识符或main\n");
			printf("应该为标识符或main\n");
		}
		else if(errortab[i].errornum == 30){
			fprintf(fpout, "缺少主函数\n");
			printf("缺少主函数\n");
		}
		else if(errortab[i].errornum == 31){
			fprintf(fpout, "主函数结束后还有单词\n");
			printf("主函数结束后还有单词\n");
		}
		else if(errortab[i].errornum == 32){
			fprintf(fpout, "标识符重复定义\n");
			printf("标识符重复定义\n");
		}
		else if(errortab[i].errornum == 33){
			fprintf(fpout, "标识符未定义\n");
			printf("标识符未定义\n");
		}
		else if(errortab[i].errornum == 34){
			fprintf(fpout, "数值溢出\n");
			printf("数值溢出\n");
		}
		else if(errortab[i].errornum == 35){
			fprintf(fpout, "!后应该为=\n");
			printf("!后应该为=\n");
		}
		else if(errortab[i].errornum == 36){
			fprintf(fpout, "不是合法的字符常量\n");
			printf("不是合法的字符常量\n");
		}
		else if(errortab[i].errornum == 37){
			fprintf(fpout, "字符常量结尾应该为单引号\n");
			printf("字符常量结尾应该为单引号\n");
		}
		else if(errortab[i].errornum == 38){
			fprintf(fpout, "字符串不支持换行\n");
			printf("字符串不支持换行\n");
		}
		else if(errortab[i].errornum == 39){
			fprintf(fpout, "不是可以出现在字符串中的合法字符\n");
			printf("不是可以出现在字符串中的合法字符\n");
		}
		else if(errortab[i].errornum == 40){
			fprintf(fpout, "不是数组\n");
			printf("不是数组\n");
		}
		else if(errortab[i].errornum == 41){
			fprintf(fpout, "变量定义结束时应该为;\n");
			printf("变量定义结束时应该为;\n");
		}
		else if(errortab[i].errornum == 42){
			fprintf(fpout, "参数错误\n");
			printf("参数错误\n");
		}
		else if(errortab[i].errornum == 43){
			fprintf(fpout, "while后面应该为(\n");
			printf("while后面应该为(\n");
		}
		else if(errortab[i].errornum == 44){
			fprintf(fpout, "return错误\n");
			printf("return错误\n");
		}
		else if(errortab[i].errornum == 45){
			fprintf(fpout, "应该为;\n");
			printf("应该为;\n");
		}
		else if(errortab[i].errornum == 46){
			fprintf(fpout, "函数定义必须有{\n");
			printf("函数定义必须有{\n");
		}
		else if(errortab[i].errornum == 47){
			fprintf(fpout, "main函数必须有(\n");
			printf("main函数必须有(\n");
		}
		else if(errortab[i].errornum == 48){
			fprintf(fpout, "代码不完整\n");
			printf("代码不完整\n");
		}
		else if(errortab[i].errornum == 49){
			fprintf(fpout, "应该为标识符\n");
			printf("应该为标识符\n");
		}
		else if(errortab[i].errornum == 50){
			fprintf(fpout, "应该为int或char或void\n");
			printf("应该为int或char或void\n");
		}
		else if(errortab[i].errornum == 51){
			fprintf(fpout, "void类型函数不能有return\n");
			printf("void类型函数不能有return\n");
		}
	}
	fprintf(fpout, "\n******************* end *******************\n");
	printf("\n******************* end *******************\n");
}

void fatal(int n){
	char msg[8][12] = {"identifier", "strings", 
	                   "arrays", "functions",
					   "cases", "middle code",
	                   "mips code", "error"};
	fprintf(fpout, "\n");
	printf("\n");
	errormsg();
	fprintf(fpout, "compiler table for %s is too small\n", msg[n]);
	printf("compiler table for %s is too small\n", msg[n]);
	exit(0);
}

/*void emitmachinecode(){
	//fprintf(mipsfp, "\n\nmipstab:\n");
	//printf("\n\mipsftab:\n");
	FILE *mipsfp;
	mipsfp = fopen("15061075_张安澜_机器码.txt","w");
	int i;
	for(i = 0; i < mipsp; i++){
		if(mipstab[i].ordernum == 0){

		}
	}
	fclose(mipsfp);
}*/