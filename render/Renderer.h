#pragma once
#include "../engine/Track.h"
#include "../engine/Agent.h"


namespace Renderer {
    void DrawTrack(const Track& track);
    void DrawAgent(const Agent& agent, bool offTrack);
    void DrawBounds(const Bounds& bounds);
}
