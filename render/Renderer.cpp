#include "Renderer.h"
#include "raylib.h"

static Vector2 ToRL(Vec2 v) { return {v.x, v.y}; }

void Renderer::DrawTrack(const Track& track) {
    size_t n = track.waypoints.size();
    if (n < 2) return;

    for (size_t i = 0; i < n; i++) {
        Vec2 a = track.waypoints[i].pos;
        Vec2 b = track.waypoints[(i + 1) % n].pos;
        DrawLineV(ToRL(a), ToRL(b), DARKGRAY);

        Vec2 dir = Normalize(b - a);
        Vec2 normal = {-dir.y, dir.x};
        float hw = track.waypoints[i].width * 0.5f;

        Vec2 outerA = a + normal * hw;
        Vec2 outerB = b + normal * hw;
        Vec2 innerA = a - normal * hw;
        Vec2 innerB = b - normal * hw;

        DrawLineV(ToRL(outerA), ToRL(outerB), LIGHTGRAY);
        DrawLineV(ToRL(innerA), ToRL(innerB), LIGHTGRAY);

        DrawCircleV(ToRL(a), 3, RED);
    }
}

void Renderer::DrawAgent(const Agent& agent, bool offTrack) {
    DrawCircleV(ToRL(agent.position), 8, offTrack ? RED : BLUE);
    Vec2 tip = agent.position + Vec2{cosf(agent.heading) * 20, sinf(agent.heading) * 20};
    DrawLineV(ToRL(agent.position), ToRL(tip), BLACK);
}

void Renderer::DrawBounds(const Bounds& bounds) {
    Rectangle rect = {
        bounds.min.x, bounds.min.y,
        bounds.max.x - bounds.min.x, bounds.max.y - bounds.min.y
    };
    DrawRectangleLinesEx(rect, 2.0f, MAROON);
}
