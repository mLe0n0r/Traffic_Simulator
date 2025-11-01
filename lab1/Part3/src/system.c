#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked_list.h"
#include "generate_arrivals.h"
#include "duration_of_calls.h"

#define CHEGADA 0
#define PARTIDA 1
#define GENERAL 0
#define SPECIFIC 1
#define num_events 10000

double lambda = 0.0222;
double gen_ratio = 0.3;

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

// void handle_general_call(list** events_list, list** specific_list, queue_list** queue, int N_general, int L, int *busy, int *delayed, int *blocked, int *in_queue, double *delay, double *sum_delay, int ev_type, int ev_purpose, double ev_time){
//     if(ev_type == CHEGADA){
//         if(*busy < N_general){ // is imediately answered
//             (*busy)++;
//             *events_list = __add(*events_list, PARTIDA, ev_purpose, ev_time + call_duration(ev_purpose, GENERAL));
//         }
//         else{ // goes to queue
//             if(*in_queue < L){
//                 (*delayed)++;
//                 *queue = __add_queue(*queue, ev_purpose, ev_time); // we store the time the event entered the queue
//                 (*in_queue)++;
//             }
//             else (*blocked)++;
//         }
//         *events_list = generate_events(lambda, ev_time, *events_list);  // in both case we will generate a new arrival
//     }
//     else{
//         if (ev_purpose == SPECIFIC) {
//             *specific_list = __add(*specific_list, CHEGADA, SPECIFIC, ev_time);
//         }

//         if(*queue != NULL){ // call was in the queue
//             double ev_arrival;
//             int q_purpose;
//             *queue = __remove_queue(*queue, &q_purpose, &ev_arrival); // ev_arrival to calculate delay

//             *events_list = __add(*events_list, PARTIDA, GENERAL, ev_time + call_duration(q_purpose, GENERAL));

//             if (*in_queue > 0) (*in_queue)--;

//             *delay = ev_time - ev_arrival;
//             *sum_delay += *delay;
//         }
//         else{
//             (*busy)--; 
//             if (*busy < 0) *busy = 0;
//             //if(ev_purpose == SPECIFIC) *specific_list = __add(*specific_list, CHEGADA, SPECIFIC, ev_time);
//         }
//     }
// }



// void handle_specific_call(list **events_specific, queue_list **queue_specific, int N_specific, int *busy_specific, int *delayed_specific, int *in_queue_specific, double *delay_specific, double *sum_delay_specific, int ev_type, int ev_purpose, double ev_time){

//     if(ev_type == CHEGADA){
//         if (*busy_specific < N_specific){ // imediately answered
//             (*busy_specific)++;
//             *events_specific = __add(*events_specific, PARTIDA, SPECIFIC, ev_time + call_duration(SPECIFIC, SPECIFIC));
//         }
//         else{
//             (*delayed_specific)++;
//             *queue_specific = __add_queue(*queue_specific, SPECIFIC, ev_time); // we store the time the event entered the queue
//             (*in_queue_specific)++; 
//         }
//     }
//     else{ // departure
//         if(*queue_specific != NULL){ // call was in the queue
//             double ev_arrival;
//             int q_purpose;
//             *queue_specific = __remove_queue(*queue_specific, &q_purpose, &ev_arrival);

//             *delay_specific = ev_time - ev_arrival;
//             *sum_delay_specific += *delay_specific;

//             *events_specific = __add(*events_specific, PARTIDA, SPECIFIC, ev_time + call_duration(SPECIFIC, SPECIFIC));

//             if (*in_queue_specific > 0) (*in_queue_specific)--;
//         }
//         else{
//             (*busy_specific)--;
//             if (*busy_specific < 0) *busy_specific = 0;
//             *in_queue_specific = 0;
//         }
//     }
// }



// EC_Metrics call_center_system(double lambda, int N_general, int N_specific, int L){
//     list * events_list = NULL;
//     list * specific_list = NULL;
// 	queue_list * queue = NULL;
//     queue_list * queue_specific = NULL;

