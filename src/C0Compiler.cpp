// C0Compiler.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	fpin = fopen((char *)argv[1], "r");
	if(fpin == NULL){
		printf("Can not find the file %s\n", argv[1]);
		return 0;
	}
	fpout = fopen("result.txt", "w");
	nextch();
	inittempreg();
	program();
	fclose(fpin);
	fclose(fpout);
	return 0;
}

