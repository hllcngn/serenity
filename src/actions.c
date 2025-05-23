#include "serenity.h"
//see list.c for action list operations

Action** create_actiontable(){
Action **action= malloc(sizeof(Action*)*nb_action);
for (int i=0; i<nb_action; i++)
	action[i] =malloc(sizeof(Action));
action[fall_tree]->label= strdup("Fall tree");
action[fall_tree]->key= 'f';
action[fall_tree]->c= 0;
action[fall_tree]->action= &act_fall_tree;
action[pull_stump]->label= strdup("Pull stump");
action[pull_stump]->key= 'p';
action[pull_stump]->c= 0;
action[pull_stump]->action= &act_pull_stump;
action[harvest_fruits]->label= strdup("Harvest fruits");
action[harvest_fruits]->key= 'h';
action[harvest_fruits]->c= 0;
action[harvest_fruits]->action= &act_harvest_fruits;
action[light_fire]->label= strdup("Light on fire");
action[light_fire]->key= 'l';
action[light_fire]->c= 0;
action[light_fire]->action= &act_light_fire;
for (int i=0; i<nb_action; i++)
	action[i]->labellen =strlen(action[i]->label);
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
