#include "treemaking.h"
#include "serenity.h"

int main(int ac, char** av){
srand(time(NULL));
int fruit=0;
if (ac>1 &&!strcmp(av[1],"fruits")) fruit=1;

int** tree =calloc_arrayint2(TREEH,TREEW);
tree[0][0]=tree[0][5]=' ';
tree[2][0]=tree[2][5]=' ';
tree[3][0]=tree[3][1]=tree[3][4]=tree[3][5]=' ';
tree[3][2]=tree[3][3]='|';
for (int y=0;y<TREEH;y++)for (int x=0;x<TREEW;x++)
if (!tree[y][x]){
if (fruit &&rand()%2)
	tree[y][x]=fruits[rand()%FRUIT_N];
else
	tree[y][x]=treebase[rand()%TREEBASE_N];}


//displaying
for (int y=0;y<TREEH;y++){
	for (int x=0;x<TREEW;x++)
		putchar(tree[y][x]);
	putchar('\n');}

free_arrayint2(tree,TREEH,TREEW);	return 0;}