//     double ev_time = 0.0;
//     double sum_delay = 0.0;
//     double delay = 0.0;

//     int busy = 0;
//     int busy_specific = 0;
// 	int arrivals = 0;

// 	int delayed = 0;
//     int delayed_specific = 0;
//     int blocked = 0;

//     int in_queue = 0;
//     int in_queue_specific = 0;

//     events_list = generate_events(lambda, ev_time, events_list); // first event

//     while (arrivals < num_events){
//         double next_general_time = (events_list != NULL) ? events_list->time : 1e18;
//         double next_specific_time = (specific_list != NULL) ? specific_list->time : 1e18;

//         if(next_general_time <= next_specific_time){
//             int ev_type = events_list->type;
//             int ev_purpose = events_list->purpose;
//             double ev_time = events_list->time;

//             events_list = __remove(events_list);

//             if(ev_type == CHEGADA) arrivals++;

//             handle_general_call(&events_list, &specific_list, &queue, N_general, L, &busy, &delayed, &blocked, &in_queue, &delay, &sum_delay, ev_type, ev_purpose, ev_time);
//         }
//         else{
//             int ev_type_s = specific_list->type;
//             int ev_purpose_s = specific_list->purpose;
//             double ev_time_s = specific_list->time;

//             specific_list = __remove(specific_list);

//             handle_specific_call( &specific_list, &queue_specific, N_specific, &busy_specific, &delayed_specific, &in_queue_specific, &delay, &sum_delay, ev_type_s, ev_purpose_s, ev_time_s);
//         } 
//     }
    
//     EC_Metrics results = {0};
//     results.delay_prob = (double)delayed/arrivals * 100;
//     results.blocking_prob = (double)blocked/arrivals * 100;

//     return results;
// }

// int main(void){ 
//     srand(time(NULL)); 

//     for(int N_general = 1; N_general < 5; N_general++){ 
//         for(int L = 1; L < 10; L++){ 
//             EC_Metrics result = call_center_system(lambda, N_general, 5, L); 
//             printf("%d operators and a queue of length %d:\n", N_general, L); 
//             printf("Prob delay: %.2f %%\n", result.delay_prob); 
//             printf("Prob block: %.2f %%\n", result.blocking_prob); 
//             printf("\n"); 
//         } 
//     } 
// }
#define TARGET_DELAY_PCT    30.0
#define TARGET_BLOCK_PCT     2.0
#define TARGET_AVG_DELAY    30.0

#define MAX_DELAY_PCT       30.0
#define MAX_BLOCK_PCT        2.0
#define MAX_AVG_DELAY       30.0

void handle_general_call(list** events_list, list** specific_list, queue_list** queue, int N_general, int L, int *busy, int *delayed, int *blocked, int *in_queue, double *delay, double *sum_delay, int ev_type, int ev_purpose, double ev_time){
    
    if(ev_type == CHEGADA){
        if(*busy < N_general){ // call is immediately answered
            (*busy)++;
            *events_list = __add(*events_list, PARTIDA, ev_purpose, ev_time + call_duration(ev_purpose, GENERAL));
        }
        else{ // goes to the queue
            if(*in_queue < L){
                (*delayed)++;
                *queue = __add_queue(*queue, ev_purpose, ev_time);
                (*in_queue)++;
            }
            else (*blocked)++;
        }

        *events_list = generate_events(lambda, ev_time, *events_list);
    }

    else{ // departure
        if (*queue != NULL){ // call was in the queue
            double ev_arrival;
            int q_purpose;

            *queue = __remove_queue(*queue, &q_purpose, &ev_arrival);
            if (*in_queue > 0) (*in_queue)--;

            *delay = ev_time - ev_arrival;
            *sum_delay += *delay;

            *events_list = __add(*events_list, PARTIDA, q_purpose, ev_time + call_duration(q_purpose, GENERAL)); // departure of the call now in the operator

            if(q_purpose == SPECIFIC) *specific_list = __add(*specific_list, CHEGADA, SPECIFIC, ev_time); // goes to the specific service
            
        }
        else{ // queue is empty
            (*busy)--; 
            if (*busy < 0) *busy = 0;

            if(ev_purpose == SPECIFIC) *specific_list = __add(*specific_list, CHEGADA, SPECIFIC, ev_time);  // goes to the specific service
            
        }
    }
}

