/*Cristian Alexandru Catalin 312CC*/
#include "library.h"
#include <stdlib.h>
#include <string.h>

// se adauga un nou incident in lista
// inserarea se face inainte de santinela
void push_incident_list(struct incident *new_incident,  struct incident *incidents)
{
    new_incident->prev = incidents->prev;
    new_incident->next = incidents;
    incidents->prev->next = new_incident;
    incidents->prev = new_incident;
}

// Citeste datele unui incident din fisier si il adauga in lista
// incidentul este adaugat si in coada de prioritate
void add_incident(struct system *sistem, FILE *f )
{
    
    char buff[270];
    struct incident *new_incident = malloc(sizeof(struct incident));
    // initial starea nodului este in asteptare
    strcpy(new_incident->status,"queued");
    //citeste din fisier datele
    fscanf(f,"%d", &new_incident->id);
    fscanf(f,"%s", new_incident->priority);
    
    fgetc(f);
    fgets(buff,270,f);
    // aloca dinamic descrierea
    int length = strlen(buff);
    new_incident->description = malloc((length +1 ) * sizeof(char));
    strcpy(new_incident->description, buff);
    new_incident->description[length -1] = '\0';

    // adauga in lista
    push_incident_list(new_incident, sistem->incidents);

    // incidentul se adauga si in coada de prioritate respectiva
    if(!strcmp("low", new_incident->priority))
    {
        push_incidents_queue(new_incident, sistem->queue_low);
    }
    else if(!strcmp("medium", new_incident->priority))
    {
        push_incidents_queue(new_incident, sistem->queue_medium);
    }
    else if(!strcmp("high", new_incident->priority))
    {
        push_incidents_queue(new_incident, sistem->queue_high);
    }

}