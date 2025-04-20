#include "serenity.h"

Action** create_actiontable(){
Action** action =malloc(sizeof(Action*)*nb_action);
for (int i=0;i<nb_action;i++)
	action[i] =malloc(sizeof(Action));
action[fall_tree]->id=fall_tree;
action[fall_tree]->label =strdup("Fall tree");
action[fall_tree]->c =0;
action[fall_tree]->key ='f';
action[fall_tree]->action =&act_fall_tree;
action[pull_stump]->id=pull_stump;
action[pull_stump]->label =strdup("Pull stump");
action[pull_stump]->c =0;
action[pull_stump]->key ='p';
action[pull_stump]->action =&act_pull_stump;
action[harvest_fruits]->id=harvest_fruits;
action[harvest_fruits]->label =strdup("Harvest fruits");
action[harvest_fruits]->c =0;
action[harvest_fruits]->key ='h';
action[harvest_fruits]->action =&act_harvest_fruits;
action[light_fire]->id=light_fire;
action[light_fire]->label =strdup("Light on fire");
action[light_fire]->c =0;
action[light_fire]->key ='l';
action[light_fire]->action =&act_light_fire;
for (int i=0;i<nb_action;i++)
	action[i]->labellen =strlen(action[i]->label);
return action;}

void free_actiontable(Action** action){
for (int i=0;i<nb_action;i++){
	free(action[i]->label);
	free(action[i]);}
free(action);}



void add_action(Actionlist** actionlist, Action* action, int condition){
Actionlist* al =malloc(sizeof(Actionlist));
al->action =action; al->condition =condition; al->previous =NULL;
if (*actionlist) (*actionlist)->previous =al;
al->next =*actionlist; *actionlist =al;}

void destroy_action(Actionlist* al){
if (!al)	return;
if (al->previous) al->previous->next =al->next;
if (al->next) al->next->previous =al->previous;
free(al);}

Actionlist* generate_complete_al(Player* pl, List* in){
Actionlist* aldisp =NULL;
for (Actionlist *al=pl->actionlist; al; al=al->next)
	if (al &&al->condition==SUPERABLE)
		add_action(&aldisp, al->action, 0);
for (Actionlist *al=((Inst*)(in->inst))->actionlist; al; al=al->next){
	if (al->condition==SUPERABLE)
		add_action(&aldisp, al->action, 0);
	else {	Actionlist* plal =find_action(al->action->label,pl->actionlist);
		if (plal &&plal->condition)
			add_action(&aldisp, plal->action, 0);}}
for (Actionlist *al=((Inter*)(in->item))->actionlist; al; al=al->next){
	if (al->condition==SUPERABLE)
		add_action(&aldisp, al->action, 0);
	else {	Actionlist* plal =find_action(al->action->label,pl->actionlist);
		if (plal &&plal->condition)
			add_action(&aldisp, plal->action, 0);}}
al_remove_duplicates(aldisp);
return aldisp;}

void al_remove_duplicates(Actionlist* list){
for (Actionlist* al=list; al; al=al->next)
	for (Actionlist *al2=al->next, *al3=al->next; al2;){
		al3=al2->next;
		if (al2->action==al->action) destroy_action(al2);
		al2=al3;}}

Actionlist* find_action(char* label, Actionlist* al){
for (Actionlist* all=al; all; all=all->next)
	if (!strcmp(label,all->action->label))
		return all;
return NULL;}

Actionlist* find_action_key(char key, Actionlist* al){
Actionlist* all =NULL;
for (all=al; all &&all->action->key!=key; all=all->next);
return all;}

void free_actionlist(Actionlist* al){
if (!al)	return;
free_actionlist(al->next);
free(al);}
/*
void free_actionlist(Actionlist* al){
for (Actionlist *al2=al; al2;){
	al2 =al->next;
	free(al);
	al =al2;}}
*/



void act(Ref* ref, Map* map, Player* pl, char c){
List* inst =list_inst_get(map->inst, pl->y, pl->x);
if (!inst)	return;
Actionlist* al =generate_complete_al(pl, inst);
al =find_action_key(c, al);
if (al) al->action->action(inst, map, ref);}


void act_fall_tree(List* inst, Map* map, Ref* ref){
int y =((Inst*)(inst->inst))->y, x =((Inst*)(inst->inst))->x;
list_remove(&(map->inst),inst);
list_inst_insert_new(&(map->inst), ref->inter[stump], y+2, x+rand()%2+1);}

void act_pull_stump(List* inst, Map* map, Ref* ref){
list_remove(&(map->inst),inst);}

void act_harvest_fruits(List* inst, Map* map, Ref* ref){
}

void act_light_fire(List* inst, Map* map, Ref* ref){
list_remove(&(map->inst),inst);}
