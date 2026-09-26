/*Cristian Alexandru Catalin 312CC*/
#include "library.h"
#include <stdlib.h>

// initializeaza coada cu unitati
qlist_u *init_unit_list()
{
    qlist_u *queue = malloc(sizeof(qlist_u));

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

// adauga elemente la coada cu unitati
void push_units_queue(struct unit* address, qlist_u *queue)
{
    node_unit *new_node = malloc(sizeof(node_unit));
    new_node->address = address;
    new_node->next = NULL;

    // verifica daca coada este goala
    if(queue->tail == NULL)
    {
        queue->head = new_node;
    }
    else
    {
        queue->tail->next = new_node;
    }

    queue->tail = new_node;

}

// elimina primul nod din coada de unitati
void pop_units_queue(qlist_u *queue)
{
   node_unit *delete = queue->head;
   queue->head = queue->head->next;

    if(queue->head == NULL){
        queue->tail = NULL;
   }
   
   free(delete);
}

// initializeaza coada cu incidente
qlist_i *init_incident_list()
{
    qlist_i *queue = malloc(sizeof(qlist_i));

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

// adauga elemente la coada cu incidente (la final)
void push_incidents_queue(struct incident* address, qlist_i *queue)
{
    node_incident *new_node = malloc(sizeof(node_incident));
    new_node->address = address;
    new_node->next = NULL;

    if(queue->tail == NULL)
    {
        queue->head = new_node;
    }
    else
    {
        queue->tail->next = new_node;
    }
    queue->tail = new_node;

}

// adauga elemente la coada cu incidente (la inceput)
// se foloseste la readaugarea unui incident in coada
void push_incidents_queue_first(struct incident* address, qlist_i *queue)
{
    node_incident *new_node = malloc(sizeof(node_incident));
    new_node->address = address;
    new_node->next = queue->head;
    queue->head = new_node;
}

// elimina primul nod din coada de incidente
void pop_incidents_queue(qlist_i *queue)
{
   node_incident *delete = queue->head;
   queue->head = queue->head->next;

   if(queue->head == NULL){
        queue->tail = NULL;
   }

   free(delete);
}

// initializeaza stiva
 stack * init_stack()
{
    stack *st = malloc(sizeof(stack));
    st->head = NULL;
    return st;
}

// adauga elemente la stiva (in varful ei)
void push_intervention_stack(struct intervention *address, stack *st)
{
    node_stack *new_node = malloc(sizeof(node_stack));
    new_node->address = address;
    new_node->next = st->head;
    st->head = new_node;
}

// elimina nodul din varful stivei
void pop_intervention_stack(stack *st)
{
    if(st->head == NULL)
    {
        return;
    }
    node_stack *delete = st->head;
    st->head = st->head->next;
    free(delete);
}
