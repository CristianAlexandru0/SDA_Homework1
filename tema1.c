/*Cristian Alexandru Catalin 312CC*/
#include "library.h"
#include <stdlib.h>
#include <string.h>

// insereaza o noua interventie in lista circulara dublu inlantuita de interventii
void push_intervention_list(struct incident * curr_incident, struct unit * curr_unit, struct system * sistem)
{
    struct intervention * new_intervention = malloc(sizeof(struct intervention));

    new_intervention->incident = curr_incident;
    new_intervention->unit = curr_unit;

    // se insereaza inainte de nodul santinela
    new_intervention->next = sistem->interventions;
    new_intervention->prev = sistem->interventions->prev;
    sistem->interventions->prev->next = new_intervention;
    sistem->interventions->prev = new_intervention;

    // se adauga pe stiva pentru a fi retinuta in istoric
    push_intervention_stack(new_intervention, sistem->history_stack);
}

// returneaza primul incident din coada
struct incident * extract_incident(qlist_i * queue)
{
    return queue->head->address;
}

// returneaza prima unitate din coada
struct unit * extract_unit(qlist_u * queue)
{
    return queue->head->address;
}

// cauta incidentul cu cel mai mare grad de gravitate si trimite o unitate pentru a-l rezolva
void dispatch(struct system *sistem, FILE *out)
{
    // ok = 1 daca sunt unitati disponibile si 0 daca nu
    int ok = check_units_availability(sistem->queue_available_units, out, 1);

    // verifica incidentele din cozile de asteptare (dpdv al prioritatii) si creeaza o interventie
    if (check_incident_availability(sistem->queue_high) > 0 && ok != 0){
        struct incident * curr_incident = extract_incident(sistem->queue_high);
        pop_incidents_queue(sistem->queue_high);
        strcpy(curr_incident->status,"intervened");

        struct unit * curr_unit = extract_unit(sistem->queue_available_units);
        pop_units_queue(sistem->queue_available_units);
         // unitatea nu mai poate fi trimisa in alte interventii
        curr_unit->availability = 0;

        push_intervention_list(curr_incident, curr_unit , sistem);
    }else if (check_incident_availability(sistem->queue_medium) > 0 && ok != 0){
        struct incident * curr_incident = extract_incident(sistem->queue_medium);
        pop_incidents_queue(sistem->queue_medium);
        strcpy(curr_incident->status,"intervened");

        struct unit * curr_unit = extract_unit(sistem->queue_available_units);
        pop_units_queue(sistem->queue_available_units);
        curr_unit->availability = 0;

        push_intervention_list(curr_incident, curr_unit , sistem);
    }else if (check_incident_availability(sistem->queue_low) > 0 && ok != 0){
        struct incident * curr_incident = extract_incident(sistem->queue_low);
        pop_incidents_queue(sistem->queue_low);
        strcpy(curr_incident->status,"intervened");

        struct unit * curr_unit = extract_unit(sistem->queue_available_units);
        pop_units_queue(sistem->queue_available_units);
        curr_unit->availability = 0;

        push_intervention_list(curr_incident, curr_unit , sistem);
    }else{
        // afiseaza eroare daca nu exista unitati disponibile sau incidente in coada de asteptare
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
    }

}

// anuleaza ultimul dispatch, elimina ultima interventie nerezolvata
void undo_last_dispatch(struct system *sistem, FILE *out)
{
    node_stack *curr_element = sistem->history_stack->head;
    // sare peste interventiile "solved"
    while (curr_element != NULL && strcmp(curr_element->address->incident->status, "intervened") != 0 ){
        pop_intervention_stack(sistem->history_stack);
        curr_element = sistem->history_stack->head;
    }
    if (curr_element == NULL){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }
    // incidentul devine "queued" si echipajul disponibil
    strcpy(curr_element->address->incident->status, "queued");
    curr_element->address->unit->availability = 1;
    struct intervention * delete = curr_element->address;
    // se adauga la inceputul cozii corespunzatoare
    if (strcmp(delete->incident->priority,"low") == 0){
        push_incidents_queue_first(delete->incident, sistem->queue_low);
    }else if (strcmp(delete->incident->priority,"medium") == 0){
        push_incidents_queue_first(delete->incident, sistem->queue_medium);
    }
    else if (strcmp(delete->incident->priority,"high") == 0){
        push_incidents_queue_first(delete->incident, sistem->queue_high);
    }
    // se scoate nodul din stiva
    pop_intervention_stack(sistem->history_stack);
    // unitatea se introduce in coada de unitati disponibile
    push_units_queue(delete->unit,sistem->queue_available_units);

    // se elimina interventia
    delete->prev->next = delete->next;
    delete->next->prev = delete->prev;
    free(delete);


}

