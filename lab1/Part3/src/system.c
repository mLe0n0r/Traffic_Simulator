#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked_list.h"
#include "generate_arrivals.h"
#include "duration_of_calls.h"

#define CHEGADA    0
#define PARTIDA    1
#define GENERAL    0
#define SPECIFIC   1
#define num_events 10000

#define MAX_DELAY 30.0
#define MAX_BLOCK 2.0
#define MAX_AVG_DELAY 30.0
#define MAX_TOTAL_TIME 90.0

#define MAX_HIST_ERROR 30.0
#define MAX_HIST_DELAY 60.0
#define NUM_BINS 15 
#define DELAYS_NUM_BINS 20

double lambda = 0.0222;

typedef struct
{
    double delay_prob;
    double blocking_prob;
    double avg_delay;
    double avg_total_time;
    double avg_abs_err;
    double avg_rel_err;
    double prediction;
    double bin_width; 
    double bin_width_delays;
    int num_bins; 
    int num_bins_delays;
    int *delays_histogram;
    int *histogram; 
} EC_Metrics;

void handle_general_call(list** events_list, list** specific_list, queue_list** queue, int N_general, int L, int *busy, int *delayed, int *blocked, int *in_queue, int *out_of_queue, double *sum_delay, int ev_type, int ev_purpose, double ev_time, double ev_arrival, int arrivals, double *running_avg, double *sum_abs_error, double *sum_rel_error, double *prediction, int *delays_histogram, double delta_delays, int *histogram, double bin_width){
    double delay = 0.0;
    double abs_error = 0.0;
    double rel_error = 0.0;
    double arrival_to_general; 

    if(ev_type == CHEGADA){
        arrival_to_general = ev_time; // For arrivals, the arrival time to general is the current event time
        
        if(*busy < N_general) { // call is immediately answered
            (*busy)++;
            double dur = call_duration(ev_purpose, GENERAL);

            *events_list = __add(*events_list, PARTIDA, ev_purpose, ev_time + dur, arrival_to_general); // schedule departure 
        }
        else { // goes to the queue
            if(*in_queue < L){
                double predicted_wait = *running_avg;
                *prediction += predicted_wait;
                (*delayed)++;
                *queue = __add_queue(*queue, ev_purpose, ev_time, arrival_to_general, predicted_wait);
                (*in_queue)++;
            }
            else (*blocked)++;
        }

        if(arrivals < num_events) *events_list = generate_events(lambda, ev_time, *events_list); // next arrival
    }
    else // departure
    {
        arrival_to_general = ev_arrival; // For departures, we use the arrival time that was stored
         
        double arrival_time_G;
        double arrival_time_S;
        int q_purpose;

        if (*queue != NULL){
            (*out_of_queue)++;
            double prediction = 0.0;

            *queue = __remove_queue(*queue, &q_purpose, &arrival_time_S, &arrival_time_G, &prediction);
            if (*in_queue > 0) (*in_queue)--;

            delay = ev_time - arrival_time_G;
            *sum_delay += delay;

            // build the delays histogram:
            int i = (int)(delay / delta_delays);
            if (i >= DELAYS_NUM_BINS) i = DELAYS_NUM_BINS - 1; 
            delays_histogram[i]++;

            if (*out_of_queue> 0) *running_avg = ((*running_avg * (*out_of_queue- 1)) + delay) / *out_of_queue;
            abs_error = fabs(prediction - delay);
            rel_error = (delay > 0) ? abs_error / delay : 0;
            *sum_abs_error += abs_error;
            *sum_rel_error += rel_error;

            // build the histogram:
            int bin_index = (int)(abs_error / bin_width);
            if (bin_index >= NUM_BINS) bin_index = NUM_BINS - 1; 
            histogram[bin_index]++;
            
            double dur = call_duration(q_purpose, GENERAL);

            *events_list = __add(*events_list, PARTIDA, q_purpose, ev_time + dur, arrival_time_G); // departure of the call now in the operator

            if(q_purpose == SPECIFIC)
                *specific_list = __add(*specific_list, CHEGADA, SPECIFIC, ev_time, arrival_time_G); // goes to the specific service
        }
        else{ // queue is empty
            (*busy)--;

            if(ev_purpose == SPECIFIC) *specific_list = __add(*specific_list, CHEGADA, SPECIFIC, ev_time, arrival_to_general); 
        }
    }
}

