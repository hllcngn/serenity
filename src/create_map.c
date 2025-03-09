#include "serenity.h"
Map* load_map(House* house){ //TODO move between indoors maps
Map* map =malloc(sizeof(Map)); //TODO go back to outdoors map
map->type =INDOORS;		//TODO exit to a different outdoors map
map->h =house->h; map->w =house->w;
map->name =strdup("House");
map->bg   =malloc_arrayint2(map->h,map->w);
for (int y=0; y<map->h; y++)
for (int x=0; x<map->w; x++)
	map->bg[y][x]=' ';
map->clsn =calloc_arrayint2(map->h,map->w);
map->fg   =calloc_arrayint2(map->h,map->w);
map->it   =calloc_arrayint2(map->h,map->w);
map->tp   =calloc_arrayint2(map->h,map->w);
map->inst =NULL;
paste_house(map,house,0,0);
return map;}

void create_map(Map* map, Ref* ref){
map->type =OUTDOORS;
map->bg   =malloc_arrayint2(map->h,map->w);
map->clsn =calloc_arrayint2(map->h,map->w);
map->fg   =calloc_arrayint2(map->h,map->w);
map->it   =calloc_arrayint2(map->h,map->w);
map->tp   =calloc_arrayint2(map->h,map->w);
map->inst =NULL;
int** blckd =calloc_arrayint2(map->h,map->w);

for (int y=0; y<map->h; y++)
	if (!(y%2))
		for (int x=0; x<map->w; x++)
			if (!(rand()%5))	map->bg[y][x]='v';
			else if (!(x%2))	map->bg[y][x]='=';
			else			map->bg[y][x]=' ';
	else	for (int x=0; x<map->w; x++)
			if (!(rand()%5))	map->bg[y][x]='v';
			else if (!(x%2))	map->bg[y][x]=' ';
			else			map->bg[y][x]='=';

map->bg[0][0]=' '; map->bg[0][map->w-1]=' ';
map->bg[map->h-1][0]=' '; map->bg[map->h-1][map->w-1]=' ';
for (int x=2; x<map->w-2; x+=2)
	if (!(rand()%2)) map->bg[0][x]=' ';
for (int x=map->h%2+1; x<map->w-2; x+=2)
	if (!(rand()%2)) map->bg[map->h-1][x]=' ';
for (int y=2; y<map->h-2; y+=2)
	if (!(rand()%2)) map->bg[y][0]=' ';
for (int y=map->w%2+1; y<map->h-2; y+=2)
	if (!(rand()%2)) map->bg[y][map->w-1]=' ';

House* ahouse =load_house("ass/house.txt");
int yhouse =map->h/2-15, xhouse =map->w/2-30;
paste_house(map,ahouse,yhouse,xhouse);
for (int y=0; y<ahouse->h; y++)
	for (int x=0; x<ahouse->w; x++)
		blckd[yhouse+y][xhouse+x] ='X';
free_house(ahouse);
House* hahouse =load_house("ass/houses/house1.txt");
map->house =hahouse;

Asset* atree1 =load_asset("ass/tree1.txt");
paste_asset(map,atree1,10,10);
free_asset(atree1);

Asset* umbrella =load_asset("ass/umbrella.txt");
paste_asset(map,umbrella,yhouse+20,xhouse+5);
free_asset(umbrella);

//add_inst(map,20,20,ref->interactive[0]);
//add_inst(map,20,30,ref->interactive[0]);

//very naive solution
//-> it doesn't consider foreground items
//-> I can make the house fully collisionable bg now
//-> check collisions on background items only
//-> blckd might be useful in the future
for (int i=0;i<map->w/3;i++){
	int yinst =rand()%(map->h-20)+10, xinst =rand()%(map->w-20)+10;
	if (!(blckd[yinst][xinst]) &&!(blckd[yinst+ref->interactive[1]->h][xinst])
			&&!(blckd[yinst+ref->interactive[1]->h][xinst+ref->interactive[1]->w])
			&&!(blckd[yinst][xinst+ref->interactive[1]->w]))
		add_inst(map,yinst,xinst,ref->interactive[1]);}
for (int i=0;i<map->w/6;i++){
	int yinst =rand()%(map->h-20)+10, xinst =rand()%(map->w-20)+10;
	if (!(blckd[yinst][xinst]))
		add_inst(map,yinst,xinst,ref->interactive[2]);}
}



void free_map(Map* map){
for (int y=0;y<map->h;y++) free(map->bg[y]);
for (int y=0;y<map->h;y++) free(map->clsn[y]);
for (int y=0;y<map->h;y++) free(map->fg[y]);
for (int y=0;y<map->h;y++) free(map->it[y]);
free(map->bg);free(map->clsn);free(map->fg);free(map->it);
free_instlist(map->inst);
free_house(map->house);
free(map->name);free(map);}
