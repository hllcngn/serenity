#include "serenity.h"
World* create_world(void){
World*	world =malloc(sizeof(World));
return world;}



void create_map(Ref* ref, World* world, Map* map){
world->maplist =malloc(sizeof(Maplist));
world->maplist->map =map;
world->maplist->previous =world->maplist->next =NULL;
map->type =OUTDOORS;
map->bg   =malloc_arraychar2(map->h,map->w);
map->clsn =calloc_arraychar2(map->h,map->w);
map->fg   =calloc_arraychar2(map->h,map->w);
map->it   =calloc_arrayint2(map->h,map->w);
map->tp   =calloc_arraychar2(map->h,map->w);
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

House* ahouse =load_house("ass/houseassets/house.txt");
ahouse->id ='a';
int yhouse =map->h/2-15, xhouse =map->w/2-30;
ahouse->y =yhouse; ahouse->x =xhouse;
paste_house(map,ahouse,yhouse,xhouse);
for (int y=0; y<ahouse->h; y++)
	for (int x=0; x<ahouse->w; x++)
		blckd[yhouse+y][xhouse+x] ='X';
map->houselist =malloc(sizeof(Houselist));
map->houselist->house =ahouse;
map->houselist->previous =map->houselist->next =NULL;
//free_house(ahouse);
House* hahouse =load_house("ass/houses/house1.txt");
Map* maphouse =load_map(hahouse, map);
maphouse->name =strdup("House");
Maplist* ml =malloc(sizeof(Maplist));
ml->map =maphouse;
ml->previous =NULL; ml->next =world->maplist;
world->maplist->previous =ml;
world->maplist =ml;

/*Asset* atree1 =load_asset("ass/assets/tree1.txt");
paste_asset(map,atree1,10,10);
free_asset(atree1);*/

Asset* aumbrella =load_asset("ass/assets/umbrella.txt");
paste_asset(map,aumbrella,yhouse+20,xhouse+5);
free_asset(aumbrella);
Instance* uminst =add_inst_loaded(map,yhouse+24,xhouse+30,ref->interactive[umbrella]);

//add_inst(map,20,20,ref->interactive[0]);
//add_inst(map,20,30,ref->interactive[0]);

//very naive solution with blckd
//-> it doesn't consider foreground items
//-> I can make the house fully collisionable bg now
//-> check collisions on background items only
//-> blckd might be useful in the future
//=>check collision on instance spawning instead
for (int i=0;i<map->h*(map->w)/100;i++){
	int yinst =rand()%(map->h-20)+10, xinst =rand()%(map->w-20)+10;
	if (!(blckd[yinst][xinst]) &&!(blckd[yinst+ref->interactive[fruittree]->h][xinst])
			&&!(blckd[yinst+ref->interactive[fruittree]->h]
				[xinst+ref->interactive[fruittree]->w])
			&&!(blckd[yinst][xinst+ref->interactive[fruittree]->w]))
		add_inst_loaded(map,yinst,xinst,ref->interactive[fruittree]);}
for (int i=0;i<map->w/6;i++){
	int yinst =rand()%(map->h-20)+10, xinst =rand()%(map->w-20)+10;
	if (!(blckd[yinst][xinst]))
		add_inst_loaded(map,yinst,xinst,ref->interactive[stump]);}

for (int i=0;i<map->h*(map->w)/100;i++){
	int yinst =rand()%(map->h-20)+10, xinst =rand()%(map->w-20)+10;
	if (!(blckd[yinst][xinst]) &&!(blckd[yinst+ref->interactive[fruittree]->h][xinst])
			&&!(blckd[yinst+ref->interactive[fruittree]->h]
				[xinst+ref->interactive[fruittree]->w])
			&&!(blckd[yinst][xinst+ref->interactive[fruittree]->w]))
		add_inst_generated(map, yinst,xinst, create_tree(ref));}

}



Map* load_map(House* house,Map* oldmap){ //TODO move between indoors maps
Map* map =malloc(sizeof(Map)); //TODO go back to outdoors map
map->type =INDOORS;		//TODO exit to a different outdoors map
map->h =house->h; map->w =house->w;
map->name =strdup("House");
//map->oldmap =house->oldmap;
map->bg   =malloc_arraychar2(map->h,map->w);
for (int y=0; y<map->h; y++)
for (int x=0; x<map->w; x++)
	map->bg[y][x]=' ';
map->clsn =calloc_arraychar2(map->h,map->w);
map->fg   =calloc_arraychar2(map->h,map->w);
map->it   =calloc_arrayint2(map->h,map->w);
map->tp   =calloc_arraychar2(map->h,map->w);
map->inst =NULL;
paste_house(map,house,0,0);
return map;}


void save_map(Map* map){
mkdir("saves", 0744);
char path[17];
strcpy(path,"saves/");
strcat(path,map->name);
FILE* f =fopen(path, "w");
fputs(map->name,f); putc('\n',f);
fprintf(f,"%d %d\n",map->h,map->w);
for (int y=0; y<map->h; y++){
	fwrite(map->bg[y],sizeof(char),map->w,f);
	putc('\n',f);} putc('\n',f);
fput_arraychar2(f,map->clsn,map->h,map->w); putc('\n',f);
fput_arraychar2(f,map->fg,map->h,map->w); putc('\n',f);
for (int y=0; y<map->h; y++){
	fwrite(map->it[y],sizeof(int),map->w,f);
	putc('\n',f);} putc('\n',f);
fput_arraychar2(f,map->tp,map->h,map->w); putc('\n',f);
fclose(f);}


void free_map(Map* map){
free_arraychar2(map->bg,map->h,map->w);
free_arraychar2(map->clsn,map->h,map->w);
free_arraychar2(map->fg,map->h,map->w);
free_arrayint2(map->it,map->h,map->w);
free_arraychar2(map->tp,map->h,map->w);
free_instlist(map->inst);
//free_house(map->house);
free(map->name);
free(map);}
