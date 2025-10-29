#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"
#include "generate_events.h"

#define CHEGADA 0
#define PARTIDA 1
#define general 0
#define specific 1
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

double call_duration(int type){
    if(type == 0){ // is a general-purpose call
        double min_duration = 60.0;  // seconds
        double max_duration = 300.0; // max duration = 5 min
        double exp_duration = 0;

        double u = (rand() + 1.0) / (RAND_MAX + 2.0); 
        double x = - 120.0 * log(u);

        double duration = min_duration + x;

        if(duration < max_duration) return duration;
        else return max_duration; 
    }

    else { // is area-specific
        int min_duration = 30;
        int max_duration = 120;

        double mu = 60;
        double sigma = 20;

        double u = (rand() + 1.0) / (RAND_MAX + 2.0);
        double duration = 0.0;

        while (duration < min_duration || duration > max_duration){
            double u1 = (rand() + 1.0) / (RAND_MAX + 2.0); 
            double u2 = (rand() + 1.0) / (RAND_MAX + 2.0); 

            double z = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.14159 * u2);

            duration = mu + sigma * z;
        }
        return duration;
    }
}

EC_Metrics general_purpose(int lambda, double dm, int max_samps, int N_operators, int L){

}