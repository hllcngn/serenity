#include "serenity.h"

Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
ass->h=0; ass->w=0;
char c; while ((c=fgetc(f))!=EOF &&c!='-'){ fseek(f,-1,SEEK_CUR);
	int x=0; while ((c=fgetc(f))!='\n' &&c!=EOF) x++;
	if (x>ass->w) ass->w=x;
	ass->h++;}

rewind(f);
ass->map =malloc(sizeof(int*)*ass->h);
for (int y=0;y<ass->h;y++)
	ass->map[y] =malloc(sizeof(int)*ass->w);
for (int y=0;y<ass->h;y++){
	for (int x=0;x<ass->w;x++){
		ass->map[y][x]=fgetc(f);
		if (ass->map[y][x]=='\n'){
			for (;x<ass->w;x++)
				ass->map[y][x]=' ';
			break;}
		else if (x==ass->w-1) fgetc(f);}}

fseek(f,2,SEEK_CUR);
ass->info =malloc(sizeof(int*)*ass->h);
for (int y=0;y<ass->h;y++)
	ass->info[y] =malloc(sizeof(int)*ass->w);
for (int y=0;y<ass->h;y++){
	for (int x=0;x<ass->w;x++){
		ass->info[y][x]=fgetc(f);
		if (ass->info[y][x]=='\n'){
			for (;x<ass->w;x++)
				ass->info[y][x]=' ';
			break;}
		else if (x==ass->w-1) fgetc(f);}}
fclose(f);	return ass;}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++)
	free(ass->map[y]);
free(ass->map); free(ass); return;}


Interactive* load_inter(char* path){
Interactive* inter =malloc(sizeof(Interactive));
FILE* f =fopen(path,"r"); while (fgetc(f)!='\n');
inter->h=0; inter->w=0;
char c; while ((c=fgetc(f))!=EOF &&c!='-'){ fseek(f,-1,SEEK_CUR);
	int x=0; while ((c=fgetc(f))!='\n' &&c!=EOF) x++;
	if (x>inter->w) inter->w=x;
	inter->h++;}

rewind(f); while (fgetc(f)!='\n');
inter->map =malloc(sizeof(int*)*inter->h);
for (int y=0;y<inter->h;y++)
	inter->map[y] =malloc(sizeof(int)*inter->w);
for (int y=0;y<inter->h;y++){
	for (int x=0;x<inter->w;x++){
		inter->map[y][x]=fgetc(f);
		if (inter->map[y][x]=='\n'){
			for (;x<inter->w;x++)
				inter->map[y][x]=' ';
			break;}
		else if (x==inter->w-1) fgetc(f);}}

fseek(f,2,SEEK_CUR);
inter->info =malloc(sizeof(int*)*inter->h);
for (int y=0;y<inter->h;y++)
	inter->info[y] =malloc(sizeof(int)*inter->w);
for (int y=0;y<inter->h;y++){
	for (int x=0;x<inter->w;x++){
		inter->info[y][x]=fgetc(f);
		if (inter->info[y][x]=='\n'){
			for (;x<inter->w;x++)
				inter->info[y][x]=' ';
			break;}
		else if (x==inter->w-1) fgetc(f);}}

fseek(f,2,SEEK_CUR);
inter->inter =malloc(sizeof(int*)*inter->h);
for (int y=0;y<inter->h;y++)
	inter->inter[y] =malloc(sizeof(int)*inter->w);
for (int y=0;y<inter->h;y++){
	for (int x=0;x<inter->w;x++){
		inter->inter[y][x]=fgetc(f);
		if (inter->inter[y][x]=='\n'){
			for (;x<inter->w;x++)
				inter->inter[y][x]=' ';
			break;}
		else if (x==inter->w-1) fgetc(f);}}
	
int len=0; while ((c=fgetc(f))!='\n' &&c!=EOF) len++;
inter->label =malloc(len);
fseek(f,-(len),SEEK_CUR);
for (int i=0;i<len;i++) inter->label[i] =fgetc(f);
fclose(f);	return inter;}


void paste_asset(Map* map, int y, int x, Asset* ass){
for (int yy=0;yy<ass->h;yy++)	//TODO edge cases
	for (int xx=0;xx<ass->w;xx++)
		switch (ass->info[yy][xx]){
		case ' ':	break;
		case 'b': map->bg[y+yy][x+xx] =ass->map[yy][xx];   break;
		case 'X': map->clsn[y+yy][x+xx] =ass->map[yy][xx]; break;
		case 'f': map->fg[y+yy][x+xx] =ass->map[yy][xx];   break;
		default:	break;}	return;}

void add_inter(Map* map, int y, int x, Interactive* inter){
inter->y =y; inter->x =x;
if (!map->inter)	inter->id =0;
else			inter->id =map->inter->id+1;
inter->next =map->inter;
map->inter =inter;
for (int yy=0;yy<inter->h;yy++)	//TODO edge cases
	for (int xx=0;xx<inter->w;xx++)
		if (inter->inter[yy][xx]=='i')
			map->it[y+yy][x+xx]=inter->id;
return;}
