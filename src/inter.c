#include "serenity.h"

Instance* create_tree(Ref* ref){
Instance* inst =malloc(sizeof(Instance));
inst->type =GENERATED; inst->actionlist =NULL;
inst->inter =ref->interactive[tree2];
inst->ascii =malloc(sizeof(Interactive));
Interactive* treemodel =ref->interactive[tree2];
int treeh=treemodel->h, treew=treemodel->w;
inst->ascii->ascii =duplicate_arraychar2(treemodel->ascii, treeh, treew);
for (int y=0; y<treeh; y++) for (int x=0; x<treew; x++)
	if (treemodel->ascii[y][x]!=' ' &&treemodel->ascii[y][x]!='|')
		inst->ascii->ascii[y][x] =treebase[rand()%TREEBASE_N];
inst->ascii->info =duplicate_arraychar2(treemodel->info, treeh, treew);
inst->ascii->inter =duplicate_arraychar2(treemodel->inter, treeh, treew);
return inst;}

Instance* create_fruittree(Ref* ref){
Instance* inst =malloc(sizeof(Instance));
inst->type =GENERATED; inst->actionlist =NULL;
inst->inter =ref->interactive[fruittree];
inst->ascii =malloc(sizeof(Interactive));
Interactive* treemodel =ref->interactive[tree2];
int treeh=treemodel->h, treew=treemodel->w;
inst->ascii->ascii =duplicate_arraychar2(treemodel->ascii, treeh, treew);
for (int y=0; y<treeh; y++) for (int x=0; x<treew; x++)
	if (treemodel->ascii[y][x]!=' ' &&treemodel->ascii[y][x]!='|'){
		int r =rand()%(TREEBASE_N+FRUITBASE_N);
		if (r <TREEBASE_N)
			inst->ascii->ascii[y][x] =treebase[r];
		else	inst->ascii->ascii[y][x] =fruitbase[r-TREEBASE_N];}
inst->ascii->info =duplicate_arraychar2(treemodel->info, treeh, treew);
inst->ascii->inter =duplicate_arraychar2(treemodel->inter, treeh, treew);
return inst;}
