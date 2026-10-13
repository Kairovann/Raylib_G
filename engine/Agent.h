#pragma once
#include "Math.h"
 
struct AgentConfig {
    float acceleration = 300.0f;   // units/sec^2 from full throttle
    float maxSpeed = 400.0f;       // forward top speed
    float maxReverseSpeed = 150.0f;
    float friction = 150.0f;       // deceleration when coasting, units/sec^2
    float maxSteerRate = 3.0f;     // radians/sec at full speed
};
 
class Agent {
public:
    Vec2 position = {0.0f, 0.0f};
    float heading = 0.0f;  // radians
    float speed = 0.0f;    // scalar along heading, negative = reverse
 
    AgentConfig config;
 
    // steer: -1 (left) to 1 (right), throttle: -1 (reverse) to 1 (forward)
    void Update(float dt, float steer, float throttle);
 
    Vec2 GetVelocity() const;
};
 

