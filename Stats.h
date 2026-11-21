#ifndef STATS_H
#define STATS_H

class Stats {
public:
    // Calculates average wait time from a raw array of wait times
    static double calculateAverageWaitTime(const int* waitTimes, int count);
};

#endif
