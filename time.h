#ifndef TIME_H
#define TIME_H

#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59

typedef struct Time Time;

enum time_type {HOUR, MINUTE, SECOND};

Time* initialize_default_time();
Time* initialize_time(int, int, int);
void increment_time(Time*, int, enum time_type);


#endif
