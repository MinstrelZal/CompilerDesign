#include "stdafx.h"

typedef struct edge{
	int tailvid;  // 反向
	int headvid;  // 正向
	struct edge* tailnext;  // 反向
	struct edge* headnext;  // 正向
}edge;

typedef struct vertex{
	mcfield node;
	int fctnum;
	bool indag;
	bool inqueue;
	edge* rowlink;  // 正向
	edge* collink;  // 反向
}vertex;

typedef struct relation{
	vertex v;
	// int mctabpos;
	int id;
}relation;

relation rtab[100];
int r;

int totalvid = 0;

mctabitem opt_mctab[MCMAX];
int opt_mc = 0;

int nodeid(int mctabpos, int vtype, bool alter, int midvid){
	int i;
	mcfield temp;
	if(vtype == 1){
		temp = mctab[mctabpos].x;
	}
	else if(vtype == 2){
		temp = mctab[mctabpos].y;
	}
	else if(vtype == 3){
		temp = mctab[mctabpos].z;
	}
	else{
		return -1;
	}
	if(alter){
		if(temp.type == 1){
			for(i = 0; i < r; i++){
				if(rtab[i].v.fctnum != -1){
					continue;
				}
				if(rtab[i].v.node.type == temp.type){
					if(strcmp(rtab[i].v.node.data.name, temp.data.name) == 0){
							rtab[r].v = rtab[i].v;
							rtab[r].v.indag = false;
							rtab[r].v.rowlink = NULL;
							rtab[r].v.collink = NULL;
							rtab[r].id = midvid;
							r = r + 1;
							return rtab[r-1].id;
					}
				}
			}
		}
		else{
			for(i = 0; i < r; i++){
				if(rtab[i].v.fctnum != -1){
					continue;
				}
				if(rtab[i].v.node.type == temp.type){
					if(rtab[i].v.node.data.i == temp.data.i){
						if(temp.type == 4){
							rtab[r].v = rtab[i].v;
							rtab[r].v.indag = false;
							rtab[r].v.rowlink = NULL;
							rtab[r].v.collink = NULL;
							rtab[r].id = midvid;
							r = r + 1;
							return rtab[r-1].id;
						}
						else{
							rtab[i].id = midvid;
							return rtab[i].id;
						}
					}
				}
			}
		}
	}
	else{
		int maxpos = -1;
		if(temp.type == 1){
			for(i = 0; i < r; i++){
				if(rtab[i].v.fctnum != -1){
					continue;
				}
				if(rtab[i].v.node.type == temp.type && strcmp(rtab[i].v.node.data.name, temp.data.name) == 0){
					maxpos = (rtab[i].id > maxpos) ? rtab[i].id : maxpos;
				}
			}
			return maxpos;
		}
		else{
			for(i = 0; i < r; i++){
				if(rtab[i].v.fctnum != -1){
					continue;
				}
				if(rtab[i].v.node.type == temp.type && rtab[i].v.node.data.i == temp.data.i){
					maxpos = (rtab[i].id > maxpos) ? rtab[i].id : maxpos;
				}
			}
			return maxpos;
		}
	}
	return -1;
	/*if(temp.type == 1){
		for(i = 0; i < r; i++){
			if(rtab[i].v.fctnum != -1){
				continue;
			}
			if(rtab[i].v.node.type == temp.type && !rtab[i].v.indag){
				if(strcmp(rtab[i].v.node.data.name, temp.data.name) == 0){
					if(mctab[mctabpos].fctnum == 9){
						if(alter){
							rtab[r].v = rtab[i].v;
							rtab[r].v.indag = false;
							rtab[r].v.rowlink = NULL;
							rtab[r].v.collink = NULL;
							rtab[r].id = midvid;
							r = r + 1;
							return rtab[r-1].id;
						}
						return rtab[i].id;
					}
					else{
						if(alter){
							rtab[r].v = rtab[i].v;
							rtab[r].v.indag = false;
							rtab[r].v.rowlink = NULL;
							rtab[r].v.collink = NULL;
							rtab[r].id = midvid;
							r = r + 1;
							return rtab[r-1].id;
						}
						return rtab[i].id;
					}
				}
			}
		}
		for(i = 0; i < r; i++){
			if(rtab[i].v.fctnum != -1){
				continue;
			}
			if(rtab[i].v.node.type == temp.type && rtab[i].v.indag){
				if(strcmp(rtab[i].v.node.data.name, temp.data.name) == 0){
					if(alter){
						rtab[r].v = rtab[i].v;
						rtab[r].v.indag = false;
						rtab[r].v.rowlink = NULL;
						rtab[r].v.collink = NULL;
						rtab[r].id = midvid;
						r = r + 1;
						return rtab[r-1].id;
					}
					return rtab[i].id;
				}
			}
		}
	}
	else{
		for(i = 0; i < r; i++){
			if(rtab[i].v.fctnum != -1){
				continue;
			}
			if(rtab[i].v.node.type == temp.type){
				if(rtab[i].v.node.data.i == temp.data.i){
					if(temp.type == 4){
						if(alter){
							rtab[r].v = rtab[i].v;
							rtab[r].v.indag = false;
							rtab[r].v.rowlink = NULL;
							rtab[r].v.collink = NULL;
							rtab[r].id = midvid;
							r = r + 1;
							return rtab[r-1].id;
						}
						return rtab[i].id;
					}
					if(alter){
						rtab[i].id = midvid;
					}
					return rtab[i].id;
				}
			}
		}
	}
	return -1;*/
}

