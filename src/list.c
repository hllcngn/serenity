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

List* list_inst_get(List* list, int y, int x){
for (List* in=list; in; in=in->next)
	/*
	insty =((Inst*)(in->inst))->y;
	instx =((Inst*)(in->inst))->x;
	interh =((Inter*)(in->item))->h;
	interw =((Inter*)(in->item))->w;
	*/
	if (	y>=((Inst*)(in->inst))->y
		&&y<((Inst*)(in->inst))->y+((Inter*)(in->item))->h
		&&x>=((Inst*)(in->inst))->x
		&&x<((Inst*)(in->inst))->x+((Inter*)(in->item))->w)
			return in;
return NULL;}
/*
Inst* find_inst_inter(Ref* ref, Map* map, Inter* inter){
for (Inst* inst=map->inst; inst; inst=inst->next)
	if (inst->inter==inter) return inst;
return NULL;}
*/
