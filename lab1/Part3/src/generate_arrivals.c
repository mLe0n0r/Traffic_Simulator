#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "linked_list.h"

#define CHEGADA 0
#define PARTIDA 1
#define GENERAL 0
#define SPECIFIC 1

list* generate_events(double lambda, double event_time, list *list)
{
    double u_arriv = (rand() + 1.0) / (RAND_MAX + 2.0); 
    double c = -log(u_arriv) / lambda;

    double u_type = (rand() + 1.0) / (RAND_MAX + 2.0);
    int is_generic_only = (u_type < 0.3) ? GENERAL : SPECIFIC; // 30% only general, 70% will be transferred to the specific area

    int enc_purpose = is_generic_only;

    list = __add(list, CHEGADA, enc_purpose, c + event_time);

    return list;
}