int createvex(int mctabpos, int vtype, int midvid){  // vtype: 0:op, 1:x, 2:y, 3:z
	vertex v;
	if(vtype == 0){
		v.fctnum = mctab[mctabpos].fctnum;
		v.indag = true;
	}
	else{
		v.fctnum = -1;
		if(vtype == 1){
			v.node = mctab[mctabpos].x;
			v.indag = false;
		}
		else if(vtype == 2){
			v.node = mctab[mctabpos].y;
			v.indag = true;
		}
		else if(vtype == 3){
			v.node = mctab[mctabpos].z;
			v.indag = true;
		}
		else{
			return -1;
		}
	}
	v.inqueue = false;
	v.rowlink = NULL;
	v.collink = NULL;
	rtab[r].v = v;
	// rtab[r].mctabpos = mctabpos;
	if(vtype == 1){
		rtab[r].id = midvid;
	}
	else{
		rtab[r].id = totalvid++;
	}
	r = r + 1;
	return rtab[r-1].id;
}

int midvexid(int fctnum, int leftvid, int rightvid){
	int i;
	edge* p = NULL;
	bool lefttrue = false;
	bool righttrue = false;
	for(i = 0; i < r; i++){
		lefttrue = false;
		righttrue = false;
		if(rtab[i].v.indag && rtab[i].v.fctnum == fctnum){
			p = rtab[i].v.rowlink;
			while(p != NULL){
				if(p->headvid == leftvid){
					lefttrue = true;
				}
				if(p->headvid == rightvid){
					righttrue = true;
				}
				if(fctnum == 8 && lefttrue){
					return rtab[i].id;
				}
				if(fctnum != 8 && lefttrue && righttrue){
					return rtab[i].id;
				}
				p = p->headnext;
			}
		}
	}
	return -1;
}

void setedge(int midvid, int leftvid, int rightvid){
	int i;
	edge* p = NULL;
	for(i = 0; i < r; i++){
		if(rtab[i].v.indag && rtab[i].id == midvid && rtab[i].v.fctnum != -1){
			rtab[i].v.rowlink = (edge*)malloc(sizeof(edge));
			rtab[i].v.rowlink->headvid = leftvid;
			rtab[i].v.rowlink->tailvid = -1;
			rtab[i].v.rowlink->headnext = NULL;
			rtab[i].v.rowlink->tailnext = NULL;
			int j;
			for(j = 0; j < r; j++){  // 建立逆邻接表
				if(rtab[j].id == leftvid && rtab[j].v.indag){
					if(rtab[j].v.collink == NULL){
						rtab[j].v.collink = (edge*)malloc(sizeof(edge));
						rtab[j].v.collink->tailvid = midvid;
						rtab[j].v.collink->headvid = -1;
						rtab[j].v.collink->tailnext = NULL;
						rtab[j].v.collink->headnext = NULL;
					}
					else{
						p = rtab[j].v.collink;
						while(p->tailnext != NULL){
							p = p->tailnext;
						}
						p->tailnext = (edge*)malloc(sizeof(edge));
						p->tailnext->tailvid = midvid;
						p->tailnext->headvid = -1;
						p->tailnext->tailnext = NULL;
						p->tailnext->headnext = NULL;
					}
					break;
				}
			}
			if(rightvid != -1){
				p = (edge*)malloc(sizeof(edge));
				p->headvid = rightvid;
				p->tailvid = -1;
				p->headnext = NULL;
				p->tailnext = NULL;
				rtab[i].v.rowlink->headnext = p;
				int j;
				for(j = 0; j < r; j++){  // 建立逆邻接表
					if(rtab[j].id == rightvid && rtab[j].v.indag){
						if(rtab[j].v.collink == NULL){
							rtab[j].v.collink = (edge*)malloc(sizeof(edge));
							rtab[j].v.collink->tailvid = midvid;
							rtab[j].v.collink->headvid = -1;
							rtab[j].v.collink->tailnext = NULL;
							rtab[j].v.collink->headnext = NULL;
						}
						else{
							p = rtab[j].v.collink;
							while(p->tailnext != NULL){
								p = p->tailnext;
							}
							p->tailnext = (edge*)malloc(sizeof(edge));
							p->tailnext->tailvid = midvid;
							p->tailnext->headvid = -1;
							p->tailnext->tailnext = NULL;
							p->tailnext->headnext = NULL;
						}
						break;
					}
				}
			}
			break;
		}
	}
}

