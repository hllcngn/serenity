#include "serenity.h"

List* list_new(int type, void* item, void* inst){
List* new =malloc(sizeof(List));
new->type =type;
new->item =item;
new->inst =inst;
new->prev =new->next =NULL;}

void list_free_node(List* tf){
if (tf->type ==t_inst)
	if (((Inter*)(tf->item))->type ==GENERATED)
		free_inter(tf->item);
free(tf->inst);
free(tf);}

void list_free(List* list){
if (!list)	return;
list_free(list->next);
list_free_node(list);}



void list_insert_before(List** list, List* new){
if (!(*list)){	*list =new; new->prev =new->next =NULL;	return;}
new->prev =(*list)->prev;	new->next =*list;
(*list)->prev =new;
if (new->prev)	new->prev->next =new;
else		*list =new;}

void list_insert_after(List** list, List* new){
if (!(*list)){	*list =new; new->prev =new->next =NULL;	return;}
new->prev =*list;	new->next =(*list)->next;
(*list)->next =new;
if (new->next)	new->next->prev =new;}

void list_pop(List** list, List* trm){
if (!trm)	return;
if (*list ==trm)	*list =(*list)->next;
if (trm->prev)	trm->prev->next =trm->next;
if (trm->next)	trm->next->prev =trm->prev;}

void list_remove(List** list, List* trm){
list_pop(list, trm);
list_free_node(trm);}

List* list_duplicate(List* list){
List* new =NULL;
for (List* l=list; l; l=l->next){
	List* n =malloc(sizeof(List));
	memcpy(n, l, sizeof(List));
	list_insert_before(&new, n);}
return new;}

void list_remove_duplicates(List* list){
for (List* l=list; l; l=l->next)
	for (List *l2=l->next, *l3=l->next; l2;){
		l3=l2->next;
		if (l2->item==l->item) list_remove(&list, l2);
		l2=l3;}}



// SPECIFIC TO INSTANCES
List* list_inst_insert_new(List** list, Inter* inter, int y, int x){
Inst* inst =inst_new(0, y+2, x+rand()%2+1);
List* new =list_new(t_inst, inter, inst);
list_inst_insert(list, new);}

void list_inst_insert(List** list, List* inst){
List* in =*list;
if (!in){ *list =inst; inst->prev =inst->next =NULL;}
else {	List* i2=NULL; for (;in &&((Inst*)(in->inst))->y<((Inst*)(inst->inst))->y;
			in=in->next) i2=in;
	if(!i2)	list_insert_before(list, inst);
	else if(!in)	list_insert_after(&i2, inst);
	else	list_insert_before(&in, inst);}}

List* list_inst_find(List* list, int y, int x){
for (List* in=list; in; in=in->next){
	int insty =((Inst*)(in->inst))->y,
	instx =((Inst*)(in->inst))->x,
	interh =((Inter*)(in->item))->h,
	interw =((Inter*)(in->item))->w;
	if (	y>=insty
		&&y<insty+interh
		&&x>=instx
		&&x<instx+interw)
			return in;}
return NULL;}
/*
Inst* find_inst_inter(Ref* ref, Map* map, Inter* inter){
for (Inst* inst=map->inst; inst; inst=inst->next)
	if (inst->inter==inter) return inst;
return NULL;}
*/


// SPECIFIC TO ACTIONS
void list_act_insert_new(List** list, Action* action, int condition){
List* new =malloc(sizeof(List));
new->type =t_action;
new->inst =malloc(sizeof(Actinst));
((Actinst*)(new->inst))->condition =condition;
new->item =action;
list_insert_before(list, new);}

List* list_act_find_key(List* list, char key){
List* l; for (l=list; l &&((Action*)(l->item))->key!=key; l=l->next);
return l;}

List* list_act_find_label(List* list, char* label){
for (List* l=list; l; l=l->next)
	if (!strcmp(label,((Action*)(l->item))->label))
		return l;
return NULL;}

List* list_act_generate(Player* pl, List* inst){ //TODO break down this function
List* list =NULL;
for (List* l=pl->actlist; l; l=l->next)
	if (l &&((Actinst*)(l->inst))->condition==SUPERABLE)
		list_act_insert_new(&list, l->item, 0);
for (List *l=((Inst*)(inst->inst))->actlist; l; l=l->next){
	if (l &&((Actinst*)(l->inst))->condition==SUPERABLE)
		list_act_insert_new(&list, l->item, 0);
	else {	List* pll =list_act_find_label(pl->actlist,((Action*)(l->item))->label);
		if (pll &&((Actinst*)(pll->inst))->condition)
			list_act_insert_new(&list, pll->item, 0);}}
for (List *l=((Inter*)(inst->item))->actlist; l; l=l->next){
	if (l &&((Actinst*)(l->inst))->condition==SUPERABLE)
		list_act_insert_new(&list, l->item, 0);
	else {	List* pll =list_act_find_label(pl->actlist,((Action*)(l->item))->label);
		if (pll &&((Actinst*)(pll->inst))->condition)
			list_act_insert_new(&list, pll->item, 0);}}
list_remove_duplicates(list);
return list;}

