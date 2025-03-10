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
	new[y][x] =arr[y][x];
return new;}

int** spacoc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int y=0;y<h;y++){
       arr[y] =malloc(sizeof(int)*w);
       for (int x=0;x<w;x++)
	       arr[y][x]=' ';}
return arr;}

void fput_arrayint2(FILE* f,int** arr,int h,int w){
for (int y=0;y<h;y++){
	for (int x=0;x<w;x++)
		if (!arr[y][x])	putc(' ',f);
		else		putc((char)(arr[y][x]),f);
	putc('\n',f);}}

char** malloc_arraychar2(int h,int w){
char** arr =malloc(sizeof(char*)*h);
for (int y=0;y<h;y++) arr[y] =malloc(w*sizeof(char));
return arr;}

char** calloc_arraychar2(int h,int w){
char** arr =malloc(sizeof(char*)*h);
for (int y=0;y<h;y++) arr[y] =calloc(w, sizeof(char));
return arr;}

char** spacoc_arraychar2(int h,int w){
char** arr =malloc(sizeof(char*)*h);
for (int y=0;y<h;y++){
       arr[y] =malloc(sizeof(char)*w);
       for (int x=0;x<w;x++)
	       arr[y][x]=' ';}
return arr;}

void fput_arraychar2(FILE* f,char** arr,int h,int w){
for (int y=0;y<h;y++){
	for (int x=0;x<w;x++)
		if (!arr[y][x])	putc(' ',f);
		else		putc(arr[y][x],f);
	putc('\n',f);}}

void free_arrayint2(int** arr,int h,int w){
for (int y=0;y<h;y++) free(arr[y]);
free(arr);}

void free_arraychar2(char** arr,int h,int w){
for (int y=0;y<h;y++) free(arr[y]);
free(arr);}


int flen_line(FILE* f){
int len=0, c;
while (len<255 &&(c=getc(f))!='\n' &&c!=EOF)
	len++;
return len;}

char* fread_line(FILE* f){
char buf[256]; int len=0, c;
while (len<255 &&(c=getc(f))!='\n' &&c!=EOF){
	buf[len]=c; len++;}
char* line=malloc(len+1); line[len]='\0';
for (len--; len>=0; len--)
	line[len]=buf[len];
return line;}

void fsize_map(FILE* f, int* h, int* w){
int hh=0, ww=0, c;
while((c=getc(f))!='-' &&c!=EOF){ fseek(f,-1,SEEK_CUR);
	int x=flen_line(f);
	if(x>ww) ww=x; hh++;}
*h=hh; *w=ww;}

int** fread_map(FILE* f, int h, int w){
int** map =malloc(sizeof(int*)*h), c;
for (int y=0;y<h;y++){
	map[y] =malloc(sizeof(int)*w);
	for (int x=0;x<w;x++){
		c=getc(f);
		if (c=='\n'){
			for (x;x<w;x++) map[y][x]=' ';
			break;}
		else if (x==w-1) getc(f);
		map[y][x]=c;}}
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
