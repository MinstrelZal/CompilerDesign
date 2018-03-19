#include "stdafx.h"

mctabitem mctab[MCMAX];
int mc;
char zaltemp[10][ALNG + 1] = {	"zaltemp0", "zaltemp1", "zaltemp2", "zaltemp3", "zaltemp4", 
								"zaltemp5", "zaltemp6", "zaltemp7", "zaltemp8", "zaltemp9"};

bool reginuse[10];

void inittempreg(){
	int i;
	for(i = 0; i < 10; i++){
		reginuse[i] = false;
	}
}

int regid(){
	int i;
	for(i = 0; i < 10; i++){
		if(!reginuse[i]){
			reginuse[i] = true;
			return i;
		}
	}
	// fatal
	printf("\nno more temp register\n");
	exit(0);
	return -1;
}

// emit middle code, -1 represents no use
void emitmidcode(int fctnum, mcfield x, mcfield y, mcfield z){
	if(mc == MCMAX){
		fatal(5);
	}
	mctab[mc].fctnum = fctnum;
	mctab[mc].x = x;
	mctab[mc].y = y;
	mctab[mc].z = z;
	mctab[mc].blocknum = -1;
	mc = mc + 1;
}

void printmctab(){
	int lastblock = -1;
	int i;
	int fctnum;
	mcfield x, y, z;
	FILE *midfp;
	midfp = fopen("15061075_张安澜_优化前中间代码.txt","w");
	for(i = 0; i < mc; i++){
		// printf("%d: ", i);
		fctnum = mctab[i].fctnum;
		x = mctab[i].x;
		y = mctab[i].y;
		z = mctab[i].z;
		/*if(fctnum >= 0 && fctnum <= 4){
			continue;
		}*/
		if(fctnum == 5){
			fprintf(midfp, "\n\n\n");
			lastblock = -1;
		}
		if(lastblock != mctab[i].blocknum){
			fprintf(midfp, "\nblock %d:\n\n", mctab[i].blocknum);
			lastblock = mctab[i].blocknum;
		}
		fprintf(midfp, "%d: ", i);
		if(fctnum == 0){  // const, int/char, a, 1
			if(z.type == 6){
				fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], fct[x.data.i], y.data.name, z.data.i);
				//printf("%s, %s, %s, '%c'\n", fct[fctnum], fct[x.data.i], y.data.name, z.data.i);
			}
			else{  // ints
				fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], fct[x.data.i], y.data.name, z.data.i);
				//printf("%s, %s, %s, %d\n", fct[fctnum], fct[x.data.i], y.data.name, z.data.i);
			}
		}
		else if(fctnum == 1){ // var, int/char, a,
			if(z.type == 3){
				fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], fct[x.data.i], y.data.name);
				//printf("%s, %s, %s, \n", fct[fctnum], fct[x.data.i], y.data.name);
			}
			else{
				fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], fct[x.data.i], y.data.name, z.data.i);
				//printf("%s, %s, %s, %d\n", fct[fctnum], fct[x.data.i], y.data.name, z.data.i);
			}
		}
		else if(fctnum == 5){
			if(z.type == 3){  // func, int/char/void, f,
				fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], fct[x.data.i], y.data.name);
				//printf("%s, %s, %s, \n", fct[fctnum], fct[x.data.i], y.data.name);
			}
			else{  // func, int/char/void, f, ()
				fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], fct[x.data.i], y.data.name, fct[z.data.i]);
				//printf("%s, %s, %s, %s\n", fct[fctnum], fct[x.data.i], y.data.name, fct[z.data.i]);
			}
		}
		else if(fctnum == 6){  // para, int/char, a
			fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], fct[x.data.i], y.data.name);
			//printf("%s, %s, %s, \n", fct[fctnum], fct[x.data.i], y.data.name);
		}
		else if(fctnum == 8){  // =
			if(x.type == 1){
				if(y.type == 0){
					fprintf(midfp, "%s, %s, %d, \n", fct[fctnum], x.data.name, y.data.i);
					//printf("%s, %s, %d, \n", fct[fctnum], x.data.name, y.data.i);
				}
				else if(y.type == 6){
					fprintf(midfp, "%s, %s, '%c', \n", fct[fctnum], x.data.name, y.data.i);
					//printf("%s, %s, '%c', \n", fct[fctnum], x.data.name, y.data.i);
				}
				else if(y.type == 1){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], x.data.name, y.data.name);
					//printf("%s, %s, %s, \n", fct[fctnum], x.data.name, y.data.name);
				}
				else if(y.type == 4){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], x.data.name, zaltemp[y.data.i]);
					//printf("%s, %s, t%d, \n", fct[fctnum], x.data.name, y.data.i);
				}
				else if(y.type == 5){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], x.data.name, y.data.name);
					//printf("%s, %s, %s, \n", fct[fctnum], x.data.name, y.data.name);
				}
			}
			else if(x.type == 4){
				if(y.type == 0){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], x.data.name, zaltemp[y.data.i]);
					//printf("%s, %s, %d, \n", fct[fctnum], x.data.name, y.data.i);
				}
				else if(y.type == 6){
					fprintf(midfp, "%s, %s, '%c', \n", fct[fctnum], x.data.name, y.data.i);
					//printf("%s, %s, '%c', \n", fct[fctnum], x.data.name, y.data.i);
				}
				else if(y.type == 1){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], zaltemp[x.data.i], y.data.name);
					//printf("%s, t%d, %s, \n", fct[fctnum], x.data.i, y.data.name);
				}
				else if(y.type == 4){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], zaltemp[x.data.i], zaltemp[y.data.i]);
					//printf("%s, t%d, t%d, \n", fct[fctnum], x.data.i, y.data.i);
				}
				else if(y.type == 5){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], zaltemp[x.data.i], y.data.name);
					//printf("%s, t%d, %s, \n", fct[fctnum], x.data.i, y.data.name);
				}
			}
		}
		else if(fctnum == 9){  // []=
			if(y.type == 0){
				if(z.type == 0){
					fprintf(midfp, "%s, %s, %d, %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					//printf("%s, %s, %d, %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
				}
				else if(z.type == 6){
					fprintf(midfp, "%s, %s, %d, '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					//printf("%s, %s, %d, '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
				}
				else if(z.type == 1){
					fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
					//printf("%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
				}
				else if(z.type == 4){
					fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, zaltemp[z.data.i]);
					//printf("%s, %s, %d, t%d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
				}
				else if(z.type == 5){
					fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
					//printf("%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
				}
			}
			else if(y.type == 1){
				if(z.type == 0){
					fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					//printf("%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
				else if(z.type == 6){
					fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					//printf("%s, %s, %s, '%c'\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
				else if(z.type == 1){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, z.data.name);
					//printf("%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, z.data.name);
				}
				else if(z.type == 4){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, zaltemp[z.data.i]);
					//printf("%s, %s, %s, t%d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
				else if(z.type == 5){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, z.data.name);
					//printf("%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, z.data.name);
				}
			}
			else if(y.type == 4){
				if(z.type = 0){
					fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.i);
					//printf("%s, %s, t%d, %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
				}
				else if(z.type == 6){
					fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.i);
					//printf("%s, %s, t%d, '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
				}
				else if(z.type == 1){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.name);
					//printf("%s, %s, t%d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
				}
				else if(z.type == 4){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, zaltemp[y.data.i], zaltemp[z.data.i]);
					//printf("%s, %s, t%d, t%d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
				}
				else if(z.type == 5){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.name);
					//printf("%s, %s, t%d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
				}
			}
		}
		else if(fctnum == 10){  // =[]
			if(x.type == 1){
				if(z.type == 0){
					fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					//printf("%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
				else if(z.type == 4){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, zaltemp[z.data.i]);
					//printf("%s, %s, %s, t%d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
			}
			else if(x.type == 4){
				if(z.type == 0){
					fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], zaltemp[x.data.i], y.data.name, z.data.i);
					//printf("%s, t%d, %s, %d\n", fct[fctnum], x.data.i, y.data.name, z.data.i);
				}
				else if(z.type == 1){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], y.data.name, z.data.name);
					//printf("%s, t%d, %s, %s\n", fct[fctnum], x.data.i, y.data.name, z.data.name);
				}
				else if(z.type == 4){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], y.data.name, zaltemp[z.data.i]);
					//printf("%s, t%d, %s, t%d\n", fct[fctnum], x.data.i, y.data.name, z.data.i);
				}
			}
			else if(x.type == 5){
				if(z.type == 0){
					fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					//printf("%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
				else if(z.type == 4){
					fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, zaltemp[z.data.i]);
					//printf("%s, %s, %s, t%d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
				}
			}
		}
		else if(fctnum >= 11 && fctnum <= 20){
			if(x.type == 1){
				if(y.type == 0){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, %d, %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
						//printf("%s, %s, %d, %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, %d, '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
						//printf("%s, %s, %d, '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
						//printf("%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], x.data.name, y.data.i, zaltemp[z.data.i]);
						//printf("%s, %s, %d, t%d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
				}
				else if(y.type == 6){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, '%c', %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
						//printf("%s, %s, '%c', %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, '%c', '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
						//printf("%s, %s, '%c', '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, '%c', %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
						//printf("%s, %s, '%c', %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, '%c', %s\n", fct[fctnum], x.data.name, y.data.i, zaltemp[z.data.i]);
						//printf("%s, %s, '%c', t%d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
				}
				else if(y.type == 1){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
						//printf("%s, %s, %s, %d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
						//printf("%s, %s, %s, '%c'\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, z.data.name);
						//printf("%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, y.data.name, zaltemp[z.data.i]);
						//printf("%s, %s, %s, t%d\n", fct[fctnum], x.data.name, y.data.name, z.data.i);
					}
				}
				else if(y.type == 4){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.i);
						//printf("%s, %s, t%d, %d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
					if(z.type == 6){
						fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.i);
						//printf("%s, %s, t%d, '%c'\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, zaltemp[y.data.i], z.data.name);
						//printf("%s, %s, t%d, %s\n", fct[fctnum], x.data.name, y.data.i, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, zaltemp[y.data.i], zaltemp[z.data.i]);
						//printf("%s, %s, t%d, t%d\n", fct[fctnum], x.data.name, y.data.i, z.data.i);
					}
				}
			}
			if(x.type == 4){
				if(y.type == 0){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, %d, %d\n", fct[fctnum], zaltemp[x.data.i], y.data.i, z.data.i);
						//printf("%s, t%d, %d, %d\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, %d, '%c'\n", fct[fctnum], zaltemp[x.data.i], y.data.i, z.data.i);
						//printf("%s, t%d, %d, '%c'\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], zaltemp[x.data.i], y.data.i, z.data.name);
						//printf("%s, t%d, %d, %s\n", fct[fctnum], x.data.i, y.data.i, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, %d, %s\n", fct[fctnum], zaltemp[x.data.i], y.data.i, zaltemp[z.data.i]);
						//printf("%s, t%d, %d, t%d\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
				}
				else if(y.type == 6){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, '%c', %d\n", fct[fctnum], zaltemp[x.data.i], y.data.i, z.data.i);
						//printf("%s, t%d, '%c', %d\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, '%c', '%c'\n", fct[fctnum], zaltemp[x.data.i], y.data.i, z.data.i);
						//printf("%s, t%d, '%c', '%c'\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, '%c', %s\n", fct[fctnum], zaltemp[x.data.i], y.data.i, z.data.name);
						//printf("%s, t%d, '%c', %s\n", fct[fctnum], x.data.i, y.data.i, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, '%c', %s\n", fct[fctnum], zaltemp[x.data.i], y.data.i, zaltemp[z.data.i]);
						//printf("%s, t%d, '%c', t%d\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
				}
				else if(y.type == 1){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], zaltemp[x.data.i], y.data.name, z.data.i);
						//printf("%s, t%d, %s, %d\n", fct[fctnum], x.data.i, y.data.name, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], zaltemp[x.data.i], y.data.name, z.data.i);
						//printf("%s, t%d, %s, '%c'\n", fct[fctnum], x.data.i, y.data.name, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], y.data.name, z.data.name);
						//printf("%s, t%d, %s, %s\n", fct[fctnum], x.data.i, y.data.name, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], y.data.name, zaltemp[z.data.i]);
						//printf("%s, t%d, %s, t%d\n", fct[fctnum], x.data.i, y.data.name, z.data.i);
					}
				}
				else if(y.type == 4){
					if(z.type == 0){
						fprintf(midfp, "%s, %s, %s, %d\n", fct[fctnum], zaltemp[x.data.i], zaltemp[y.data.i], z.data.i);
						//printf("%s, t%d, t%d, %d\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
					else if(z.type == 6){
						fprintf(midfp, "%s, %s, %s, '%c'\n", fct[fctnum], zaltemp[x.data.i], zaltemp[y.data.i], z.data.i);
						//printf("%s, t%d, t%d, '%c'\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
					else if(z.type == 1){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], zaltemp[y.data.i], z.data.name);
						//printf("%s, t%d, t%d, %s\n", fct[fctnum], x.data.i, y.data.i, z.data.name);
					}
					else if(z.type == 4){
						fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], zaltemp[y.data.i], zaltemp[z.data.i]);
						//printf("%s, t%d, t%d, t%d\n", fct[fctnum], x.data.i, y.data.i, z.data.i);
					}
				}
			}
		}
		else if(fctnum == 21 || fctnum ==22){
			if(x.type == 0){
				fprintf(midfp, "%s, %d, label%d, \n", fct[fctnum], x.data.i, y.data.i);
				//printf("%s, %d, label%d, \n", fct[fctnum], x.data.i, y.data.i);
			}
			else if(x.type == 1){
				fprintf(midfp, "%s, %s, label%d, \n", fct[fctnum], x.data.name, y.data.i);
				//printf("%s, %s, label%d, \n", fct[fctnum], x.data.name, y.data.i);
			}
			else if(x.type == 4){
				fprintf(midfp, "%s, %s, label%d, \n", fct[fctnum], zaltemp[x.data.i], y.data.i);
				//printf("%s, t%d, label%d, \n", fct[fctnum], x.data.i, y.data.i);
			}
		}
		else if(fctnum == 23){
			fprintf(midfp, "%s, label%d, , \n", fct[fctnum], x.data.i);
			//printf("%s, label%d, , \n", fct[fctnum], x.data.i);
		}
		else if(fctnum == 25){
			fprintf(midfp, "%s, label%d, , \n", fct[fctnum], x.data.i);
			//printf("%s, label%d, , \n", fct[fctnum], x.data.i);
		}
		else if(fctnum == 26){  // return
			if(x.type == 3){
				fprintf(midfp, "%s, , , \n", fct[fctnum]);
				//printf("%s, , , \n", fct[fctnum]);
			}
			else{
				if(y.type == 0){
					fprintf(midfp, "%s, %s, %d, \n", fct[fctnum], x.data.name, y.data.i);
					//printf("%s, %s, %d, \n", fct[fctnum], x.data.name, y.data.i);
				}
				if(y.type == 6){
					fprintf(midfp, "%s, %s, '%c', \n", fct[fctnum], x.data.name, y.data.i);
					//printf("%s, %s, '%c', \n", fct[fctnum], x.data.name, y.data.i);
				}
				else if(y.type == 1){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], x.data.name, y.data.name);
					//printf("%s, %s, %s, \n", fct[fctnum], x.data.name, y.data.name);
				}
				else if(y.type == 4){
					fprintf(midfp, "%s, %s, %s, \n", fct[fctnum], x.data.name, zaltemp[y.data.i]);
					//printf("%s, %s, t%d, \n", fct[fctnum], x.data.name, y.data.i);
				}
			}
		}
		else if(fctnum == 27){
			fprintf(midfp, "%s, %s, , \n", fct[fctnum], x.data.name);
			//printf("%s, %s, , \n", fct[fctnum], x.data.name);
		}
		else if(fctnum == 28){  // read
			fprintf(midfp, "%s, %s, , \n", fct[fctnum], x.data.name);
			//printf("%s, %s, , \n", fct[fctnum], x.data.name);
		}
		else if(fctnum == 29){  // write
			if(x.type == 0){
				fprintf(midfp, "%s, %d, , \n", fct[fctnum], x.data.i);
				//printf("%s, %d, , \n", fct[fctnum], x.data.i);
			}
			if(x.type == 6){
				if(x.data.i == 10){
					fprintf(midfp, "%s, '\\n', , \n", fct[fctnum]);
					//printf("%s, '\\n', , \n", fct[fctnum]);
				}
				else{
					fprintf(midfp, "%s, '%c', , \n", fct[fctnum], x.data.i);
					//printf("%s, '%c', , \n", fct[fctnum], x.data.i);
				}
			}
			else if(x.type == 1){
				fprintf(midfp, "%s, %s, , \n", fct[fctnum], x.data.name);
				//printf("%s, %s, , \n", fct[fctnum], x.data.name);
			}
			else if(x.type == 4){
				fprintf(midfp, "%s, %s, , \n", fct[fctnum], zaltemp[x.data.i]);
				//printf("%s, t%d, , \n", fct[fctnum], x.data.i);
			}
		}
		else if(fctnum == 30){
			if(x.type == 0){
				fprintf(midfp, "%s, %d, %s, %s\n", fct[fctnum], x.data.i, tab[y.data.i].name, z.data.name);
				//printf("%s, %d, %s, %s\n", fct[fctnum], x.data.i, tab[y.data.i].name, z.data.name);
			}
			if(x.type == 6){
				fprintf(midfp, "%s, '%c', %s, %s\n", fct[fctnum], x.data.i, tab[y.data.i].name, z.data.name);
				//printf("%s, '%c', %s, %s\n", fct[fctnum], x.data.i, tab[y.data.i].name, z.data.name);
			}
			else if(x.type == 1){
				fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], x.data.name, tab[y.data.i].name, z.data.name);
				//printf("%s, %s, %s, %s\n", fct[fctnum], x.data.name, tab[y.data.i].name, z.data.name);
			}
			else if(x.type == 4){
				fprintf(midfp, "%s, %s, %s, %s\n", fct[fctnum], zaltemp[x.data.i], tab[y.data.i].name, z.data.name);
				//printf("%s, t%d, %s, %s\n", fct[fctnum], x.data.i, tab[y.data.i].name, z.data.name);
			}
		}
	}
	printf("\nmid code line = %d\n", mc);
	fclose(midfp);
}