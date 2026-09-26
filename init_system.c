/*Cristian Alexandru Catalin 312CC*/
#include "library.h"
#include <string.h>
#include <stdlib.h>

// initializeaza sistemul cu toate cozile,listele si stiva  
struct system *init_sistem(FILE *f)
{
    struct system *sistem = malloc(sizeof(struct system));
    int i = 0;   

    fscanf(f,"%d",&sistem->nr_unitati);
    // aloca vectorul de unitati
    sistem->units = malloc(sistem->nr_unitati * sizeof(struct unit));

    // initializeaza cele 4 cozi;
    sistem->queue_available_units = init_unit_list();
    sistem->queue_low = init_incident_list();
    sistem->queue_medium = init_incident_list();
    sistem->queue_high = init_incident_list();
    
    sistem->history_stack = init_stack();
    //citeste unitatile, le adauga in coada de disponibilitate si in vectorul de unitati
    for(i = 0; i < sistem->nr_unitati; i++){
        fscanf(f,"%d", &sistem->units[i].id);
        fscanf(f," %c", &sistem->units[i].type);
        sistem->units[i].availability = 1;

        push_units_queue(&sistem->units[i], sistem->queue_available_units);
    }

    // initializeaza listele duble circulare pentru interventii si incidente
    struct intervention *dummy1 = malloc(sizeof(struct intervention));
    dummy1->prev= dummy1;
    dummy1->next = dummy1;
    dummy1->incident = NULL;
    dummy1->unit = NULL;

    sistem->interventions = dummy1;

    struct incident *dummy2 = malloc(sizeof(struct incident));
    dummy2->prev = dummy2;
    dummy2->next = dummy2;
    dummy2->id = 0;

    strcpy(dummy2->priority,"low");  
    char *buff= malloc(strlen("test incident") + 1);
    strcpy(buff,"test incident");
    dummy2->description = buff;
    strcpy(dummy2->status,"solved");


    sistem->incidents = dummy2;

    return sistem;
}


