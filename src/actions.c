#include "serenity.h"
//see list.c for action list operations

Action** create_actiontable(){
Action **action= malloc(sizeof(Action*)*nb_action);
for (int i=0; i<nb_action; i++)
	action[i] =malloc(sizeof(Action));
char *str;                      //  key, c, labellen, label, action func
str = strdup("Fall tree");
*(action[fall_tree]) =      (Action){'f', 0, strlen(str), str, &act_fall_tree};
str = strdup("Pull stump");
*(action[pull_stump]) =     (Action){'p', 0, strlen(str), str, &act_pull_stump};
str = strdup("Harvest fruits");
*(action[harvest_fruits]) = (Action){'h', 0, strlen(str), str, &act_harvest_fruits};
str = strdup("Light on fire");
*(action[light_fire]) =     (Action){'l', 0, strlen(str), str, &act_light_fire};
return action;}

void free_actiontable(Action** action){
for (int i=0; i<nb_action; i++){
	free(action[i]->label);
	free(action[i]);}
free(action);}



void act(Ref *ref, Map *map, Player *pl, char c){
List *instlist =list_inst_generate(map, pl->y, pl->x);
List *inst =NULL;
for (List *l=instlist; l; l=l->next){
	List *inst2 =(List*)(l->item);
	if (((Inter*)(inst2->item))->inter[pl->y-((Inst*)(inst2->inst))->y]
					  [pl->x-((Inst*)(inst2->inst))->x]=='i'){
		inst =inst2; break;}}
if (instlist)	free(instlist);
if (!inst)	return;
List *l= list_act_generate(pl, inst);
List *l2= list_act_find_key(l, c);
if (l2) ((Action*)(l2->item))->action(ref, map, inst);
list_free(l);}


void act_fall_tree(Ref *ref, Map *map, List *inst){
int y= ((Inst*)(inst->inst))->y, x= ((Inst*)(inst->inst))->x;
list_remove(&(map->inst), inst);
list_inst_insert_new(&(map->inst), ref->inter[stump], y+2, x+1+rand()%2);}

void act_pull_stump(Ref *ref, Map *map, List *inst){
list_remove(&(map->inst), inst);}

void act_harvest_fruits(Ref *ref, Map *map, List *inst){
}

void act_light_fire(Ref *ref, Map *map, List *inst){
list_remove(&(map->inst), inst);}
