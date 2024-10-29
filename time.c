#include "time.h"
#include <stdio.h>
#include <stdlib.h>

// Will be used to keep track of the passage of time within the simulation
struct Time
{
    int* hours;
    int* minutes;
    int* seconds;
};

// Initialize empty time
Time* initialize_time_empty()
{
    Time* newTime = (Time*)malloc(sizeof(Time));
    newTime->hours = (int*)malloc(sizeof(int));
    newTime->minutes = (int*)malloc(sizeof(int));
    newTime->seconds = (int*)malloc(sizeof(int));
    return newTime;
}

Time* initialize_time(int h, int m, int s)
{
    Time* newTime = initialize_time_empty();
    *newTime->hours = h;
    *newTime->minutes = m;
    *newTime->seconds = s;
    return newTime;
}

void free_time(Time* victim)
{
    //avoid double frees
    if (victim != NULL)
    {
        if (victim->hours != NULL)
        {
            free(victim->hours);
            victim->hours = NULL;
        }
        if (victim->minutes != NULL)
        {
            free(victim->minutes);
            victim->minutes = NULL;
        }
        if (victim->seconds != NULL)
        {
            free(victim->seconds);
            victim->seconds = NULL;
        }
    }    
}