void handle_specific_call(list **events_specific, queue_list **queue_specific, int N_specific, int *busy_specific, int *delayed_specific, int *in_queue_specific, double *delay_specific, double *sum_delay_specific, int ev_type, int ev_purpose, double ev_time){
    
    if(ev_type == CHEGADA){
        if (*busy_specific < N_specific){ // call is immediately answered
            (*busy_specific)++;
            *events_specific = __add(*events_specific, PARTIDA, SPECIFIC, ev_time + call_duration(SPECIFIC, SPECIFIC));
        }
        else{ // goes to infinite queue
            (*delayed_specific)++;
            *queue_specific = __add_queue(*queue_specific, SPECIFIC, ev_time);
            (*in_queue_specific)++;
        }
    }
    else{ // Departure
        if (*queue_specific != NULL){ // call was in the queue
            double ev_arrival;
            int q_purpose;

            *queue_specific = __remove_queue(*queue_specific, &q_purpose, &ev_arrival);
            if (*in_queue_specific > 0) (*in_queue_specific)--;

            *delay_specific = ev_time - ev_arrival;
            *sum_delay_specific += *delay_specific;

            *events_specific = __add(*events_specific, PARTIDA, SPECIFIC, ev_time + call_duration(SPECIFIC, SPECIFIC));
        }
        else{ // queue is empty
            (*busy_specific)--;
            if (*busy_specific < 0) *busy_specific = 0;
        }
    }
}

EC_Metrics call_center_system(double lambda, int N_general, int N_specific, int L){
    list * events_list = NULL;
    list * specific_list = NULL;
	queue_list * queue = NULL;
    queue_list * queue_specific = NULL;

    double ev_time = 0.0;
    double sum_delay = 0.0;
    double delay = 0.0;

    double sum_delay_specific = 0.0;
    double delay_specific = 0.0;

    int busy = 0;
    int busy_specific = 0;
	int arrivals = 0;

	int delayed = 0;
    int delayed_specific = 0;
    int blocked = 0;

    int in_queue = 0;
    int in_queue_specific = 0;

    events_list = generate_events(lambda, ev_time, events_list); // first event

    while (arrivals < num_events){
        // to check which event is the next in time 
        double next_general_time = (events_list != NULL) ? events_list->time : 1e18;
        double next_specific_time = (specific_list != NULL) ? specific_list->time : 1e18;

        if(next_general_time <= next_specific_time){
            int ev_type = events_list->type;
            int ev_purpose = events_list->purpose;
            double ev_time = events_list->time;

            events_list = __remove(events_list);

            if(ev_type == CHEGADA) arrivals++;

            handle_general_call(&events_list, &specific_list, &queue, N_general, L, &busy, &delayed, &blocked, &in_queue, &delay, &sum_delay, ev_type, ev_purpose, ev_time);
        }
        else{
            int ev_type_s = specific_list->type;
            int ev_purpose_s = specific_list->purpose;
            double ev_time_s = specific_list->time;

            specific_list = __remove(specific_list);

            handle_specific_call( &specific_list, &queue_specific, N_specific, &busy_specific, &delayed_specific, &in_queue_specific, &delay_specific, &sum_delay_specific, ev_type_s, ev_purpose_s, ev_time_s);
        } 
    }
    
    EC_Metrics results = {0};
    results.delay_prob = (double)delayed/arrivals * 100;
    results.blocking_prob = (double)blocked/arrivals * 100;
    results.avg_delay = (double)sum_delay/delayed;

    return results;
}

// int main(void){ 
//     srand(time(NULL)); 
    
