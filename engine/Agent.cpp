#include "Agent.h"
#include <algorithm>
#include <cmath>


void Agent::Update(float dt, float steer, float throttle) {
    // clamp inputs
    steer = std::clamp(steer, -1.0f, 1.0f);
    throttle = std::clamp(throttle, -1.0f, 1.0f);

    // throttle accelerates/decelerates speed
    speed += throttle * config.acceleration * dt;

    // friction pulls speed toward zero when not fully throttling
    if (std::fabs(throttle) < 0.01f) {
        float frictionStep = config.friction * dt;
        if (speed > 0.0f) speed = std::max(0.0f, speed - frictionStep);
        else if (speed < 0.0f) speed = std::min(0.0f, speed + frictionStep);
    }

    speed = std::clamp(speed, -config.maxReverseSpeed, config.maxSpeed);

    // steering scaled by speed fraction - no turning in place, full turn rate near top speed
    float speedFrac = std::fabs(speed) / config.maxSpeed;
    float turnRate = steer * config.maxSteerRate * speedFrac;

    // reverse inverts steering feel (like a real car backing up)
    if (speed < 0.0f) turnRate = -turnRate;

    heading += turnRate * dt;

    position.x += cosf(heading) * speed * dt;
    position.y += sinf(heading) * speed * dt;
}

Vec2 Agent::GetVelocity() const {
    return {cosf(heading) * speed, sinf(heading) * speed};
}
