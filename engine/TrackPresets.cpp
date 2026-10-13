#include "TrackPresets.h"
#include <cmath>

static const float PI = 3.14159265f;
static const Vec2 CENTER = {450.0f, 300.0f};
static const int N = 20; // waypoints per track - enough for smooth curves

// r(theta) generates a point on the loop. Any r(theta) > 0 for all theta
// is guaranteed non-self-intersecting - no crossing checks needed.

Track TrackPresets::Circle() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f;
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 80.0f);
    }
    return t;
}

Track TrackPresets::Oval() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        t.AddWaypoint({CENTER.x + cosf(a) * 300.0f, CENTER.y + sinf(a) * 180.0f}, 80.0f);
    }
    return t;
}

Track TrackPresets::RoundedSquare() {
    Track t;
    const float R = 260.0f;
    const float n = 4.0f; // squircle exponent - higher = sharper corners
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float denom = powf(powf(fabsf(cosf(a)), n) + powf(fabsf(sinf(a)), n), 1.0f / n);
        float r = R / denom;
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 75.0f);
    }
    return t;
}

Track TrackPresets::WavyLoop() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 40.0f * sinf(5.0f * a); // 5 gentle bumps
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 70.0f);
    }
    return t;
}

Track TrackPresets::Hexagon() {
    Track t;
    const int sides = 6;
    for (int i = 0; i < sides; i++) {
        float a = i * (2.0f * PI / sides);
        t.AddWaypoint({CENTER.x + cosf(a) * 260.0f, CENTER.y + sinf(a) * 260.0f}, 80.0f);
    }
    return t;
}

Track TrackPresets::Teardrop() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 220.0f * (1.0f - 0.45f * cosf(a)); // dimpled limacon, a > b so no self-intersect
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 75.0f);
    }
    return t;
}

Track TrackPresets::NarrowSerpentine() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 35.0f * sinf(8.0f * a); // tighter, more frequent curves
        float width = 55.0f + 20.0f * sinf(8.0f * a); // width pinches through the curves
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, width);
    }
    return t;
}

Track TrackPresets::GentleStar() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 30.0f * cosf(5.0f * a); // 5-point star, mild amplitude keeps it safe
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 70.0f);
    }
    return t;
}

Track TrackPresets::Kidney() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 240.0f + 50.0f * sinf(a) + 25.0f * cosf(2.0f * a); // asymmetric bean shape
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 75.0f);
    }
    return t;
}

Track TrackPresets::Peanut() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 260.0f * (1.0f - 0.3f * cosf(2.0f * a)); // waisted middle, never crosses
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r * 0.7f}, 65.0f);
    }
    return t;
}

Track TrackPresets::Cross() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 240.0f + 55.0f * cosf(4.0f * a); // 4-point cross/star
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 65.0f);
    }
    return t;
}

Track TrackPresets::TriLobe() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 45.0f * cosf(3.0f * a); // 3-lobe Reuleaux-ish shape
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 70.0f);
    }
    return t;
}

Track TrackPresets::Pentagon() {
    Track t;
    const int sides = 5;
    for (int i = 0; i < sides; i++) {
        float a = i * (2.0f * PI / sides);
        t.AddWaypoint({CENTER.x + cosf(a) * 260.0f, CENTER.y + sinf(a) * 260.0f}, 80.0f);
    }
    return t;
}

Track TrackPresets::Octagon() {
    Track t;
    const int sides = 8;
    for (int i = 0; i < sides; i++) {
        float a = i * (2.0f * PI / sides);
        t.AddWaypoint({CENTER.x + cosf(a) * 260.0f, CENTER.y + sinf(a) * 260.0f}, 80.0f);
    }
    return t;
}

Track TrackPresets::Diamond() {
    Track t;
    const int sides = 4;
    for (int i = 0; i < sides; i++) {
        float a = i * (2.0f * PI / sides) + PI / 4.0f; // rotated to point up/down/left/right
        t.AddWaypoint({CENTER.x + cosf(a) * 280.0f, CENTER.y + sinf(a) * 280.0f}, 80.0f);
    }
    return t;
}

Track TrackPresets::FourLobeClover() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 220.0f + 90.0f * cosf(4.0f * a); // deeper lobes than Cross
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 60.0f);
    }
    return t;
}

Track TrackPresets::ZigZagLoop() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 20.0f * sinf(12.0f * a); // high-frequency small switchbacks
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 60.0f);
    }
    return t;
}

Track TrackPresets::AsymmetricEgg() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 240.0f * (1.0f - 0.3f * cosf(a) + 0.12f * sinf(2.0f * a));
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 75.0f);
    }
    return t;
}

Track TrackPresets::DoubleBump() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 240.0f + 60.0f * cosf(2.0f * a); // two wide bumps
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 75.0f);
    }
    return t;
}

Track TrackPresets::TripleWave() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 30.0f * sinf(9.0f * a); // tighter, more frequent than TriLobe
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 65.0f);
    }
    return t;
}

Track TrackPresets::Blob() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 230.0f + 35.0f * sinf(a) + 25.0f * cosf(3.0f * a) + 15.0f * sinf(5.0f * a);
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 70.0f);
    }
    return t;
}

Track TrackPresets::SpikySafe() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 250.0f + 40.0f * cosf(7.0f * a); // 7-point star, amplitude kept below base
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 60.0f);
    }
    return t;
}

Track TrackPresets::ChicaneWide() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 260.0f + 45.0f * sinf(6.0f * a);
        float width = 70.0f + 35.0f * sinf(6.0f * a); // exaggerated width swings
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, width);
    }
    return t;
}

Track TrackPresets::Dumbbell() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 260.0f * (1.0f - 0.42f * cosf(2.0f * a)); // tighter waist than Peanut
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r * 0.65f}, 60.0f);
    }
    return t;
}

Track TrackPresets::Amoeba() {
    Track t;
    for (int i = 0; i < N; i++) {
        float a = i * (2.0f * PI / N);
        float r = 235.0f + 20.0f * cosf(2.0f * a) + 18.0f * sinf(4.0f * a) + 10.0f * cosf(6.0f * a);
        t.AddWaypoint({CENTER.x + cosf(a) * r, CENTER.y + sinf(a) * r}, 68.0f);
    }
    return t;
}
