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
	if(n == 1){  // const��Ӧ����int��char
		while(!(sy == intsy || sy == charsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 2){  //int��char��void��,��Ӧ���Ǳ�ʶ��
		while(!(sy == idsy || sy == semicolonsy || sy == lbracesy || sy == lparentsy || sy == rparentsy)){
			insymbol();
		}
	}
	else if(n == 3){  // Ӧ��Ϊ=
		while(!(sy == assignsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 4){  //Ӧ��Ϊ�������ַ�����
		while(!(sy == commasy || sy == semicolonsy || sy == colonsy)){
			insymbol();
		}
	}
	else if(n == 6){  // �����������ʱӦ��Ϊ;
		while(!(sy == semicolonsy || sy == constsy || sy == intsy || sy == charsy || sy == voidsy)){
			insymbol();
		}
	}
	else if(n == 7){  // Ӧ��Ϊ�޷�������
		while(!(sy == intcon || sy == rbracketsy)){
			insymbol();
		}
	}
	/*else if(n == 8){  // Ӧ��Ϊ]
		while(!(sy == semicolonsy || sy == commasy)){
			insymbol();
		}
	}*/
	else if(n == 9){  // ���ʽ�в��ǺϷ��������׷���(��ʶ����(���������ַ�����)
		while(!(sy == idsy || sy == lparentsy || sy == intcon || sy == charcon)){
			insymbol();
		}
	}
	else if(n == 10){  // ���ʽ��Ӧ��Ϊ[
		while(!(sy == idsy || sy == lparentsy || sy == intcon || sy == charcon)){
			insymbol();
		}
	}
	else if(n == 13){  // Ӧ��Ϊ)
		while(!(sy == semicolonsy || sy == lbracesy)){
			insymbol();
		}
	}
	else if(n == 14){  // ��Ӧ����(
		while(!(sy == rparentsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 17){  // Ӧ��Ϊ(
		while(!(sy == idsy || sy == lparentsy || sy == intcon || sy == charcon || sy == semicolonsy || sy == stringcon)){
			insymbol();
		}
	}
	else if(n == 19){  // scanf(��Ӧ��Ϊ��ʶ��
		while(!(sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 20){  // Ӧ��Ϊ:
		while(!(sy == ifsy || sy == whilesy || sy == lbracesy || sy == idsy || sy == scanfsy || sy == printfsy ||
				sy == switchsy || sy == returnsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 21){  // Ӧ��Ϊcase // �޷����Ƕ��switch
		while(!(sy == intcon || sy == charcon || sy == colonsy || sy == casesy)){
			insymbol();
		}
	}
	else if(n == 26){  // ���ǺϷ�����俪ͷ(if��while��{����ʶ����scanf��printf��switch��return��;)
		while(!(sy == ifsy && sy == whilesy && sy == lbracesy && sy == idsy && sy == scanfsy && 
			sy == printfsy && sy == switchsy && sy == returnsy && sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 41){  // �����������ʱӦ��Ϊ;
		while(!(sy == intsy || sy == charsy || sy == voidsy || sy == semicolonsy)){
			insymbol();
		}
	}
	else if(n == 49){  // Ӧ��Ϊ��ʶ��
		while(!(sy == semicolonsy || sy == intsy || sy == charsy || sy == voidsy)){
			insymbol();
		}
	}
	else if(n == 50){  // Ӧ��Ϊint��char��void
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
			fprintf(fpout, "��ʶ���������\n");
			printf("��ʶ���������\n");
		}
		else if(errortab[i].errornum == 1){
			fprintf(fpout, "const��Ӧ����int��char\n");
			printf("const��Ӧ����int��char\n");
		}
		else if(errortab[i].errornum == 2){
			fprintf(fpout, "int��char��void��,��Ӧ���Ǳ�ʶ��\n");
			printf("int��char��void��,��Ӧ���Ǳ�ʶ��\n");
		}
		else if(errortab[i].errornum == 3){
			fprintf(fpout, "Ӧ��Ϊ=\n");
			printf("Ӧ��Ϊ=\n");
		}
		else if(errortab[i].errornum == 4){
			fprintf(fpout, "Ӧ��Ϊ�������ַ�����\n");
			printf("Ӧ��Ϊ�������ַ�����\n");
		}
		else if(errortab[i].errornum == 5){
			fprintf(fpout, "�����븳ֵ���Ͳ�һ��\n");
			printf("�����븳ֵ���Ͳ�һ��\n");
		}
		else if(errortab[i].errornum == 6){
			fprintf(fpout, "�����������ʱӦ��Ϊ;\n");
			printf("�����������ʱӦ��Ϊ;\n");
		}
		else if(errortab[i].errornum == 7){
			fprintf(fpout, "Ӧ��Ϊ�޷�������\n");
			printf("Ӧ��Ϊ�޷�������\n");
		}
		else if(errortab[i].errornum == 8){
			fprintf(fpout, "Ӧ��Ϊ]\n");
			printf("Ӧ��Ϊ]\n");
		}
		else if(errortab[i].errornum == 9){
			fprintf(fpout, "���ǺϷ��������׷���(��ʶ����[���������ַ�����)\n");
			printf("���ǺϷ��������׷���(��ʶ����[���������ַ�����)\n");
		}
		else if(errortab[i].errornum == 10){
			fprintf(fpout, "Ӧ��Ϊ[\n");
			printf("Ӧ��Ϊ[\n");
		}
		else if(errortab[i].errornum == 11){
			fprintf(fpout, "�����±�Խ��\n");
			printf("�����±�Խ��\n");
		}
		else if(errortab[i].errornum == 12){
			fprintf(fpout, "�������Ͳ�Ӧ����void��Ӧ����int��char\n");
			printf("�������Ͳ�Ӧ����void��Ӧ����int��char\n");
		}
		else if(errortab[i].errornum == 13){
			fprintf(fpout, "Ӧ��Ϊ)\n");
			printf("Ӧ��Ϊ)\n");
		}
		else if(errortab[i].errornum == 14){
			fprintf(fpout, "��Ӧ����(\n");
			printf("��Ӧ����(\n");
		}
		else if(errortab[i].errornum == 15){
			fprintf(fpout, "��������̫��\n");
			printf("��������̫��\n");
		}
		else if(errortab[i].errornum == 16){
			fprintf(fpout, "��������̫��\n");
			printf("��������̫��\n");
		}
		else if(errortab[i].errornum == 17){
			fprintf(fpout, "Ӧ��Ϊ(\n");
			printf("Ӧ��Ϊ(\n");
		}
		else if(errortab[i].errornum == 18){
			fprintf(fpout, "Ӧ��Ϊelse\n");
			printf("Ӧ��Ϊelse\n");
		}
		else if(errortab[i].errornum == 19){
			fprintf(fpout, "scanf(��Ӧ��Ϊ��ʶ��\n");
			printf("scanf(��Ӧ��Ϊ��ʶ��\n");
		}
		else if(errortab[i].errornum == 20){
			fprintf(fpout, "Ӧ��Ϊ:\n");
			printf("Ӧ��Ϊ:\n");
		}
		else if(errortab[i].errornum == 21){
			fprintf(fpout, "Ӧ��Ϊcase\n");
			printf("Ӧ��Ϊcase\n");
		}
		else if(errortab[i].errornum == 22){
			fprintf(fpout, "Ӧ��Ϊ{\n");
			printf("Ӧ��Ϊ{\n");
		}
		else if(errortab[i].errornum == 23){
			fprintf(fpout, "Ӧ��Ϊ}\n");
			printf("Ӧ��Ϊ}\n");
		}
		else if(errortab[i].errornum == 24){
			fprintf(fpout, "����ֵ�붨�岻һ����\n");
			printf("����ֵ�붨�岻һ��\n");
		}
		else if(errortab[i].errornum == 25){
			fprintf(fpout, "��ʶ���������\n");
			printf("��ʶ���������\n");
		}
		else if(errortab[i].errornum == 26){
			fprintf(fpout, "���ǺϷ�����俪ͷ(if��while��{����ʶ����scanf��printf��switch��return��;)\n");
			printf("���ǺϷ�����俪ͷ(if��while��{����ʶ����scanf��printf��switch��return��;)\n");
		}
		else if(errortab[i].errornum == 27){
			fprintf(fpout, "��������Ӧ��Ϊint��char\n");
			printf("��������Ӧ��Ϊint��char\n");
		}
		else if(errortab[i].errornum == 28){
			fprintf(fpout, "�޲κ�����Ӧ��������\n");
			printf("�޲κ�����Ӧ��������\n");
		}
		else if(errortab[i].errornum == 29){
			fprintf(fpout, "Ӧ��Ϊ��ʶ����main\n");
			printf("Ӧ��Ϊ��ʶ����main\n");
		}
		else if(errortab[i].errornum == 30){
			fprintf(fpout, "ȱ��������\n");
			printf("ȱ��������\n");
		}
		else if(errortab[i].errornum == 31){
			fprintf(fpout, "�������������е���\n");
			printf("�������������е���\n");
		}
		else if(errortab[i].errornum == 32){
			fprintf(fpout, "��ʶ���ظ�����\n");
			printf("��ʶ���ظ�����\n");
		}
		else if(errortab[i].errornum == 33){
			fprintf(fpout, "��ʶ��δ����\n");
			printf("��ʶ��δ����\n");
		}
		else if(errortab[i].errornum == 34){
			fprintf(fpout, "��ֵ���\n");
			printf("��ֵ���\n");
		}
		else if(errortab[i].errornum == 35){
			fprintf(fpout, "!��Ӧ��Ϊ=\n");
			printf("!��Ӧ��Ϊ=\n");
		}
		else if(errortab[i].errornum == 36){
			fprintf(fpout, "���ǺϷ����ַ�����\n");
			printf("���ǺϷ����ַ�����\n");
		}
		else if(errortab[i].errornum == 37){
			fprintf(fpout, "�ַ�������βӦ��Ϊ������\n");
			printf("�ַ�������βӦ��Ϊ������\n");
		}
		else if(errortab[i].errornum == 38){
			fprintf(fpout, "�ַ�����֧�ֻ���\n");
			printf("�ַ�����֧�ֻ���\n");
		}
		else if(errortab[i].errornum == 39){
			fprintf(fpout, "���ǿ��Գ������ַ����еĺϷ��ַ�\n");
			printf("���ǿ��Գ������ַ����еĺϷ��ַ�\n");
		}
		else if(errortab[i].errornum == 40){
			fprintf(fpout, "��������\n");
			printf("��������\n");
		}
		else if(errortab[i].errornum == 41){
			fprintf(fpout, "�����������ʱӦ��Ϊ;\n");
			printf("�����������ʱӦ��Ϊ;\n");
		}
		else if(errortab[i].errornum == 42){
			fprintf(fpout, "��������\n");
			printf("��������\n");
		}
		else if(errortab[i].errornum == 43){
			fprintf(fpout, "while����Ӧ��Ϊ(\n");
			printf("while����Ӧ��Ϊ(\n");
		}
		else if(errortab[i].errornum == 44){
			fprintf(fpout, "return����\n");
			printf("return����\n");
		}
		else if(errortab[i].errornum == 45){
			fprintf(fpout, "Ӧ��Ϊ;\n");
			printf("Ӧ��Ϊ;\n");
		}
		else if(errortab[i].errornum == 46){
			fprintf(fpout, "�������������{\n");
			printf("�������������{\n");
		}
		else if(errortab[i].errornum == 47){
			fprintf(fpout, "main����������(\n");
			printf("main����������(\n");
		}
		else if(errortab[i].errornum == 48){
			fprintf(fpout, "���벻����\n");
			printf("���벻����\n");
		}
		else if(errortab[i].errornum == 49){
			fprintf(fpout, "Ӧ��Ϊ��ʶ��\n");
			printf("Ӧ��Ϊ��ʶ��\n");
		}
		else if(errortab[i].errornum == 50){
			fprintf(fpout, "Ӧ��Ϊint��char��void\n");
			printf("Ӧ��Ϊint��char��void\n");
		}
		else if(errortab[i].errornum == 51){
			fprintf(fpout, "void���ͺ���������return\n");
			printf("void���ͺ���������return\n");
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
	mipsfp = fopen("15061075_�Ű���_������.txt","w");
	int i;
	for(i = 0; i < mipsp; i++){
		if(mipstab[i].ordernum == 0){

		}
	}
	fclose(mipsfp);
}*/