//     for(int N_general = 1; N_general < 6; N_general++){ 
//         for(int L = 1; L < 15; L++){ 
//             EC_Metrics result = call_center_system(lambda, N_general, 5, L); 
//             printf("%d operators and a queue of length %d:\n", N_general, L); 
//             printf("Probability that a call is delayed : %.2f %%\n", result.delay_prob); 
//             printf("Probability that a call is blocked: %.2f %%\n", result.blocking_prob); 
//             printf("Average delay of the calls: %.2f s\n", result.avg_delay); 
//             printf("\n");
//         } 
//     } 
// }

static EC_Metrics run_avg(double lambda, int Ng, int Ns, int L, int reps) {
    EC_Metrics acc = {0};
    unsigned base = (unsigned)time(NULL);

    for (int r = 0; r < reps; r++) {
        srand(base + 1337u * (unsigned)(Ng*101 + L*997 + Ns*17 + r));
        EC_Metrics m = call_center_system(lambda, Ng, Ns, L);
        acc.delay_prob    += m.delay_prob;
        acc.blocking_prob += m.blocking_prob;
        acc.avg_delay     += m.avg_delay;
    }
    acc.delay_prob    /= reps;
    acc.blocking_prob /= reps;
    acc.avg_delay     /= reps;
    return acc;
}

int main(void){
    int best_Ng = -1, best_L = -1;
    EC_Metrics best = {0};
    double best_tie_score = 1e300; 

    for (int Ng = 1; Ng <= 6; Ng++){
        for (int L = 1; L <= 15; L++){
            EC_Metrics m = run_avg(lambda, Ng, 1, L, 5); // 5 reps

            if (m.delay_prob    <= MAX_DELAY_PCT &&
                m.blocking_prob <= MAX_BLOCK_PCT &&
                m.avg_delay     <= MAX_AVG_DELAY)
            {
                if (best_Ng == -1) {
                    best_Ng = Ng; best_L = L; best = m;
                    best_tie_score = fabs(m.delay_prob - TARGET_DELAY_PCT)
                                   + fabs(m.blocking_prob - TARGET_BLOCK_PCT)
                                   + fabs(m.avg_delay - TARGET_AVG_DELAY);
                } else {
                    if (Ng < best_Ng) {
                        best_Ng = Ng; best_L = L; best = m;
                        best_tie_score = fabs(m.delay_prob - TARGET_DELAY_PCT)
                                       + fabs(m.blocking_prob - TARGET_BLOCK_PCT)
                                       + fabs(m.avg_delay - TARGET_AVG_DELAY);
                    }
                    else if (Ng == best_Ng && L < best_L) {
                        best_Ng = Ng; best_L = L; best = m;
                        best_tie_score = fabs(m.delay_prob - TARGET_DELAY_PCT)
                                       + fabs(m.blocking_prob - TARGET_BLOCK_PCT)
                                       + fabs(m.avg_delay - TARGET_AVG_DELAY);
                    }
                    else if (Ng == best_Ng && L == best_L) {
                        double score = fabs(m.delay_prob - TARGET_DELAY_PCT)
                                     + fabs(m.blocking_prob - TARGET_BLOCK_PCT)
                                     + fabs(m.avg_delay - TARGET_AVG_DELAY);
                        if (score < best_tie_score){
                            best_tie_score = score;
                            best = m;
                        }
                    }
                }
            }
        }
    }

    if (best_Ng >= 0){
        printf("====== Best combination ======\n");
        printf("Ng=%d, L=%d\n", best_Ng, best_L);
        printf("Probability of a call being delayed = %.2f%% (<= %.2f%%)\n", best.delay_prob, MAX_DELAY_PCT);
        printf("Probability of a call being lost = %.2f%% (<= %.2f%%)\n", best.blocking_prob, MAX_BLOCK_PCT);
        printf("Average delay of the calls = %.2fs (<= %.2fs)\n", best.avg_delay, MAX_AVG_DELAY);
    } else {
        printf("No combination met all the limits.\n");
    }
    return 0;
}

