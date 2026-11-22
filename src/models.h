#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <vector>
#include "ManualCircularQueue.h"

// 1. Road Data Structure (matches the syllabus requirement)
struct Road {
    int id;
    double x1, y1;
    double x2, y2;
};

// 2. Intersection Data Structure
struct Intersection {
    int id;
    double x, y;
    QString label;
    
    // Your custom Day 1 Queue!
    ManualCircularQueue<int> waitingCars; 

    Intersection(int id, double x, double y, QString label) 
        : id(id), x(x), y(y), label(label), waitingCars(10) {} // Max 10 cars

    // Because your ManualCircularQueue implemented the Rule of Three (deep copy),
    // Intersection is safely copyable by std::vector during resizing!
};

#endif // MODELS_H