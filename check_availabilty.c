/*Cristian Alexandru Catalin 312CC*/
#include "library.h"

// afiseaza (option == 0) sau returneaza (option == 1) numarul de unitati disponibile
// (option == 1) se foloseste in functia dispatch
int check_units_availability(qlist_u *queue_units, FILE *out , int option)
{
    
    node_unit *current = queue_units->head;
    int c = 0; // numarul de unitati disponibile
    if(option == 0)
    {
    if(current == NULL)
    {
        fprintf(out, "Number of available units: 0\n");
        return c;
    }
    }
    // numara nodurile din coada
    while(current != NULL)
    {
        c++;
        current = current->next;
    }
    if(option == 0)
    {
    fprintf(out, "Number of available units: %d\n", c);
    }
    return c;
}
// returneaza numarul de incidente din coada
int check_incident_availability(qlist_i *queue_incidents)
{
    node_incident *current = queue_incidents->head;
    int c = 0; 
    if(current == NULL)
    {
        return 0;
    }
    
    while(current != NULL)
    {
        c++;
        current = current->next;
    }
    return c;
}
