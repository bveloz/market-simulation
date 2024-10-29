#ifndef TIME_H
#define TIME_H

#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59

typedef struct Time Time;


Time* initialize_time_empty();
Time* initialize_time(int, int, int);

#endif
