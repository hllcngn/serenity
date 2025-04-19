#include "serenity.h"

Inter** create_intertable(Action** actiontable){
Inter** inter =malloc(sizeof(Inter*)*nb_inter);
inter[tree2] =load_inter("ass/inter/tree2.txt", actiontable);
inter[fruittree] =load_inter("ass/inter/fruittree.txt", actiontable);
inter[stump] =load_inter("ass/inter/stump.txt", actiontable);
inter[umbrella] =load_inter("ass/inter/umbrella.txt", actiontable);
return inter;}

Inter* load_inter(char* path, Action** actiontable){
Inter* inter=malloc(sizeof(Inter));
FILE* f=fopen(path,"r");
fsize_map(f,&(inter->h),&(inter->w));
rewind(f);	     inter->ascii =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->info  =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->inter =fread_map(f,inter->h,inter->w);
inter->actionlist = NULL; fseek(f,2,SEEK_CUR);
char c; while ((c=getc(f))!='\n'&&c!=EOF){ fseek(f,-1,SEEK_CUR);
	char* act =fread_line(f);	//TODO read actions in reverse order
	for (int i=0;i<nb_action;i++)
	if (!strcmp(act,actiontable[i]->label))
		add_action(&(inter->actionlist),actiontable[i], ABLE);
	free(act);}
fclose(f);	return inter;}

void free_inter(Inter* inter){
for (int y=0;y<inter->h;y++){	free(inter->ascii[y]);
				free(inter->info[y]);
				free(inter->inter[y]);}
free(inter->ascii);free(inter->info);free(inter->inter);
free_actionlist(inter->actionlist);
free(inter);}

void free_intertable(Inter** inter){
for (int i=0;i<nb_inter;i++) free_inter(inter[i]);
free(inter);}



//return Inter
Inst* create_tree(Ref* ref){
Inst* inst =malloc(sizeof(Inst));
inst->type =GENERATED; inst->actionlist =NULL;
inst->inter =ref->inter[tree2];
inst->ascii =malloc(sizeof(Inter));
Inter* treemodel =ref->inter[tree2];
int treeh=treemodel->h, treew=treemodel->w;
inst->ascii->ascii =duplicate_arraychar2(treemodel->ascii, treeh, treew);
for (int y=0; y<treeh; y++) for (int x=0; x<treew; x++)
	if (treemodel->ascii[y][x]!=' ' &&treemodel->ascii[y][x]!='|')
		inst->ascii->ascii[y][x] =treebase[rand()%TREEBASE_N];
inst->ascii->info =duplicate_arraychar2(treemodel->info, treeh, treew);
inst->ascii->inter =duplicate_arraychar2(treemodel->inter, treeh, treew);
return inst;}

Inst* create_fruittree(Ref* ref){
Inst* inst =malloc(sizeof(Inst));
inst->type =GENERATED; inst->actionlist =NULL;
inst->inter =ref->inter[fruittree];
inst->ascii =malloc(sizeof(Inter));
Inter* treemodel =ref->inter[tree2];
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
