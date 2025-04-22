#include "serenity.h"

Inter** create_intertable(Action** actiontable){
Inter** inter =malloc(sizeof(Inter*)*nb_inter);
inter[tree2] =load_inter("ass/inter/tree2.txt", actiontable);
inter[fruittree] =load_inter("ass/inter/fruittree.txt", actiontable);
inter[stump] =load_inter("ass/inter/stump.txt", actiontable);
inter[umbrella] =load_inter("ass/inter/umbrella.txt", actiontable);
return inter;}

void free_intertable(Inter** inter){
for (int i=0;i<nb_inter;i++) free_inter(inter[i]);
free(inter);}


Inter* load_inter(char* path, Action** actiontable){
Inter* inter=malloc(sizeof(Inter));
FILE* f=fopen(path,"r");
fsize_map(f,&(inter->h),&(inter->w));
rewind(f);	     inter->ascii =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->info  =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->inter =fread_map(f,inter->h,inter->w);
inter->actlist = NULL; fseek(f,2,SEEK_CUR);
char c; while ((c=getc(f))!='\n'&&c!=EOF){ fseek(f,-1,SEEK_CUR);
	char* act =fread_line(f);
	for (int i=0;i<nb_action;i++)
	if (!strcmp(act,actiontable[i]->label))
		list_act_insert_new(&(inter->actlist), actiontable[i], ABLE);
	free(act);}
fclose(f);	return inter;}

void free_inter(Inter* inter){
for (int y=0;y<inter->h;y++){	free(inter->ascii[y]);
				free(inter->info[y]);
				free(inter->inter[y]);}
free(inter->ascii);free(inter->info);free(inter->inter);
list_free(inter->actlist);
free(inter);}


Inter* duplicate_inter(Inter* inter){
Inter* new =malloc(sizeof(Inter));
new->type =inter->type;
new->h =inter->h; new->w =inter->w;
new->ascii =duplicate_arraychar2(inter->ascii, inter->h, inter->w);
new->info =duplicate_arraychar2(inter->info, inter->h, inter->w);//can i leave these pointers pointing
new->inter =duplicate_arraychar2(inter->inter, inter->h, inter->w);//to the same arrays?
new->actlist =list_duplicate(inter->actlist);			  //it would be difficult to free them
return new;}



Inter* create_tree(Ref* ref){
Inter* inter =duplicate_inter(ref->inter[tree2]);
inter->type =GENERATED;
for (int y=0; y<inter->h; y++) for (int x=0; x<inter->w; x++)
	if (inter->ascii[y][x]!=' ' &&inter->ascii[y][x]!='|')
		inter->ascii[y][x] =treebase[rand()%TREEBASE_N];
return inter;}

Inter* create_fruittree(Ref* ref){
Inter* inter =duplicate_inter(ref->inter[tree2]);
inter->type =GENERATED;
for (int y=0; y<inter->h; y++) for (int x=0; x<inter->w; x++)
	if (inter->ascii[y][x]!=' ' &&inter->ascii[y][x]!='|'){
		int r =rand()%(TREEBASE_N+FRUITBASE_N);
		if (r <TREEBASE_N)
			inter->ascii[y][x] =treebase[r];
		else	inter->ascii[y][x] =fruitbase[r-TREEBASE_N];}
return inter;}
