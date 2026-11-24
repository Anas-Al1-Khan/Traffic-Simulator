
#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <vector>
#include "ManualCircularQueue.h"

struct Road {
    int id;
    double x1, y1;
    double x2, y2;
};

struct Intersection {
    int id;
    double x, y;
    QString label;

    ManualCircularQueue<int> waitingCars;

    Intersection(int id, double x, double y, QString label)
        : id(id), x(x), y(y), label(label), waitingCars(10) {}
};

#endif
