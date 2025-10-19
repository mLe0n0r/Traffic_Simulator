#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"
#include "generate_events.h"

#define CHEGADA 0
#define PARTIDA 1
#define num_events 100000
double lambda = 200;
double dm = 0.008;
double Ax = 0.008;

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

EC_Metrics general_case(int lambda, double dm, int N, int max_samps, double Ax, int L){
    int busy = 0;
	int arrivals = 0;

	int delayed = 0;
    int delayed_gt_Ax = 0;
    int blocked = 0;

    double sum_delay = 0.0;
    double delay = 0.0;

    double ev_time = 0.0;
    int in_queue = 0;

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
                if(in_queue < L){
                    delayed++;
                    queue = __add_queue(queue, ev_time); // we store the time the event entered the queue
                    in_queue++;
                }
                else blocked++;
            }
            // in both cases we generate the next arrival
            events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list);
        }

        else{ // Partida
            if(queue != NULL){ // if the queue is not empty the service continues as busy
                double ev_arrival;
                queue = __remove_queue(queue, &ev_arrival); // we are going to obtain the time the event entered the queue
                events_list = generate_events(lambda, dm, PARTIDA, ev_time, events_list); // now that the event got out the queue we can generate a new departure
                
                if (in_queue > 0) in_queue--;

                delay = ev_time - ev_arrival;

                // build the histogram:
                int i = (int)((delay)/delta);
                if (i >= intervals) i = intervals -1;
                histogram[i]++;

                // for the average delay calculation:
                sum_delay += delay;
                if (delay > Ax) delayed_gt_Ax++;
            }
            else{
                busy--; 
                in_queue = 0;
            }
            
        }
    }

    EC_Metrics results;
    results.delay_prob = ((double)delayed / (double)arrivals) * 100;
    results.blocking_prob = ((double)blocked / (double)arrivals) * 100;
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

    // Confirming that the general case with length = 0 is equal to the erlang B:
    printf("Blocking Probability with L = 0:\n");
    for(int N = 1; N < 11; N++){
        EC_Metrics result = general_case(lambda, dm, N, num_events, Ax, 0);
        printf("%d services: %.2f %%\n", N, result.blocking_prob);
    }

    // Confirming that the general case with length = ∞ is equal to the erlang C:
    printf("Results with L = 100000:\n");
    for(int N = 1; N < 11; N++){
        EC_Metrics result = general_case(lambda, dm, N, num_events, Ax, 100000);
        printf("Metric when having %d services:\n", N);
        printf("Delay probability: %.2f %%\n", result.delay_prob);

        if(N == 1) printf("Average delay (Am): %f ms\n", result.avg_delay);
        else printf("Average delay (Am): %f us\n", result.avg_delay * 1000);

        printf("Probability of A >  %.3f: %.2f %%\n", Ax, result.p_delay_gt_Ax);
        printf("\n");
    }

    // General case for different queue lengths:
    for (int length = 1; length < 38; length += 2){
        EC_Metrics result = general_case(lambda, dm, 1, num_events, Ax, length);
        printf("Results for a queue of length = %d\n", length);
        printf("Delay probability: %.2f %%\n", result.delay_prob);
        printf("Blocking Probability: %.2f %%\n", result.blocking_prob);
        printf("Probability of A > %.3f: %.2f %%\n", Ax, result.p_delay_gt_Ax);
        printf("Average delay (Am): %f ms\n", result.avg_delay);
        printf("\n");
    }

    // Detect the necessary length to obtain 1% of blocking probability:
    for (int N = 2; N < 7; N++){
        int length = 0;
        EC_Metrics result = general_case(lambda, dm, N, num_events, Ax, length);
        while(result.blocking_prob > 1){
            length++;
            result = general_case(lambda, dm, N, num_events, Ax, length);
        }

        printf("%d services:\n", N);
        printf("Queue length: %d\n", length);
        printf("Blocking Probability: %.2f%%\n", result.blocking_prob);
        printf("\n");
    }

}