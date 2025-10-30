#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "linked-list.h"

#define CHEGADA 0
#define PARTIDA 1
#define GENERAL 0
#define SPECIFIC 1

list* generate_events(double lambda, double event_time, list *list)
{
    double max = 5.0 / lambda;
    double delta = 1.0 / (5.0 * lambda);

    double u = (rand() + 1.0) / (RAND_MAX + 2.0); 
    double c = -log(u) / lambda;

    if (u < 0.3) list = __add(list, CHEGADA, GENERAL, c + event_time); // adiciona novo evento
    else list = __add(list, CHEGADA, SPECIFIC, c + event_time);
 
    return list;
}