#include "serenity.h"

void add_inst_to_map_from_inter(Map* map, char** blckd, Inter* inter);
void grassland(Map* map);


World* create_world(void){
World*	world =malloc(sizeof(World));
world->maplist =NULL;
return world;}

void free_world(World* world){
list_free(world->maplist);
free(world);}


Map* new_map(int type, int h, int w){
Map* map =malloc(sizeof(Map));
map->type =type;
map->h =h; map->w =w;
map->bg   =malloc_arraychar2(map->h,map->w);
map->inst =NULL;
map->name =NULL;
map->tp =NULL;
return map;}

Map* create_map1(Ref* ref, int h, int w){
Map* map =new_map(OUTDOORS, h, w);
char** blckd =calloc_arraychar2(map->h,map->w);

grassland(map);

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
for (int y=0; y<ahouse->h+10; y++)
	for (int x=0; x<ahouse->w; x++)
		blckd[yhouse+y][xhouse+x] ='X';

map->houselist =malloc(sizeof(Houselist)); //TODO refactor houses into inters and maps
map->houselist->house =ahouse;
map->houselist->previous =map->houselist->next =NULL;
//free_house(ahouse);
House* hahouse =load_house("ass/houses/house1.txt");
Map* maphouse =load_map(hahouse, map);
maphouse->name =strdup("House");

Asset* aumbrella =load_asset("ass/assets/umbrella.txt");
paste_asset(map,aumbrella,yhouse+20,xhouse+5);
free_asset(aumbrella);

List* uminst =list_inst_insert_new(&(map->inst), ref->inter[umbrella], yhouse+24, xhouse+30);

for (int i=0;i<map->w;i++)
	add_inst_to_map_from_inter(map, blckd, ref->inter[stump]);

for (int i=0;i<map->h*(map->w)/200;i++)
	add_inst_to_map_from_inter(map, blckd, create_tree(ref));

for (int i=0;i<map->h*(map->w)/200;i++)
	add_inst_to_map_from_inter(map, blckd, create_fruittree(ref));


List* pinst =list_inst_insert_new(&(map->inst), ref->inter[portal], yhouse+27, xhouse+55);
Tp* tp =malloc(sizeof(Tp));
tp->srcit ='a'; tp->dstit =0;
tp->srcmap =map; tp->dstmap =NULL;
tp->dsty =0; tp->dstx =0;
tp->srcinst =NULL; tp->dstinst =NULL;
((Inst*)(pinst->inst))->tp =tp;
return map;}


Map* create_further_map(Map* srcmap){
v2i size ={(rand()%16+10)*10,(rand()%16+10)*20};
Map* map =new_map(OUTDOORS, size.y, size.x);
map->name =strdup("map 2");
grassland(map);
return map;}


void free_map(Map* map){
free_arraychar2(map->bg,map->h,map->w);
list_free(map->inst);
//free_house(map->house);
free(map->name);
free(map);}


void add_inst_to_map_from_inter(Map* map, char** blckd, Inter* inter){
int yinst, xinst; int blocked =1; while (blocked){ blocked =0;
yinst =rand()%(map->h-20)+10; xinst =rand()%(map->w-20)+10;
for (int y=0; y<inter->h &&!blocked; y++) for (int x=0; x<inter->w &&!blocked; x++)
	if(inter->info[y][x]=='X' &&blckd[yinst+y][xinst+x])	blocked =1;}
list_inst_insert_new(&(map->inst), inter, yinst, xinst);
for (int y=0; y<inter->h; y++) for (int x=0; x<inter->w; x++)
	if (inter->info[y][x]=='X')
		blckd[yinst+y][xinst+x] ='X';}

void grassland(Map* map){
for (int y=0; y<map->h; y++)
	if (!(y%2))
		for (int x=0; x<map->w; x++)
			if (!(rand()%5))	map->bg[y][x]='v';
			else if (!(x%2))	map->bg[y][x]='=';
			else			map->bg[y][x]=' ';
	else	for (int x=0; x<map->w; x++)
			if (!(rand()%5))	map->bg[y][x]='v';
			else if (!(x%2))	map->bg[y][x]=' ';
			else			map->bg[y][x]='=';}



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
fput_arraychar2(f,map->bg,map->h,map->w); putc('\n',f);
fclose(f);}
