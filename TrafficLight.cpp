#include "TrafficLight.h"

TrafficLight::TrafficLight() {
    currentState = RED;
}

void TrafficLight::updateLight(int queueLength, bool emergencyVehiclePresent) {
    // The core logic requested by the syllabus
    if (queueLength > 5 || emergencyVehiclePresent) {
        currentState = GREEN;
    }
    else {
        currentState = RED;
    }
}

LightState TrafficLight::getState() const {
    return currentState;
}

const char* TrafficLight::getStateString() const {
    return (currentState == GREEN) ? "GREEN" : "RED";
}