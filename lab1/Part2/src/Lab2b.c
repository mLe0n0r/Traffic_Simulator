#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"
#include "generate_events.h"

#define CHEGADA 0
#define PARTIDA 1

typedef struct queue_list {
    double time;
    struct queue_list* next;
} queue_list;

typedef struct
{
    double delay_prob;
    double avg_delay;
    double p_delay_gt_Ax;
} EC_Metrics;


EC_Metrics erlang_C(int lambda, double dm, int N, int max_samps, double Ax){
    int busy = 0;
	int arrivals = 0;
	int delayed = 0;
    int delayed_gt_Ax = 0;
    double sum_delay = 0.0;
    double delay = 0.0;

    double ev_time = 0.0;

    list * events_list = NULL;
	queue_list * queue = NULL;

    events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list); // first event

    while (arrivals < max_samps){

        int ev_type = events_list->type;
        ev_time = events_list->time;

        if(events_list != NULL) events_list = __remove(events_list); // remove the event that passed

        if(ev_type == CHEGADA){
            arrivals++;
            
            if(busy < N){ // if is free the event is automatically attended
                busy++;
                events_list = generate_events(lambda, dm, PARTIDA, ev_time, events_list);
            }

            else {
                delayed++;
                queue = __add_queue(queue, ev_time); // we store the time the event entered the queue
            }
            // in both cases we generate the next arrival
            events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list);
        }

        else{ // Partida
            if(queue != NULL){ // if the queue is not empty the service continues as busy
                double ev_arrival;
                queue = __remove_queue(queue, &ev_arrival); // we are going to obtain the time the event entered the queue
                events_list = generate_events(lambda, dm, PARTIDA, ev_time, events_list); // now that the event got out the queue we can generate a new departure

                delay = ev_time - ev_arrival;
                sum_delay += delay;
                if (delay > Ax) delayed_gt_Ax++;
            }
            else busy--;
            
        }
    }

    EC_Metrics results;
    results.delay_prob = ((double)delayed / (double)arrivals) * 100;
    results.avg_delay = (double)(sum_delay/arrivals);
    results.p_delay_gt_Ax = ((double)delayed_gt_Ax / (double)arrivals) * 100;

    return results;
    
}

int main(void){
    srand(time(NULL));
   
    for(int N = 1; N < 7; N++){
        EC_Metrics result = erlang_C(200, 0.008, N, 10000, 0.02);
        printf("Metric when having %d services:\n", N);
        printf("Delay probability: %.2f\n", result.delay_prob);
        printf("Average delay (Am): %f\n", result.avg_delay);
        printf("Probability of A > Ax: %.2f\n", result.p_delay_gt_Ax);
        printf("\n");
    }   
}