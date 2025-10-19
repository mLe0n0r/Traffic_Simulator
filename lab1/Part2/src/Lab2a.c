#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"
#include "generate_events.h"

#define CHEGADA 0
#define PARTIDA 1

double erlang_B(int lambda, double dm, int N, int max_samps){
    int busy = 0;
    int blocked = 0;

    double ev_time = 0.0;
    int arrivals = 0;
    double blocking_prob = 0.0;

    list  * events_list = NULL;

    events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list); // first event

    while (arrivals < max_samps){

        int ev_type = events_list->type;
        ev_time = events_list->time;

        if(events_list != NULL) events_list = __remove(events_list); // remove the event that passed

        if(ev_type == CHEGADA){
            arrivals++;

            if(busy < N){ // if is not blocked we can generate the next departure
                busy++;
                events_list = generate_events(lambda, dm, PARTIDA, ev_time, events_list);
            }

            else blocked++;
            // in both cases we generate the next arrival
            events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list);
        }

        else{
            busy--; //if the next event is departure we decrease the counter
        }
    }
    return blocking_prob = ((double)blocked / (double)arrivals) * 100;
}

int main(void) // Earlang-B distribution
{
    srand(time(NULL));

    for(int N = 1; N < 11; N++){
        double prob = erlang_B(200, 0.008, N, 100000);
        printf("Blocking probability when having %d services: %.2f\n", N, prob);
    }   
}