void setdag(int blockbegin, int blockend){
	int i;
	r = 0;
	totalvid = 0;
	int leftvid = -1;
	int rightvid = -1;
	int midvid = -1;
	int resultvid = -1;
	for(i = blockbegin; i < blockend; i++){
		if(!(mctab[i].fctnum >= 8 && mctab[i].fctnum <= 14)){
			continue;
		}
		leftvid = -1;
		rightvid = -1;
		midvid = -1;
		resultvid = -1;
		leftvid = nodeid(i, 2, false, -1); // 查找左节点
		if(leftvid == -1){  // 没有则新建一个
			leftvid = createvex(i, 2, -1);
		}
		if(mctab[i].fctnum != 8){
			rightvid = nodeid(i, 3, false, -1);
			if(rightvid == -1){
				rightvid = createvex(i, 3, -1);
			}
		}
		else{
			rightvid = -1;
		}
		midvid = midvexid(mctab[i].fctnum, leftvid, rightvid);  // 查找中间节点
		if(midvid == -1){  // 没有则新建一个
			midvid = createvex(i, 0, -1);
			setedge(midvid, leftvid, rightvid);
		}
		resultvid = nodeid(i, 1, true, midvid);
		if(resultvid == -1){
			resultvid = createvex(i, 1, midvid);
		}
	}
	for(i = 0; i < r; i++){
		fprintf(fpout, "标号: %d\n", rtab[i].id);
		if(rtab[i].v.fctnum != -1){
			fprintf(fpout, "节点 %s 的邻接节点有:", fct[rtab[i].v.fctnum]);
		}
		else{
			if(rtab[i].v.node.type == 1){
				fprintf(fpout, "节点 %s 的邻接节点有: ", rtab[i].v.node.data.name);
			}
			else if(rtab[i].v.node.type == 0){
				fprintf(fpout, "节点 %d 的邻接节点有: ", rtab[i].v.node.data.i);
			}
			else if(rtab[i].v.node.type == 4){
				fprintf(fpout, "节点 zaltemp%d 的邻接节点有: ", rtab[i].v.node.data.i);
			}
			else if(rtab[i].v.node.type == 5){
				fprintf(fpout, "节点 RET 的邻接节点有: ");
			}
			else if(rtab[i].v.node.type == 6 || rtab[i].v.node.type == 7){
				fprintf(fpout, "节点 '%c' 的邻接节点有: ", rtab[i].v.node.data.i);
			}
		}
		edge* p = rtab[i].v.rowlink;
		while(p != NULL){
			fprintf(fpout, "%d ", p->headvid);
			p = p->headnext;
		}
		fprintf(fpout, "\n");
		if(rtab[i].v.fctnum != -1){
			fprintf(fpout, "节点 %s 的逆邻接节点有:", fct[rtab[i].v.fctnum]);
		}
		else{
			if(rtab[i].v.node.type == 1){
				fprintf(fpout, "节点 %s 的逆邻接节点有: ", rtab[i].v.node.data.name);
			}
			else if(rtab[i].v.node.type == 0){
				fprintf(fpout, "节点 %d 的逆邻接节点有: ", rtab[i].v.node.data.i);
			}
			else if(rtab[i].v.node.type == 4){
				fprintf(fpout, "节点 zaltemp%d 的逆邻接节点有: ", rtab[i].v.node.data.i);
			}
			else if(rtab[i].v.node.type == 5){
				fprintf(fpout, "节点 RET 的逆邻接节点有: ");
			}
			else if(rtab[i].v.node.type == 6 || rtab[i].v.node.type == 7){
				fprintf(fpout, "节点 '%c' 的逆邻接节点有: ", rtab[i].v.node.data.i);
			}
		}
		p = rtab[i].v.collink;
		while(p != NULL){
			fprintf(fpout, "%d ", p->tailvid);
			p = p->tailnext;
		}
		fprintf(fpout, "\n");
	}
}

bool candel(int rpos){
	if(rtab[rpos].v.collink == NULL){
		return true;
	}
	else{
		edge* p = rtab[rpos].v.collink;
		bool alldel = true;
		while(p != NULL){
			for(int i = 0; i < r; i++){
				if(rtab[i].id == p->tailvid && rtab[i].v.indag && !rtab[i].v.inqueue){
					alldel = false;
					break;
				}
			}
			if(!alldel){
				break;
			}
			p = p->tailnext;
		}
		if(alldel){
			return true;
		}
	}
	return false;
}

void exportcode(int blocknum, int blockbegin, int blockend){
	relation vqueue[100];
	mctabitem opt_code[100];
	int vq = 0;
	int optc = 0;
	int i, j;
	bool allinqueue = false;
	while(!allinqueue){
		allinqueue = true;
		for(i = 0; i < r; i++){
			if(rtab[i].v.indag && rtab[i].v.rowlink != NULL && !rtab[i].v.inqueue){  // 未进入队列的中间节点
				if(allinqueue){
					allinqueue = false;
				}
				int tempi = i;
				while(candel(tempi)){
					rtab[tempi].v.inqueue = true;
					vqueue[vq++] = rtab[tempi];
					bool find = false;
					for(int k = 0; k < r; k++){
						if(rtab[k].id == rtab[tempi].v.rowlink->headvid && rtab[k].v.indag && rtab[k].v.rowlink != NULL && !rtab[k].v.inqueue){
							find = true;
							tempi = k;
							break;
						}
					}
					if(!find){
						break;
					}
				}
				/*if(candel(i)){
					rtab[i].v.inqueue = true;
					vqueue[vq++] = rtab[i];
				}*/
			}
		}
	}
	fprintf(fpout, "vqueue: ");
	for(i = 0; i < vq; i++){
		fprintf(fpout, "%d ", vqueue[i].id);
	}
	fprintf(fpout, "\n");
	bool first = true;
	mcfield firstitem;
	for(i = vq - 1; i >= 0; i--){
		first = true;
		for(j = 0; j < r; j++){
			if(rtab[j].id == vqueue[i].id && !rtab[j].v.indag){
				opt_code[optc].blocknum = blocknum;
				opt_code[optc].x = rtab[j].v.node;
				if(first){
					opt_code[optc].fctnum = vqueue[i].v.fctnum;
					for(int k = 0; k < r; k++){
						if(rtab[k].id == vqueue[i].v.rowlink->headvid && rtab[k].v.fctnum == -1){
							opt_code[optc].y = rtab[k].v.node;
							break;
						}
					}
					firstitem = rtab[j].v.node;
					first = false;
				}
				else{
					opt_code[optc].fctnum = 8;
					opt_code[optc].y = firstitem;
				}
				if(opt_code[optc].fctnum != 8){
					for(int k = 0; k < r; k++){
						if(rtab[k].id == vqueue[i].v.rowlink->headnext->headvid && rtab[k].v.fctnum == -1){
							opt_code[optc].z = rtab[k].v.node;
							break;
						}
					}
				}
				else{
					opt_code[optc].z.type = 3;
				}
				optc = optc + 1;
			}
		}
	}
	bool hasexported = false;
	for(i = blockbegin; i < blockend; i++){
		if(!(mctab[i].fctnum >= 8 && mctab[i].fctnum <= 14)){
			opt_mctab[opt_mc++] = mctab[i];
		}
		else{
			if(!hasexported){
				for(j = 0; j < optc; j++){
					opt_mctab[opt_mc++] = opt_code[j];
				}
				hasexported = true;
			}
		}
	}
}

