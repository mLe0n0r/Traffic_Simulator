#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>

double call_duration(int type, int operator_type){
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

    else if(type == 1 && operator_type == 0) { // is area-specific 
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

    else{ // is area-specific and has been answered by a are-specific
        double min_duration = 60.0; 
        double exp_duration = 0;

        double u = (rand() + 1.0) / (RAND_MAX + 2.0); 
        double x = - 150.0 * log(u);

        double duration = min_duration + x;
        return duration;
    }
}