void handle_specific_call(list **events_specific, queue_list **queue_specific, int N_specific, int *busy_specific, int *delayed_specific, int *in_queue_specific, double arrival_G, double *sum_total_time, int ev_type, double ev_time, int *specific_calls){
    
    if(ev_type == CHEGADA){
        if (*busy_specific < N_specific) { // call is immediately processed
            (*busy_specific)++;

            double total_time = ev_time - arrival_G;
            *sum_total_time += total_time;
            (*specific_calls)++;

            double durS = call_duration(SPECIFIC, SPECIFIC);
            *events_specific = __add(*events_specific, PARTIDA, SPECIFIC, ev_time + durS, arrival_G);
        }
        else{ // goes to infinite queue
            (*delayed_specific)++;
            *queue_specific = __add_queue(*queue_specific, SPECIFIC, ev_time, arrival_G, 0.0);
            (*in_queue_specific)++;
        }
    }
    else { // departure
        if (*queue_specific != NULL){
            double arrival_S_queue;
            double arrival_G_queue;
            int q_purpose;
            double dummy = 0.0;

            *queue_specific = __remove_queue(*queue_specific, &q_purpose, &arrival_S_queue, &arrival_G_queue, &dummy);
            if (*in_queue_specific > 0) (*in_queue_specific)--;

            double total_time = ev_time - arrival_G_queue;
            *sum_total_time += total_time;
            (*specific_calls)++;

            double durS = call_duration(SPECIFIC, SPECIFIC);
            *events_specific = __add(*events_specific, PARTIDA, SPECIFIC, ev_time + durS, arrival_G_queue);
        }
        else{ // queue is empty
            (*busy_specific)--;
            if (*busy_specific < 0) *busy_specific = 0;
        }
    }
}

EC_Metrics call_center_system(double lambda, int N_general, int N_specific, int L)
{
    list * events_list = NULL;
    list * specific_list = NULL;
	queue_list * queue = NULL;
    queue_list * queue_specific = NULL;

    int busy = 0;
    int busy_specific = 0;

    int arrivals = 0;
    int specific_calls = 0;

    int delayed = 0; 
    int delayed_specific = 0;
    int out_of_queue = 0;
    int blocked = 0;

    int in_queue = 0;
    int in_queue_specific = 0;
    
    double sum_delay = 0.0, delay = 0.0;
    double sum_total_time = 0.0;

    double running_avg = 0.0;
    double sum_abs_error = 0.0;
    double sum_rel_error = 0.0;
    double prediction = 0.0;

    // ------------------- Histogram --------------------
    double delta_delays = MAX_HIST_DELAY / (double)DELAYS_NUM_BINS;

    int *delays_histogram = malloc(DELAYS_NUM_BINS * sizeof(int));
    for (int k = 0; k < DELAYS_NUM_BINS; k++) delays_histogram[k] = 0;

    double delta = MAX_HIST_ERROR / (double)NUM_BINS;

    int *histogram = malloc(NUM_BINS * sizeof(int));
    for (int k = 0; k < NUM_BINS; k++) histogram[k] = 0;

    // ---------------------------------------------------

    events_list = generate_events(lambda, 0.0, events_list); // first event

    while (arrivals < num_events || events_list != NULL || specific_list != NULL){
        // to check which event is the next in time
        double nextG = events_list ? events_list->time : 1e18;
        double nextS = specific_list ? specific_list->time : 1e18;

        if(nextG <= nextS){
            int ev_type = events_list->type;
            int ev_purpose = events_list->purpose;
            double ev_time = events_list->time;
            double ev_arrival_time = events_list->arrival;

            events_list = __remove(events_list);

            if(ev_type == CHEGADA) arrivals++;

            handle_general_call(&events_list, &specific_list, &queue, N_general, L, &busy, &delayed, &blocked, &in_queue, &out_of_queue, &sum_delay, ev_type, ev_purpose, ev_time, ev_arrival_time, arrivals, &running_avg, &sum_abs_error, &sum_rel_error, &prediction, delays_histogram, delta_delays, histogram, delta);
        }
        else{
            int ev_type = specific_list->type;
            double ev_time = specific_list->time;
            double arrival_GP = specific_list->arrival; 

            specific_list = __remove(specific_list);

            handle_specific_call(&specific_list, &queue_specific, N_specific, &busy_specific, &delayed_specific, &in_queue_specific, arrival_GP, &sum_total_time, ev_type, ev_time, &specific_calls);
        }
    }
    EC_Metrics results;
    results.delay_prob = (double)delayed / arrivals * 100.0;
    results.blocking_prob = (double)blocked / arrivals * 100.0;
    results.avg_delay = (delayed > 0) ? (double)sum_delay / delayed : 0.0;
    results.avg_total_time = (specific_calls > 0) ? (double)sum_total_time / specific_calls : 0.0;
    results.avg_abs_err = (delayed > 0) ? (double)sum_abs_error / delayed : 0.0;
    results.avg_rel_err = (delayed > 0) ? (double)sum_rel_error / delayed : 0.0;
    results.prediction = (double)prediction/delayed;
    results.num_bins = NUM_BINS;
    results.num_bins_delays = DELAYS_NUM_BINS;
    results.bin_width_delays = delta_delays;
    results.bin_width = delta;
    results.delays_histogram = delays_histogram;
    results.histogram = histogram;

    return results;
}

