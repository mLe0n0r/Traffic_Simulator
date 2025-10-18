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
    double bin_width; 
    int num_bins; 
    int *histogram; // pointer for the histogram
} EC_Metrics;

// ----------------- Functions to calculate the teoretical Am -----------------
double calculate_factorial(int N) {
    double result = 1.0;
    for (int i = 1; i <= N; i++) {
        result *= i;
    }
    return result;
}

double calculate_p_delay(double lambda, double dm, int N){
    double A = lambda * dm;
    double numerator = (pow(A, N) / calculate_factorial(N)) * (N / (N - A));
    double denominator = 0.0;

    for (int k = 0; k < N; k++) {
        denominator += (pow(A, k) / calculate_factorial(k));
    }
    denominator += numerator;

    return (double)(numerator / denominator);
}

double calculate_p_block(double lambda, double dm, int N) {
    double A = lambda * dm;
    double numerator = (double)(pow(A, N) / calculate_factorial(N));
    double denominator = 0.0;

    for (int k = 0; k < N; k++) {
        denominator += (double)(pow(A, k) / calculate_factorial(k));
    }
    denominator += numerator;

    return (double)(numerator / denominator);
}

double calculate_am(double p_delay, double lambda, double p_block) {
    return (double)(p_delay / (lambda * (1 - p_block)));
}

// ------------------------------------------------------

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

    // ------------------- Histogram --------------------
    double p_delay = calculate_p_delay(lambda, dm, N);
    double p_block = calculate_p_block(lambda, dm, N);
    double max;
    if(N == 1) max = 3 * 60; // for N = 1 its not possible to estimate Am due to instability
    else max = 30 * calculate_am(p_delay, lambda, p_block); // this way the range of delays is better represented for each case 
	int intervals = 25;
    double delta = (double)(max/intervals);

    int *histogram = malloc(intervals * sizeof(int));
    for (int k = 0; k < intervals; k++) histogram[k] = 0;
    // ---------------------------------------------------

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

                // build the histogram:
                int i = (int)((delay)/delta);
                if (i >= intervals) i = intervals -1;
                histogram[i]++;

                // for the average delay calculation:
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

    // histogram:
    results.bin_width = delta;
    results.num_bins = intervals;
    results.histogram = histogram;

    return results;
}

int main(void){
    srand(time(NULL));

    for(int N = 1; N < 11; N++){
        EC_Metrics result = erlang_C(200, 0.008, N, 100000, 0.008);
        printf("Metric when having %d services:\n", N);
        printf("Delay probability: %.2f %%\n", result.delay_prob);

        if(N == 1) printf("Average delay (Am): %f ms\n", result.avg_delay);
        else printf("Average delay (Am): %f us\n", result.avg_delay * 1000);

        printf("Probability of A > Ax: %.2f %%\n", result.p_delay_gt_Ax);
        if(N < 7){
            printf("Histogram of delays:\n");
            if(N == 1) {
                for(int i = 0; i < result.num_bins; i++){
                    double low = i * result.bin_width;
                    double high = low + result.bin_width;
                    printf("[%.3f, %.3f[ ms : %d\n", low, high, result.histogram[i]);
                }
            }
            else{
                for(int i = 0; i < result.num_bins; i++){
                double low = i * result.bin_width * 1000;
                double high = low + result.bin_width;
                printf("[%.3f, %.3f[ us : %d\n", low, high, result.histogram[i]);
            }
            }
            
        }
        
        printf("\n");
    }   
}