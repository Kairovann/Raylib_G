#include "raylib.h"
#include "../engine/Track.h"
#include "../engine/Agent.h"
#include "Renderer.h"
#include "../engine/TrackPresets.h"
#include <algorithm>

struct TrackEntry {
    const char* name;
    Track (*fn)();
};

static const TrackEntry TRACK_LIST[] = {
    {"Circle",           TrackPresets::Circle},
    {"Oval",             TrackPresets::Oval},
    {"Rounded square",   TrackPresets::RoundedSquare},
    {"Wavy loop",        TrackPresets::WavyLoop},
    {"Hexagon",          TrackPresets::Hexagon},
    {"Teardrop",         TrackPresets::Teardrop},
    {"Narrow serpentine",TrackPresets::NarrowSerpentine},
    {"Gentle star",      TrackPresets::GentleStar},
    {"Kidney",           TrackPresets::Kidney},
    {"Peanut",           TrackPresets::Peanut},
    {"Cross",            TrackPresets::Cross},
    {"Tri-lobe",         TrackPresets::TriLobe},
    {"Pentagon",         TrackPresets::Pentagon},
    {"Octagon",          TrackPresets::Octagon},
    {"Diamond",          TrackPresets::Diamond},
    {"Four-lobe clover", TrackPresets::FourLobeClover},
    {"Zig-zag loop",     TrackPresets::ZigZagLoop},
    {"Asymmetric egg",   TrackPresets::AsymmetricEgg},
    {"Double bump",      TrackPresets::DoubleBump},
    {"Triple wave",      TrackPresets::TripleWave},
    {"Blob",             TrackPresets::Blob},
    {"Spiky safe",       TrackPresets::SpikySafe},
    {"Chicane wide",     TrackPresets::ChicaneWide},
    {"Dumbbell",         TrackPresets::Dumbbell},
    {"Amoeba",           TrackPresets::Amoeba},
};
static const int TRACK_COUNT = sizeof(TRACK_LIST) / sizeof(TRACK_LIST[0]);

