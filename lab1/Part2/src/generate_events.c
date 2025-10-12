#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "linked-list.h"

#define CHEGADA 0
#define PARTIDA 1

list* generate_events(double lambda, double dm, int event_typ, double event_time, list *list)
{
    double max = 5.0 / lambda;
    double delta = 1.0 / (5.0 * lambda);

    if(event_typ == CHEGADA){
        double u = (rand() + 1.0) / (RAND_MAX + 2.0); 
        double c = -log(u) / lambda;

        list = __add(list, event_typ, c + event_time); // adiciona novo evento
    } 

    else{
        double u = (rand() + 1.0) / (RAND_MAX + 2.0);
        double d = -dm * log(u);
        list = __add(list, event_typ, d + event_time);  
    }
 
    return list;
}

// int main(void)
// {
//     srand(time(NULL));
//     list  * event_list = NULL;
//     event_list = generate_events(5, 0.008, CHEGADA, 0, event_list);
//     printf("%f\n", event_list->time);
// }
