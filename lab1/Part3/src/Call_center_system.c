#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"
#include "generate_arrivals.h"
#include "duration_of_calls.h"

#define CHEGADA 0
#define PARTIDA 1
#define GENERAL 0
#define SPECIFIC 1
#define num_events 100000

double lambda = 0.0222;

typedef struct
{
    double delay_prob;
    double avg_delay;
    double p_delay_gt_Ax;
    double blocking_prob;
    double bin_width; 
    int num_bins; 
    int *histogram; 
} EC_Metrics;

// Forward declaration for cross-call
EC_Metrics area_specific(int N_operators, double arrival_time);


EC_Metrics general_purpose(double lambda, int N_operators, int N_specific, int L){
    list * events_list = NULL;
	queue_list * queue = NULL;

    double ev_time = 0.0;

    int busy = 0;
	int arrivals = 0;

	int delayed = 0;
    int blocked = 0;

    int in_queue = 0;

    events_list = generate_events(lambda, ev_time, events_list); // first event

    while (arrivals < num_events){
        int ev_type = events_list->type;
        int ev_purpose = events_list->purpose;
        ev_time = events_list->time;

        if(events_list != NULL) events_list = __remove(events_list);

        if(ev_type == CHEGADA){
            arrivals++;
            
            if(busy < N_operators){ 
                busy++;
                events_list = __add(events_list, PARTIDA, ev_purpose, ev_time + call_duration(GENERAL, GENERAL)); // we know that by the time the call finishes we have a departure
            }

            else {
                if(in_queue < L){
                    delayed++;
                    queue = __add_queue(queue, ev_purpose, ev_time); // we store the time the event entered the queue
                    in_queue++;
                }
                else blocked++;
            }
            
            events_list = generate_events(lambda, ev_time, events_list); // in both cases we generate the next arrival

        }
        else{ // we have a departure
            double ev_arrival;
        
            if(queue != NULL){ // call was not immediately answered
                in_queue--;
                queue = __remove_queue(queue, &ev_arrival); // we are going to obtain the time the event entered the queue

                if(ev_purpose == 0){ // is a general call
                    events_list = __add(events_list, PARTIDA, ev_purpose, ev_time + call_duration(GENERAL, GENERAL)); // we calculate that call's departure
                }
                else{ // is area-specific
                    double arrival_time = ev_time + call_duration(SPECIFIC, GENERAL); // time the call will be transferred
                    events_list = __add(events_list, PARTIDA, ev_purpose, arrival_time); // the call that is now been considered will departure after more x seconds of duration
                    area_specific(N_specific, arrival_time);
                }
            }
            else{
                busy--;
                in_queue = 0;
            }
        }
    }

    EC_Metrics results = {0};
    results.delay_prob = ((double)delayed / (double)arrivals) * 100;
    return results;
}

EC_Metrics area_specific(int N_operators, double arrival_time){
    list * calls_list = NULL;
    queue_list * queue = NULL;

    double ev_time = 0.0;

    int busy = 0;
    int delayed = 0;
    int in_queue = 0;

    calls_list = __add(calls_list, CHEGADA, SPECIFIC, arrival_time); // first arrival
    int ev_type = calls_list->type;

    if(ev_type == CHEGADA){
        if (busy < N_operators){ // if its available
            busy++;
            calls_list = __add(calls_list, PARTIDA, SPECIFIC, arrival_time + call_duration(SPECIFIC, SPECIFIC));
        }
        else{ // goes to the queue
            queue = __add_queue(queue, SPECIFIC, arrival_time);
            delayed++;
            in_queue++;
        }
    }
    else{ // departure
        double ev_arrival;

        if(queue != NULL){
            in_queue--;
            queue = __remove_queue(queue, &ev_arrival);
        }
        else{
            busy--;
            in_queue = 0;
        }
    }

    EC_Metrics results = {0};
    return results;   
}

int main(void){
    srand(time(NULL));

    for(int N_general = 1; N_general < 11; N_general++){
        for (int length = 1; length < 38; length += 2){
            EC_Metrics result = general_purpose(lambda, N_general, 5, length);
            printf("%d general operators and a queue of length %d: %f\n", N_general, length, result.delay_prob);
        }
    }
    
}