void optimize_dag(){
	int i, j;
	int blockbegin;
	int blockend;
	bool hascall = false;
	bool nocalculate = true;
	for(i = 0; i < mc; i++){
		if(mctab[i].fctnum == 5){
			opt_mctab[opt_mc++] = mctab[i];  // func语句原样输出
			blockbegin = i + 1;
			hascall = false;
			nocalculate = true;
			for(j = i + 1; mctab[j].fctnum != 5 && j < mc; j++){
				if(j > i + 1 && mctab[j].blocknum != mctab[j-1].blocknum){
					fprintf(fpout, "block %d:\n\n", mctab[j-1].blocknum);
					blockend = j;
					if(hascall || nocalculate){  // 有函数调用的不优化, 没有表达式计算的不优化
						for(int k = blockbegin; k < blockend; k++){
							opt_mctab[opt_mc++] = mctab[k];
						}
					}
					else{
						setdag(blockbegin, blockend);
						exportcode(mctab[j-1].blocknum, blockbegin, blockend);
					}
					blockbegin = j;
					hascall = false;
					nocalculate = true;
				}
				if(!hascall && mctab[j].fctnum == 27){  // call
					hascall = true;
				}
				if(nocalculate && mctab[j].fctnum >= 8 && mctab[j].fctnum <= 14){  // +,-,*,/,[]=,=[]
					nocalculate = false;
				}
			}
			fprintf(fpout, "block %d:\n\n", mctab[j-1].blocknum);
			blockend = j;
			setdag(blockbegin, blockend);
			exportcode(mctab[j-1].blocknum, blockbegin, blockend);
			i = j - 1;
		}
		else{
			opt_mctab[opt_mc++] = mctab[i];  // 函数之间的语句原样输出
		}
	}
}

void block(){
	int i,j;
	int blockid;
	int funcbegin;
	int funcend;
	for(i = 0; i < mc; i++){
		if(mctab[i].fctnum == 5){
			funcbegin = i;
			blockid = 0;
			int lastblockid = blockid;
			for(j = i + 1; mctab[j].fctnum != 5 && j < mc; j++){
				if(mctab[j].fctnum == 25){  // setlab
					if(lastblockid == blockid){
						blockid = blockid + 1;
					}
					mctab[j].blocknum = blockid;
					lastblockid = blockid;
				}
				else{
					mctab[j].blocknum = blockid;
					if(lastblockid != blockid){
						lastblockid = blockid;
					}
					if(mctab[j].fctnum >= 21 && mctab[j].fctnum <= 23){
						blockid = blockid + 1;
					}
					else if(mctab[j].fctnum == 29 && (mctab[j].x.type == 1 || mctab[j].x.type == 4)){
						blockid = blockid + 1;
					}
				}
			}
			funcend = j;
			// printf("\nfuncbegin = %d, funcend = %d\n", funcbegin, funcend);
			i = j - 1;
		}
	}
}