// cu ajutorul unui id, se marcheaza incidentul respectiv ca "solved"
void solved_incident(FILE *f, struct system * sistem, FILE * out)
{
    int incident_id;
    fscanf(f, "%d", &incident_id);

    struct intervention *curr_intervention = sistem->interventions->next;
    // cautam in lista pana gasim id ul sau pana ajungem la capatul listei
    while (curr_intervention != sistem->interventions && curr_intervention->incident->id != incident_id){
        curr_intervention = curr_intervention->next;
    }

    // afiseaza eroare daca nu s a gasit id ul sau daca incidentul este deja rezolvat
    if (curr_intervention == sistem->interventions || strcmp(curr_intervention->incident->status, "solved") == 0){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    strcpy(curr_intervention->incident->status,"solved");
    // elibereaza echipajul si il pune inapoi in coada de echipaje
    curr_intervention->unit->availability = 1;
    push_units_queue(curr_intervention->unit, sistem->queue_available_units);
}

// afiseaza informatii despre unitati (id-ul, tipul, disponibilitatea)
void show_unit(FILE *f, struct system *sistem, FILE *out)
{
    int unit_id, i;
    fscanf(f, "%d", &unit_id); 
    // parcurge vectorul de unitati pana gaseste echipajul cu id-ul cautat
    for(i = 0; i < sistem->nr_unitati; i++){
        if(sistem->units[i].id == unit_id){
            if(sistem->units[i].availability == 1){
                fprintf(out, "Unit %d is type %c and is available\n", unit_id, sistem->units[i].type);
            }else{
                fprintf(out, "Unit %d is type %c and is unavailable\n", unit_id, sistem->units[i].type);
            }
            return;
        }
    }
    // daca nu e gasit afiseaza eroare
    fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

// afiseaza detaliile unui incident (id, prioritate, descriere, status)
void show_incident(FILE *f, struct system *sistem, FILE *out)
{
    int incident_id;
    fscanf(f, "%d", &incident_id);
    struct incident *curr_incident = sistem->incidents->next;
    while(curr_incident != sistem->incidents && curr_incident->id != incident_id){
        curr_incident = curr_incident->next;
    }

    if(curr_incident == sistem->incidents){
        fprintf(out, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    fprintf(out, "Incident %d has %s priority, the following description: %s and is %s\n", 
        incident_id, curr_incident->priority, curr_incident->description, curr_incident->status);
}

// afiseaza toate interventiile cu informatiile lor (id interventie, id unitate, status)
void show_intervention(struct system *sistem, FILE *out)
{
    if(sistem->interventions == sistem->interventions->next){
        fprintf(out, "No intervention has been initiated\n");
        return;
    }

    struct intervention * curr_intervention = sistem->interventions->next;
    while(curr_intervention != sistem->interventions){
        fprintf(out,"Incident %d was assigned to unit %d, and has the following status: \"%s\"\n", 
            curr_intervention->incident->id, curr_intervention->unit->id, curr_intervention->incident->status);
        curr_intervention = curr_intervention->next;
    }
}

// elibereaza sistemul
void free_all(struct system ** sistem)
{
    struct system *sistem_address = *sistem;
    // vectorul de unitati
    free(sistem_address->units);

    // lista de incidente
    struct incident *curr_incident = sistem_address->incidents->next;
    while(curr_incident != sistem_address->incidents){
        struct incident * free_incident = curr_incident;
        curr_incident = curr_incident->next;
        free(free_incident->description);
        free(free_incident);
    }
    free(sistem_address->incidents->description);
    free(sistem_address->incidents);

    // lista de interventii
    struct intervention *curr_intervention = sistem_address->interventions->next;
    while(curr_intervention != sistem_address->interventions){
        struct intervention * free_intervention = curr_intervention;
        curr_intervention = curr_intervention->next;
        free(free_intervention);
    }
    free(sistem_address->interventions);

    // stiva (history_stack)
    node_stack *curr_stack = sistem_address->history_stack->head;
    while(curr_stack != NULL){
        node_stack *free_stack = curr_stack;
        curr_stack = curr_stack->next;
        free(free_stack);
    }
    free(sistem_address->history_stack);

    // cozile cu incidente
    node_incident * curr_q_incident = sistem_address->queue_low->head;
    while(curr_q_incident != NULL){
        node_incident * free_q_incident = curr_q_incident;
        curr_q_incident = curr_q_incident->next;
        free(free_q_incident);
    }
    free(sistem_address->queue_low);

    curr_q_incident = sistem_address->queue_medium->head;
    while(curr_q_incident != NULL){
        node_incident * free_q_incident = curr_q_incident;
        curr_q_incident = curr_q_incident->next;
        free(free_q_incident);
    }
    free(sistem_address->queue_medium);

    curr_q_incident = sistem_address->queue_high->head;
    while(curr_q_incident != NULL){
        node_incident * free_q_incident = curr_q_incident;
        curr_q_incident = curr_q_incident->next;
        free(free_q_incident);
    }
    free(sistem_address->queue_high);

    //elibereaza coada cu echipaje
    node_unit * curr_q_unit = sistem_address->queue_available_units->head;
    while(curr_q_unit != NULL){
        node_unit * free_q_unit= curr_q_unit;
        curr_q_unit = curr_q_unit->next;
        free(free_q_unit);
    }
    free(sistem_address->queue_available_units);

    // elibereaza sistemul
    free(sistem_address);

}

int main()
{
    FILE *f = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    int nr_operatii;

    struct system *sistem = init_sistem(f);
    fscanf(f, "%d", &nr_operatii);

    char buff[150];

    while (nr_operatii!=0){
    // citeste numele operatiei 
    fscanf(f, "%s", buff);

    if(strcmp(buff, "ADD_INCIDENT") == 0){
        add_incident(sistem, f);
    }
    else if(strcmp(buff, "CHECK_UNITS_AVAILABILITY") == 0){
        check_units_availability(sistem->queue_available_units, out, 0);
    }else if(strcmp(buff, "DISPATCH") == 0){
        dispatch(sistem, out);
    }else if(strcmp(buff, "UNDO_LAST_DISPATCH") == 0){
        undo_last_dispatch(sistem, out);
    }else if(strcmp(buff, "SOLVED_INCIDENT") == 0){
        solved_incident(f, sistem, out);
    }else if(strcmp(buff, "SHOW_UNIT") == 0){
        show_unit(f, sistem, out);
    }else if(strcmp(buff, "SHOW_INCIDENT") == 0){
        show_incident(f, sistem, out);
    }else if(strcmp(buff, "SHOW_INTERVENTIONS") == 0){
        show_intervention(sistem, out);
    }

    nr_operatii--;
    }

    free_all(&sistem);
    return 0;
}