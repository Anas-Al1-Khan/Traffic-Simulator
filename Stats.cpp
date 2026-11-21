#include "Stats.h"

double Stats::calculateAverageWaitTime(const int* waitTimes, int count) {
    if (waitTimes == nullptr || count <= 0) {
        return 0.0; // Edge case: avoid division by zero
    }

    long long totalWaitTime = 0;
    for (int i = 0; i < count; ++i) {
        totalWaitTime += waitTimes[i];
    }

    return static_cast<double>(totalWaitTime) / count;
}