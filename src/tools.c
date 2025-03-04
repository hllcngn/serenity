#include "serenity.h"

int** malloc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int i=0;i<h;i++) arr[i] =malloc(sizeof(int)*w);
	return arr;}

int** calloc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int i=0;i<h;i++) arr[i] =calloc(w, sizeof(int));
	return arr;}

int** duplicate_arrayint2(int** arr,int h,int w){
int** new =malloc_arrayint2(h,w);
for (int y=0;y<h;y++) for (int x=0;x<w;x++)
	new[y][x] =arr[y][x];	return new;}

void free_arrayint2(int** arr,int h,int w){
for (int y=0;y<h;y++) free(arr[y]);
free(arr);	return;}


int flen_line(FILE* f){
char c; int len=0;
while (len<255 &&(c=fgetc(f))!='\n' &&c!=EOF)
	len++;
return len;}

char* fread_line(FILE* f){
char buf[256], c; int len=0;
while (len<255 &&(c=fgetc(f))!='\n' &&c!=EOF){
	buf[len]=c; len++;}
char* line=malloc(len+1); line[len]='\0';
for (len--; len>=0; len--)
	line[len]=buf[len];
return line;}

void fsize_map(FILE* f, int* h, int* w){
int hh=0, ww=0;
char c; while((c=fgetc(f))!='-'&&c!=EOF){
	fseek(f,-1,SEEK_CUR); int x=flen_line(f);
	if(x>ww) ww=x; hh++;}
*h=hh; *w=ww;}

int** fread_map(FILE* f, int h, int w){
int** map =malloc(sizeof(int*)*h);
for (int y=0;y<h;y++)
	map[y] =malloc(sizeof(int)*w);
for (int y=0;y<h;y++){ for (int x=0;x<w;x++){
	map[y][x]=fgetc(f);
	if (map[y][x]=='\n'){
		for (x;x<w;x++) map[y][x]=' '; break;}
	else if (x==w-1) fgetc(f);}}
return map;}


void clear_screen(int cp){
move(0,0); attron(COLOR_PAIR(cp));
for (int y=0;y<LINES;y++)
	for (int x=0;x<COLS;x++)
		addch(' ');}

void debug_msg(const char* str){
mvprintw(0,0,str); addch('\n'); getch();}

char* path_cat(const char* path, const char* file){
int l1=strlen(path), l2=strlen(file);
char* cat=malloc(l1+l2+1);
strncpy(cat,path,l1+1);
strncat(cat,file,l2);
return cat;}
