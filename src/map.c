#include "serenity.h"

void	add_inst_to_map_from_inter(Map* map, char** blckd, Inter* inter);
void	add_gen_tree_to_map_from_inter(Ref* ref, Map* map, char** blckd, Inter* inter, Inst* (*f)(Ref*));


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
map->tp   =calloc_arraychar2(map->h,map->w);
map->inst =NULL;
char** blckd =calloc_arraychar2(map->h,map->w);

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

for (int y=0; y<ahouse->h+10; y++)
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

//TODO add_inst_from_inter  list_inst_add_inter
Inst* uminst =create_inst_from_inter(ref->inter[umbrella]);
uminst->y =yhouse+24; uminst->x =xhouse+30;
List* uminstl =list_new(t_inst, ref->inter[umbrella], uminst);
list_inst_insert(&(map->inst), uminstl);

//for (int i=0;i<map->h*(map->w)/500;i++)
//	add_inst_to_map_from_inter(map, blckd, ref->inter[fruittree]);

for (int i=0;i<map->w*7;i++)
	add_inst_to_map_from_inter(map, blckd, ref->inter[stump]);

for (int i=0;i<map->h*(map->w)/200;i++)
	add_gen_tree_to_map_from_inter(ref, map, blckd, ref->inter[tree2], &create_tree);

for (int i=0;i<map->h*(map->w)/200;i++)
	add_gen_tree_to_map_from_inter(ref, map, blckd, ref->inter[tree2], &create_fruittree);
}

void	add_inst_to_map_from_inter(Map* map, char** blckd, Inter* inter){
	int yinst, xinst;
	int blocked =1; while (blocked){ blocked =0;
	yinst =rand()%(map->h-20)+10; xinst =rand()%(map->w-20)+10;
	for (int y=0; y<inter->h &&!blocked; y++) for (int x=0; x<inter->w &&!blocked; x++)
		if(inter->info[y][x]=='X' &&blckd[yinst+y][xinst+x])	blocked =1;}
	Inst* inst =create_inst_from_inter(inter);
	inst->y =yinst; inst->x =xinst;
	List* new =list_new(t_inst, inter, inst);
	list_inst_insert(&(map->inst), new);
	for (int y=0; y<inst->inter->h; y++) for (int x=0; x<inst->inter->w; x++)
		if (inst->inter->info[y][x]=='X')
			blckd[inst->y+y][inst->x+x] ='X';}

void	add_gen_tree_to_map_from_inter(Ref* ref, Map* map, char** blckd, Inter* inter, Inst* (*f)(Ref*)){
	int yinst, xinst;
	int blocked =1; while (blocked){ blocked =0;
	yinst =rand()%(map->h-20)+10; xinst =rand()%(map->w-20)+10;
	for (int y=0; y<inter->h &&!blocked; y++) for (int x=0; x<inter->w &&!blocked; x++)
		if(inter->info[y][x]=='X' &&blckd[yinst+y][xinst+x])	blocked =1;}
	Inst* inst =(*f)(ref);
	inst->y =yinst; inst->x =xinst;
	List* new =list_new(t_inst, inter, inst); //TODO handle inter properly
	list_inst_insert(&(map->inst), new);
	for (int y=0; y<inst->inter->h; y++) for (int x=0; x<inst->inter->w; x++)
		if (inst->inter->info[y][x]=='X')
			blckd[inst->y+y][inst->x+x] ='X';}


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
//map->it   =calloc_arrayint2(map->h,map->w);
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
fput_arraychar2(f,map->tp,map->h,map->w); putc('\n',f);
fclose(f);}


void free_map(Map* map){
free_arraychar2(map->bg,map->h,map->w);
free_arraychar2(map->clsn,map->h,map->w);
free_arraychar2(map->fg,map->h,map->w);
free_arraychar2(map->tp,map->h,map->w);
//free_instlist(map->inst);
//free_house(map->house);
free(map->name);
free(map);}
