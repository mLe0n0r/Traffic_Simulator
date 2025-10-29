#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"
#include "generate_events.h"
#include "duration_of_calls.h"

#define CHEGADA 0
#define PARTIDA 1
#define general 0
#define specific 1
#define num_events 100000

double lambda = 0.0222;

typedef struct queue_list {
    double time;
    struct queue_list* next;
} queue_list;

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


EC_Metrics general_purpose(int lambda, int N_operators, int L){
    list * events_list = NULL;
	queue_list * queue = NULL;

    double ev_time = 0.0;

    int busy = 0;
	int arrivals = 0;

	int delayed = 0;
    int blocked = 0;

    events_list = generate_events(lambda, ev_time, events_list); // first event

    while (arrivals < num_events){

        
    }
}

int main(){

}