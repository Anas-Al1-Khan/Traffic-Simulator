#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

enum LightState {
    RED,
    GREEN
};

class TrafficLight {
private:
    LightState currentState;

public:
    TrafficLight();
    void updateLight(int queueLength, bool emergencyVehiclePresent);
    LightState getState() const;
    const char* getStateString() const;
};

#endif#pragma once