int main() {
    const int screenW = 1350, screenH = 600;
    InitWindow(screenW, screenH, "Agent Playable Test");
    SetTargetFPS(60);

    int currentTrackIndex = 3; // WavyLoop by default
    Track track = TRACK_LIST[currentTrackIndex].fn();
    Bounds bounds = track.GetBounds(80.0f); // hard play-area limit, 80px beyond the track
    bounds.min.x = std::max(bounds.min.x, 0.0f);
    bounds.min.y = std::max(bounds.min.y, 0.0f);
    bounds.max.x = std::min(bounds.max.x, (float)screenW);
    bounds.max.y = std::min(bounds.max.y, (float)screenH); // clamp to the visible window - otherwise top/bottom can sit off-screen

    Agent agent;
    agent.position = track.waypoints[0].pos;
    agent.heading = 0.0f;

    float cumulativeReward = 0.0f;
    const float penaltyWeight = 0.5f;
    const float progressWeight = 1.0f;
    float lastProgress = 0.0f;
    bool progressInit = false;

    // track picker panel layout - 3 columns so 25 tracks fit vertically
    const int panelX = 850;
    const int buttonW = 140;
    const int buttonH = 34;
    const int buttonGap = 6;
    const int colGap = 8;
    const int panelTop = 50;
    const int rowsPerCol = 9;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        float steer = 0.0f;
        float throttle = 0.0f;
        if (IsKeyDown(KEY_LEFT)) steer -= 1.0f;
        if (IsKeyDown(KEY_RIGHT)) steer += 1.0f;
        if (IsKeyDown(KEY_UP)) throttle += 1.0f;
        if (IsKeyDown(KEY_DOWN)) throttle -= 1.0f;
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        // handle track picker clicks
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < TRACK_COUNT; i++) {
                int col = i / rowsPerCol;
                int row = i % rowsPerCol;
                Rectangle btn = { (float)(panelX + col * (buttonW + colGap)),
                                   (float)(panelTop + row * (buttonH + buttonGap)),
                                   (float)buttonW, (float)buttonH };
                if (CheckCollisionPointRec(mouse, btn)) {
                    currentTrackIndex = i;
                    track = TRACK_LIST[currentTrackIndex].fn();
                    bounds = track.GetBounds(80.0f);
                    bounds.min.x = std::max(bounds.min.x, 0.0f);
                    bounds.min.y = std::max(bounds.min.y, 0.0f);
                    bounds.max.x = std::min(bounds.max.x, (float)screenW);
                    bounds.max.y = std::min(bounds.max.y, (float)screenH);
                    agent.position = track.waypoints[0].pos;
                    agent.heading = 0.0f;
                    agent.speed = 0.0f;
                    cumulativeReward = 0.0f;
                    progressInit = false;
                    break;
                }
            }
        }

        agent.Update(dt, steer, throttle);

        // hard boundary - agent cannot leave the play area
        bool hitBoundary = false;
        if (agent.position.x < bounds.min.x) { agent.position.x = bounds.min.x; hitBoundary = true; }
        if (agent.position.x > bounds.max.x) { agent.position.x = bounds.max.x; hitBoundary = true; }
        if (agent.position.y < bounds.min.y) { agent.position.y = bounds.min.y; hitBoundary = true; }
        if (agent.position.y > bounds.max.y) { agent.position.y = bounds.max.y; hitBoundary = true; }
        if (hitBoundary) agent.speed = 0.0f;

        OffTrackResult info = track.GetOffTrackInfo(agent.position);
        float progress = track.GetArcLengthProgress(info.segmentIndex, info.segmentT);

        if (!progressInit) { lastProgress = progress; progressInit = true; }
        float progressDelta = progress - lastProgress;
        float total = track.TotalLength();
        if (progressDelta < -total * 0.5f) progressDelta += total;
        if (progressDelta > total * 0.5f) progressDelta -= total;
        lastProgress = progress;

        float reward = progressWeight * progressDelta - penaltyWeight * info.penalty;
        cumulativeReward += reward;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Renderer::DrawTrack(track);
        Renderer::DrawBounds(bounds);
        Renderer::DrawAgent(agent, info.penalty > 0.0f);

        DrawText(TextFormat("Speed: %.1f", agent.speed), 10, 10, 18, BLACK);
        DrawText(TextFormat("Off-track distance: %.2f", info.distance), 10, 32, 18, BLACK);
        DrawText(TextFormat("Penalty this frame: %.2f", info.penalty), 10, 54, 18, BLACK);
        DrawText(TextFormat("Progress: %.2f / %.2f", progress, total), 10, 76, 18, BLACK);
        DrawText(TextFormat("Cumulative reward: %.2f", cumulativeReward), 10, 98, 18, BLACK);
        DrawText("UP/DOWN throttle, LEFT/RIGHT steer, F11 fullscreen", 10, 120, 18, GRAY);

        // track picker panel
        DrawText("Tracks", panelX, 20, 18, BLACK);
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < TRACK_COUNT; i++) {
            int col = i / rowsPerCol;
            int row = i % rowsPerCol;
            Rectangle btn = { (float)(panelX + col * (buttonW + colGap)),
                               (float)(panelTop + row * (buttonH + buttonGap)),
                               (float)buttonW, (float)buttonH };
            bool active = (i == currentTrackIndex);
            bool hovered = CheckCollisionPointRec(mouse, btn);

            Color fill = active ? SKYBLUE : (hovered ? LIGHTGRAY : GRAY);
            DrawRectangleRec(btn, fill);
            DrawRectangleLinesEx(btn, 1.0f, DARKGRAY);
            DrawText(TRACK_LIST[i].name, (int)btn.x + 6, (int)btn.y + 8, 13, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
