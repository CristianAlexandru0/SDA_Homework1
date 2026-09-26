/*Cristian Alexandru Catalin 312CC*/
#include <stdio.h>

struct unit
{
    int id;
    char type;
    int availability;
};

struct incident
{
    int id;
    char priority[7];
    char *description;
    char status[11];
    struct incident * prev;
    struct incident *next;
};

struct intervention
{
    struct incident *incident;
    struct unit *unit;
    struct intervention *prev;
    struct intervention *next;
};

// nod pentru coada de unitati
typedef struct node_u
{
    // pointer la adresa unei unitati
    struct unit *address; 
    struct node_u *next;
}node_unit;

// coada de unitati
typedef struct 
{
    node_unit *head;
    node_unit *tail;
}qlist_u;

// nod pentru coada de incidente
typedef struct node_i
{
    // pointer la adresa unui incident
    struct incident *address; 
    struct node_i *next;
}node_incident;

// coada de incidente
typedef struct 
{
    node_incident *head;
    node_incident *tail;
}qlist_i;

// nod pentru stiva
typedef struct node_s
{
    struct intervention *address;
    struct node_s *next;
}node_stack;

// stiva
typedef struct
{
    node_stack *head;
}stack;

// sistemul de urgente
struct system
{
    int nr_unitati;
    struct unit *units;
    struct incident *incidents;
    struct intervention *interventions;

    qlist_u *queue_available_units;

    qlist_i *queue_low;
    qlist_i *queue_medium;
    qlist_i *queue_high;
    
    

    stack  *history_stack;
};

// functii pentru stiva
stack * init_stack();
void push_intervention_stack(struct intervention *address, stack *st);
void pop_intervention_stack(stack *st);

// functii pentru cozi de incidente
void push_incidents_queue(struct incident* address, qlist_i *queue);
void push_incidents_queue_first(struct incident* address, qlist_i *queue);
void pop_incidents_queue(qlist_i *queue);

// functii pentru cozi de unitati
void push_units_queue(struct unit* address, qlist_u *queue);
void pop_units_queue(qlist_u *queue);

// verificare disponibilitate
int check_units_availability(qlist_u *queue_units, FILE *out , int option);
int check_incident_availability(qlist_i *queue_incidents);


// initializare
qlist_u *init_unit_list();
qlist_i *init_incident_list();
struct system *init_sistem(FILE *f);

// adaugare incidente
void push_incident_list(struct incident *new_incident,  struct incident *incidents);
void add_incident(struct system *sistem, FILE *f);