void printoptmctab(){
	int lastblock = -1;
	int i;
	int fctnum;
	mcfield x, y, z;
	FILE *midfp;
	midfp = fopen("15061075_张安澜_优化后中间代码.txt","w");
	for(i = 0; i < opt_mc; i++){
		// printf("%d: ", i);
		fctnum = opt_mctab[i].fctnum;
		x = opt_mctab[i].x;
		y = opt_mctab[i].y;
		z = opt_mctab[i].z;
		/*if(fctnum >= 0 && fctnum <= 4){
			continue;
		}*/
		if(fctnum == 5){
			fprintf(midfp, "\n\n\n");
			lastblock = -1;
		}
		if(lastblock != opt_mctab[i].blocknum){
			fprintf(midfp, "\nblock %d:\n\n", opt_mctab[i].blocknum);
			lastblock = opt_mctab[i].blocknum;
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
	printf("\nopt mid code line = %d\n", opt_mc);
	fclose(midfp);
}

void emitoptmips(){
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
	for(i = 0; i < opt_mc; i++){
		if(opt_mctab[i].fctnum == 5){  // func, 更新tab表中函数的起始地址adr
			int ytpos;
			ytpos = loc(opt_mctab[i].y.data.name, 0);
			tab[ytpos].adr = textpoint;
			nowf = tab[ytpos].ref;
			// set func begin label
			mips_func(28, ytpos);
			//if(strcmp(opt_mctab[i].y.data.name, key[7]) != 0){
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
		if(opt_mctab[i].fctnum == 8){  // =
			if(opt_mctab[i].x.type == 1){
				int xtpos;
				xtpos = loc(opt_mctab[i].x.data.name, nowf);
				if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].y.data.i);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*tab[xtpos].adr);
				}
				else if(opt_mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $t0,offset(base)
					mips7(23, 8, base, 4*tab[xtpos].adr);
				}
				else if(opt_mctab[i].y.type == 1){
					int ytpos;
					ytpos = loc(opt_mctab[i].y.data.name, nowf);
					int base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*tab[xtpos].adr);
				}
			}
			else if(opt_mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				if(opt_mctab[i].y.type == 5){
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
		if(opt_mctab[i].fctnum == 9){  // []=
			int xtpos = loc(opt_mctab[i].x.data.name, nowf);
			if(opt_mctab[i].y.type == 0){
				if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].z.data.i);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*(tab[xtpos].adr+opt_mctab[i].y.data.i));
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
					int base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset($base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $s0,offset(base)
					mips7(23, 16, base, 4*(tab[xtpos].adr+opt_mctab[i].y.data.i));
				}
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// sw $t0,offset(base)
					mips7(23, 8, base, 4*(tab[xtpos].adr+opt_mctab[i].y.data.i));
				}
			}
			else if(opt_mctab[i].y.type == 1){
				int ytpos = loc(opt_mctab[i].y.data.name, nowf);
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
				if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].z.data.i);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sw $t0,offset($s1)
					mips7(23, 8, 17, 0);
				}
			}
			else if(opt_mctab[i].y.type == 4){
				int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
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
				if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].z.data.i);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// sw $s0,offset($s1)
					mips7(23, 16, 17, 0);
				}
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sw $t0,offset($s1)
					mips7(23, 8, 17, 0);
				}
			}
		}
		else if(opt_mctab[i].fctnum == 10){  // =[]
			int base;
			int ytpos = loc(opt_mctab[i].y.data.name, nowf);
			if(opt_mctab[i].x.type == 1){
				int xtpos = loc(opt_mctab[i].x.data.name, nowf);
				if(opt_mctab[i].z.type == 0){
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips2(24, 16, base, 4*(tab[ytpos].adr+opt_mctab[i].z.data.i));
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
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
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
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
			else if(opt_mctab[i].x.type == 4){
				int base;
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				if(opt_mctab[i].z.type == 0){
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $t0,offset(base)
					mips7(24, 8, base, 4*(tab[ytpos].adr+opt_mctab[i].z.data.i));
					// sw $t0,offset($s6)
					mips7(23, 8, 22, 4*tab[xtpos].adr);
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
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
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
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
		else if(opt_mctab[i].fctnum == 11 || opt_mctab[i].fctnum == 12){  // x / 
			int base;
			if(opt_mctab[i].x.type == 1){
				int xtpos = loc(opt_mctab[i].x.data.name, nowf);
				if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].y.data.i);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s2,$zero,immediate
						mips2(1, 18, 0, opt_mctab[i].z.data.i);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
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
				else if(opt_mctab[i].y.type == 1){
					int ytpos = loc(opt_mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s2,$zero,immediate
						mips2(1, 18, 0, opt_mctab[i].z.data.i);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s2
							mips3(5, 16, 18);
						}
						else{
							// div $s0,$s2
							mips3(8, 16, 18);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
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
				else if(opt_mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s0,$zero,immediate
						mips2(1, 16, 0, opt_mctab[i].z.data.i);
						if(opt_mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t1,offset($s6)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
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
			else if(opt_mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].y.data.i);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s1,$zero,immediate
						mips2(1, 17, 0, opt_mctab[i].z.data.i);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$t0
							mips3(5, 16, 8);
						}
						else{
							// div $s0,$t0
							mips3(8, 16, 8);
						}
					}
				}
				else if(opt_mctab[i].y.type == 1){
					int ytpos = loc(opt_mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s1,$zero,immediate
						mips2(1, 17, 0, opt_mctab[i].z.data.i);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$s1
							mips3(5, 16, 17);
						}
						else{
							// div $s0,$s1
							mips3(8, 16, 17);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $s0,$t0
							mips3(5, 16, 8);
						}
						else{
							// div $s0,$t0
							mips3(8, 16, 8);
						}
					}
				}
				else if(opt_mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s0,$zero,immediate
						mips2(1, 16, 0, opt_mctab[i].z.data.i);
						if(opt_mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
							// mult $t0,$s0
							mips3(5, 8, 16);
						}
						else{
							// div $t0,$s0
							mips3(8, 8, 16);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t1,offset($s6)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 11){
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
		else if(opt_mctab[i].fctnum == 13 || opt_mctab[i].fctnum == 14){  // + - 
			int base;
			if(opt_mctab[i].x.type == 1){
				int xtpos = loc(opt_mctab[i].x.data.name, nowf);
				if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].y.data.i);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s0,$s0,immediate
						if(opt_mctab[i].fctnum == 13){
							mips2(1, 16, 16, opt_mctab[i].z.data.i);
						}
						else{
							mips2(1, 16, 16, -opt_mctab[i].z.data.i);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $s0,$s0,$s2
							mips1(0, 16, 16, 18);
						}
						else{
							// sub $s0,$s0,$s2
							mips1(4, 16, 16, 18);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $s0,$s0,$t0
							mips1(0, 16, 16, 8);
						}
						else{
							// sub $s0,$s0,$t0
							mips1(4, 16, 16, 8);
						}
					}
				}
				else if(opt_mctab[i].y.type == 1){
					int ytpos = loc(opt_mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s0,$s0,immediate
						if(opt_mctab[i].fctnum == 13){
							mips2(1, 16, 16, opt_mctab[i].z.data.i);
						}
						else{
							mips2(1, 16, 16, -opt_mctab[i].z.data.i);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s2,offset(base)
						mips7(24, 18, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $s0,$s0,$s2
							mips1(0, 16, 16, 18);
						}
						else{
							// sub $s0,$s0,$s2
							mips1(4, 16, 16, 18);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $s0,$s0,$t0
							mips1(0, 16, 16, 8);
						}
						else{
							// sub $s0,$s0,$t0
							mips1(4, 16, 16, 8);
						}
					}
				}
				else if(opt_mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $s0,$t0,immediate
						if(opt_mctab[i].fctnum == 13){
							mips2(1, 16, 8, opt_mctab[i].z.data.i);
						}
						else{
							mips2(1, 16, 8, -opt_mctab[i].z.data.i);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $s0,$t0,$s0
							mips1(0, 16, 8, 16);
						}
						else{
							// sub $s0,$t0,$s0
							mips1(4, 16, 8, 16);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t1,offset($s6)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
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
			else if(opt_mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].y.data.i);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $t0,$s0,immediate
						if(opt_mctab[i].fctnum == 13){
							mips2(1, 8, 16, opt_mctab[i].z.data.i);
						}
						else{
							mips2(1, 8, 16, -opt_mctab[i].z.data.i);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $t0,$s0,$s1
							mips1(0, 8, 16, 17);
						}
						else{
							// sub $t0,$s0,$s1
							mips1(4, 8, 16, 17);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $t0,$s0,$t0
							mips1(0, 8, 16, 8);
						}
						else{
							// sub $t0,$s0,$t0
							mips1(4, 8, 16, 8);
						}
					}
				}
				else if(opt_mctab[i].y.type == 1){
					int ytpos = loc(opt_mctab[i].y.data.name, nowf);
					base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $t0,$s0,immediate
						if(opt_mctab[i].fctnum == 13){
							mips2(1, 8, 16, opt_mctab[i].z.data.i);
						}
						else{
							mips2(1, 8, 16, -opt_mctab[i].z.data.i);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s1,offset(base)
						mips7(24, 17, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $t0,$s0,$s1
							mips1(0, 8, 16, 17);
						}
						else{
							// sub $t0,$s0,$s1
							mips1(4, 8, 16, 17);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t0,offset($s6)
						mips7(24, 8, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $t0,$s0,$t0
							mips1(0, 8, 16, 8);
						}
						else{
							// sub $t0,$s0,$t0
							mips1(4, 8, 16, 8);
						}
					}
				}
				else if(opt_mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ytpos].adr);
					if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
						// addi $t0,$t0,immediate
						if(opt_mctab[i].fctnum == 13){
							mips2(1, 8, 8, opt_mctab[i].z.data.i);
						}
						else{
							mips2(1, 8, 8, -opt_mctab[i].z.data.i);
						}
					}
					else if(opt_mctab[i].z.type == 1){
						int ztpos = loc(opt_mctab[i].z.data.name, nowf);
						base = (tab[ztpos].findex == 0) ? 21 : 22;
						// lw $s0,offset(base)
						mips7(24, 16, base, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
							// add $t0,$t0,$s0
							mips1(0, 8, 8, 16);
						}
						else{
							// sub $t0,$t0,$s0
							mips1(4, 8, 8, 16);
						}
					}
					else if(opt_mctab[i].z.type == 4){
						int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
						// lw $t1,offset(base)
						mips7(24, 9, 22, 4*tab[ztpos].adr);
						if(opt_mctab[i].fctnum == 13){
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
		else if(opt_mctab[i].fctnum >= 15 && opt_mctab[i].fctnum <= 20){
			in_condition = true;
			condition_op = opt_mctab[i].fctnum;
			int base;
			int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
			if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
				// addi $s0,$zero,immediate
				mips2(1, 16, 0, opt_mctab[i].y.data.i);
				if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
					// addi $t0,$s0,-immediate
					mips2(1, 8, 16, -opt_mctab[i].z.data.i);
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s1,offset(base)
					mips7(24, 17, base, 4*tab[ztpos].adr);
					// sub $t0,$s0,$s1
					mips1(4, 8, 16, 17);
				}
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sub $t0,$s0,$t0
					mips1(4, 8, 16, 8);
				}
			}
			else if(opt_mctab[i].y.type == 1){
				int ytpos = loc(opt_mctab[i].y.data.name, nowf);
				base = (tab[ytpos].findex == 0) ? 21 : 22;
				// lw $s0,offset(base)
				mips7(24, 16, base, 4*tab[ytpos].adr);
				if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
					// addi $t0,$s0,-immediate
					mips2(1, 8, 16, -opt_mctab[i].z.data.i);
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s1,offset(base)
					mips7(24, 17, base, 4*tab[ztpos].adr);
					// sub $t0,$s0,$s1
					mips1(4, 8, 16, 17);
				}
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[ztpos].adr);
					// sub $t0,$s0,$t0
					mips1(4, 8, 16, 8);
				}
			}
			else if(opt_mctab[i].y.type == 4){
				int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
				//  lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[ytpos].adr);
				if(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6){
					// addi $t0,$t0,-immediate
					mips2(1, 8, 8, -opt_mctab[i].z.data.i);
				}
				else if(opt_mctab[i].z.type == 1){
					int ztpos = loc(opt_mctab[i].z.data.name, nowf);
					base = (tab[ztpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[ztpos].adr);
					// sub $t0,$t0,$s0
					mips1(4, 8, 8, 16);
				}
				else if(opt_mctab[i].z.type == 4){
					int ztpos = loc(zaltemp[opt_mctab[i].z.data.i], nowf);
					// lw $t1,offset($s6)
					mips7(24, 9, 22, 4*tab[ztpos].adr);
					// sub $t0,$t0,$t1
					mips1(4, 8, 8, 9);
				}
			}
			// sw $t0,offset($s6)
			mips7(23, 8, 22, 4*tab[xtpos].adr);
		}
		else if(opt_mctab[i].fctnum == 22){
			if(in_condition){
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[xtpos].adr);
				if(condition_op == 15){  // <  大于等于则转移
					// bgez $t0,label
					mips5(14, 8, opt_mctab[i].y.data.i);
				}
				else if(condition_op == 16){  // <= 大于则转移
					// bgtz $t0,label
					mips5(15, 8, opt_mctab[i].y.data.i);
				}
				else if(condition_op == 17){  // > 小于等于则转移
					// blez $t0,label
					mips5(16, 8, opt_mctab[i].y.data.i);
				}
				else if(condition_op == 18){  // >= 小于则转移
					// bltz $t0,label
					mips5(17, 8, opt_mctab[i].y.data.i);
				}
				else if(condition_op == 19){  // != 等于则转移
					// beq $t0,$zero,label
					mips4(13, 8, 0, opt_mctab[i].y.data.i);
				}
				else if(condition_op == 20){  // == 不等于则转移
					// bne $t0,$zero,label
					mips4(18, 8, 0, opt_mctab[i].y.data.i);
				}
				in_condition = false;
				condition_op = -1;
			}
			else{
				if(opt_mctab[i].x.type == 0 || opt_mctab[i].x.type == 6){
					// addi $s0,$zero,immediate
					mips2(1, 16, 0, opt_mctab[i].x.data.i);
					// beg $s0,$zero,label
					mips4(13, 16, 0, opt_mctab[i].y.data.i);
				}
				else if(opt_mctab[i].x.type == 1){
					int xtpos = loc(opt_mctab[i].x.data.name, nowf);
					int base = (tab[xtpos].findex == 0) ? 21 : 22;
					// lw $s0,offset(base)
					mips7(24, 16, base, 4*tab[xtpos].adr);
					// beg $s0,$zero,label
					mips4(13, 16, 0, opt_mctab[i].y.data.i);
				}
				else if(opt_mctab[i].x.type == 4){
					// 等于0则转移
					int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
					// lw $t0,offset($s6)
					mips7(24, 8, 22, 4*tab[xtpos].adr);
					// beq $t0,$zero,label
					mips4(13, 8, 0, opt_mctab[i].y.data.i);
				}
			}
		}
		else if(opt_mctab[i].fctnum == 23){  // jmp
			// j target
			mips8(19, opt_mctab[i].x.data.i);
		}
		else if(opt_mctab[i].fctnum == 25){  // setlab
			// setlab target
			mips_setlab(27, opt_mctab[i].x.data.i);
			labelpos[opt_mctab[i].x.data.i] = textpoint;
		}
		else if(opt_mctab[i].fctnum == 26){  // return
			if(opt_mctab[i].x.type == 3){
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
				if(opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6){
					// addi $v1,$zero,0
					mips2(1, 3, 0, opt_mctab[i].y.data.i);
					// sw $v1,offset($s6)
					mips7(23, 3, 22, 0);
				}
				else if(opt_mctab[i].y.type == 1){
					int ytpos = loc(opt_mctab[i].y.data.name, nowf);
					int base = (tab[ytpos].findex == 0) ? 21 : 22;
					// lw $v1,offset(base)
					mips7(24, 3, base, 4*tab[ytpos].adr);
					// sw $v1,offset($s6)
					mips7(23, 3, 22, 0);
				}
				else if(opt_mctab[i].y.type == 4){
					int ytpos = loc(zaltemp[opt_mctab[i].y.data.i], nowf);
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
		else if(opt_mctab[i].fctnum == 27){  // call
			int xtpos = loc(opt_mctab[i].x.data.name, 0);
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
		else if(opt_mctab[i].fctnum == 28){  // read
			int xtpos = loc(opt_mctab[i].x.data.name, nowf);
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
		else if(opt_mctab[i].fctnum == 29){  // write
			if(opt_mctab[i].y.type == 7){
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[xtpos].adr);
				// addiu $v0,$zero,11
				mips2(3, 2, 0, 11);
				// add $a0,$zero,$t0
				mips1(0, 4, 0, 8);
			}
			else{
				if(opt_mctab[i].x.type == 6){
					// addiu $v0,$zero,11
					mips2(3, 2, 0, 11);
					// addi $a0,$zero,immediate
					mips2(1, 4, 0, opt_mctab[i].x.data.i);
				}
				else if(opt_mctab[i].x.type == 0){
					// addiu $v0,$zero,1
					mips2(3, 2, 0, 1);
					// addi $a0,$zero,immediate
					mips2(1, 4, 0, opt_mctab[i].x.data.i);
				}
				else if(opt_mctab[i].x.type == 1){
					int xtpos = loc(opt_mctab[i].x.data.name, nowf);
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
				else if(opt_mctab[i].x.type == 4){
					int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
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
		else if(opt_mctab[i].fctnum == 30){  // push
			int base;
			if(opt_mctab[i].x.type == 0 || opt_mctab[i].x.type == 6){
				// addi $s0,$zero,immediate
				mips2(1, 16, 0, opt_mctab[i].x.data.i);
			}
			else if(opt_mctab[i].x.type == 1){
				int xtpos = loc(opt_mctab[i].x.data.name, nowf);
				base = (tab[xtpos].findex == 0) ? 21 : 22;
				// lw $s0,offset(base)
				mips7(24, 16, base, 4*tab[xtpos].adr);
			}
			else if(opt_mctab[i].x.type == 4){
				int xtpos = loc(zaltemp[opt_mctab[i].x.data.i], nowf);
				// lw $t0,offset($s6)
				mips7(24, 8, 22, 4*tab[xtpos].adr);
				// add $s0,$zero,$t0
				mips1(0, 16, 0, 8);
			}
			int ztpos = loc(opt_mctab[i].z.data.name, tab[opt_mctab[i].y.data.i].ref);
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

void constdelete(int mode){
	int i;
	if(mode == 0){
		for(i = 0; i < mc; i++){
			if(mctab[i].fctnum >= 11 && mctab[i].fctnum <= 14){
				if((mctab[i].y.type == 0 && mctab[i].y.data.i == 0) || (mctab[i].z.type == 0 && mctab[i].z.data.i == 0)){
					mctab[i].fctnum = 8;
					mctab[i].y.type = 0;
					mctab[i].y.data.i = 0;
					mctab[i].z.type = 3;
				}
				else if((mctab[i].y.type == 0 || mctab[i].y.type == 6 || mctab[i].y.type == 7) && 
						(mctab[i].z.type == 0 || mctab[i].z.type == 6 || mctab[i].z.type == 7)){
					mctab[i].fctnum = 8;
					mctab[i].y.type = 0;
					if(mctab[i].fctnum == 11){
						mctab[i].y.data.i = mctab[i].y.data.i * mctab[i].z.data.i;
					}
					else if(mctab[i].fctnum == 12){
						mctab[i].y.data.i = mctab[i].y.data.i / mctab[i].z.data.i;
					}
					else if(mctab[i].fctnum == 13){
						mctab[i].y.data.i = mctab[i].y.data.i + mctab[i].z.data.i;
					}
					else if(mctab[i].fctnum == 14){
						mctab[i].y.data.i = mctab[i].y.data.i - mctab[i].z.data.i;
					}
					mctab[i].z.type = 3;
				}
			}
		}
	}
	else{
		for(i = 0; i < opt_mc; i++){
			if(opt_mctab[i].fctnum >= 11 && opt_mctab[i].fctnum <= 14){
				if((opt_mctab[i].y.type == 0 && opt_mctab[i].y.data.i == 0) || (opt_mctab[i].z.type == 0 && opt_mctab[i].z.data.i == 0)){
					opt_mctab[i].fctnum = 8;
					opt_mctab[i].y.type = 0;
					opt_mctab[i].y.data.i = 0;
					opt_mctab[i].z.type = 3;
				}
				else if((opt_mctab[i].y.type == 0 || opt_mctab[i].y.type == 6 || opt_mctab[i].y.type == 7) && 
						(opt_mctab[i].z.type == 0 || opt_mctab[i].z.type == 6 || opt_mctab[i].z.type == 7)){
					opt_mctab[i].fctnum = 8;
					opt_mctab[i].y.type = 0;
					if(opt_mctab[i].fctnum == 11){
						opt_mctab[i].y.data.i = opt_mctab[i].y.data.i * opt_mctab[i].z.data.i;
					}
					else if(opt_mctab[i].fctnum == 12){
						opt_mctab[i].y.data.i = opt_mctab[i].y.data.i / opt_mctab[i].z.data.i;
					}
					else if(opt_mctab[i].fctnum == 13){
						opt_mctab[i].y.data.i = opt_mctab[i].y.data.i + opt_mctab[i].z.data.i;
					}
					else if(opt_mctab[i].fctnum == 14){
						opt_mctab[i].y.data.i = opt_mctab[i].y.data.i - opt_mctab[i].z.data.i;
					}
					opt_mctab[i].z.type = 3;
				}
			}
		}
	}
}

void peephole(){
	int i;
	for(i = 0; i < mipsp; i++){
		if(i > 0 && mipstab[i].ordernum == 24){  // lw
			if(mipstab[i-1].ordernum == 23 && mipstab[i-1].base == mipstab[i].base && mipstab[i].offset == mipstab[i-1].offset){
				if(mipstab[i].rt == mipstab[i-1].rt){
					mipstab[i].ordernum = -1;
				}
				else{
					mipstab[i].ordernum = 0;
					mipstab[i].rd = mipstab[i].rt;
					mipstab[i].rs = mipstab[i-1].rt;
					mipstab[i].rt = 0;
				}
			}
		}
		else if(i < mipsp - 1 && mipstab[i].ordernum >= 14 && mipstab[i].ordernum <= 19 && mipstab[i+1].ordernum == 27){
			if(mipstab[i].ordernum == 19){
				if(mipstab[i].target == mipstab[i+1].target){
					mipstab[i].ordernum = -1;
				}
			}
			else{
				if(mipstab[i].offset == mipstab[i+1].target){
					mipstab[i].ordernum = -1;
				}
			}
		}
	}
}