static EC_Metrics run_avg(double lambda, int Ng, int Ns, int L, int reps) {
    EC_Metrics acc = {0};
    unsigned base = (unsigned)time(NULL);

    for (int r = 0; r < reps; r++) {
        srand(base + 1337u * (unsigned)(Ng*101 + L*997 + Ns*17 + r));
        EC_Metrics m = call_center_system(lambda, Ng, Ns, L);
        acc.delay_prob += m.delay_prob;
        acc.blocking_prob += m.blocking_prob;
        acc.avg_delay += m.avg_delay;
        acc.avg_total_time += m.avg_total_time;
    }
    acc.delay_prob /= reps;
    acc.blocking_prob /= reps;
    acc.avg_delay /= reps;
    acc.avg_total_time /= reps;
    return acc;
}

int main(void){
    srand(time(NULL));

    // Results for every case:
    for(int N_general = 6; N_general < 7; N_general++){ 
        for(int L = 4; L < 5; L++){ 
            for(int N_specific = 5; N_specific < 6; N_specific++){

                EC_Metrics result = call_center_system(lambda, N_general, N_specific, L);
                
                printf("%d general operators, queue of length %d and %d area-specific operators:\n", N_general, L, N_specific); 
                // printf("Probability that a call is delayed : %.2f %%\n", result.delay_prob); 
                // printf("Probability that a call is blocked: %.2f %%\n", result.blocking_prob); 
                // printf("Average delay of the calls: %.2f s\n", result.avg_delay); 
                printf("Histogram of delays:\n");
                for(int i = 0; i < result.num_bins_delays; i++){
                    double low = i * result.bin_width_delays;
                    double high = low + result.bin_width_delays;
                    printf("[%.1f, %.1f[ s : %d\n", low, high, result.delays_histogram[i]);
                }
                free(result.histogram);
                // printf("Average waiting prediction absolute error: %.2fs\n", result.avg_abs_err);
                // printf("Average waiting prediction relative error: %.2f\n", result.avg_rel_err);
                // printf("Histogram of prediction error:\n");
                // for(int i = 0; i < result.num_bins; i++){
                //     double low = i * result.bin_width;
                //     double high = low + result.bin_width;
                //     printf("[%.1f, %.1f[ s : %d\n", low, high, result.histogram[i]);
                // }
                // free(result.histogram);
                // printf("Average total time: %.2f s\n", result.avg_total_time);
                printf("\n");
            }
        } 
    } 

    // Best case:
    // int best_Ng = -1, best_L = -1, best_Ns = -1;
    // EC_Metrics best = {0};
    // double best_score = 100;  
    // int Ng_max = 8;
    // int Ns_max = 8;

    // for(int Ng = 1; Ng <= Ng_max; Ng++){
    //     for(int Ns = 1; Ns <= Ns_max; Ns++){
    //         for(int L = 1; L <= 15; L++){

    //             EC_Metrics m = run_avg(lambda, Ng, Ns, L, 3); // 5 runs

    //             if (m.delay_prob <= MAX_DELAY && m.blocking_prob <= MAX_BLOCK &&  m.avg_delay <= MAX_AVG_DELAY && m.avg_total_time <= MAX_TOTAL_TIME) {
    //                 // normalized score:
    //                 double score = 0.2 * (m.blocking_prob / MAX_BLOCK) + 0.2 * (m.avg_delay / MAX_AVG_DELAY) + 0.1 * (m.avg_total_time / MAX_TOTAL_TIME) + 0.5 * (Ng + Ns)/(Ng_max + Ns_max); 

    //                 if (score < best_score){
    //                     best_score = score;
    //                     best_Ng = Ng;
    //                     best_L = L;
    //                     best_Ns = Ns;
    //                     best = m;
    //                 }
    //             }
    //         }
    //     }
    // }

    // printf("====== Best combination ======\n");
    // printf("General operators: %d\n", best_Ng);
    // printf("General queue length: %d\n", best_L);
    // printf("Area-specific operators: %d\n\n", best_Ns);

    // printf("Delay probability = %.2f%% (<= %.2f%%)\n",      best.delay_prob, MAX_DELAY);
    // printf("Blocking probability = %.2f%% (<= %.2f%%)\n",   best.blocking_prob, MAX_BLOCK);
    // printf("Average delay of calls = %.2fs (<= %.2fs)\n",        best.avg_delay, MAX_AVG_DELAY);
    // printf("Average total delay = %.2fs (<= %.2fs)\n",     best.avg_total_time, MAX_TOTAL_TIME);
    

    // return 0;
}

