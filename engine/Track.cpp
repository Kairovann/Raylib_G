#include "Track.h"
#include <limits>
#include <algorithm>

void Track::AddWaypoint(Vec2 pos, float width) {
    waypoints.push_back({pos, width});
    RebuildCumulativeLength();
}

void Track::RebuildCumulativeLength() {
    cumulativeLength.clear();
    if (waypoints.empty()) return;
    cumulativeLength.push_back(0.0f);
    for (size_t i = 1; i < waypoints.size(); i++) {
        float segLen = Distance(waypoints[i - 1].pos, waypoints[i].pos);
        cumulativeLength.push_back(cumulativeLength.back() + segLen);
    }
}

float Track::TotalLength() const {
    if (waypoints.size() < 2) return 0.0f;
    float closingSeg = Distance(waypoints.back().pos, waypoints.front().pos);
    return cumulativeLength.back() + closingSeg;
}

static float ClosestPointOnSegmentT(Vec2 a, Vec2 b, Vec2 p) {
    Vec2 ab = b - a;
    float abLenSq = LengthSqr(ab);
    if (abLenSq < 1e-8f) return 0.0f;
    float t = Dot(p - a, ab) / abLenSq;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return t;
}

OffTrackResult Track::GetOffTrackInfo(Vec2 p) const {
    OffTrackResult result;
    result.distance = std::numeric_limits<float>::max();

    size_t n = waypoints.size();
    if (n < 2) return result;

    for (size_t i = 0; i < n; i++) {
        Vec2 a = waypoints[i].pos;
        Vec2 b = waypoints[(i + 1) % n].pos;

        float t = ClosestPointOnSegmentT(a, b, p);
        Vec2 closest = Lerp(a, b, t);
        float dist = Distance(p, closest);

        if (dist < result.distance) {
            result.distance = dist;
            result.segmentIndex = (int)i;
            result.segmentT = t;
        }
    }

    const Waypoint& wa = waypoints[result.segmentIndex];
    const Waypoint& wb = waypoints[(result.segmentIndex + 1) % n];
    float width = wa.width + (wb.width - wa.width) * result.segmentT;

    result.penalty = result.distance - (width * 0.5f);
    if (result.penalty < 0.0f) result.penalty = 0.0f;

    return result;
}

Vec2 Track::GetSegmentDirection(int segmentIndex) const {
    size_t n = waypoints.size();
    if (n < 2) return {0.0f, 0.0f};
    Vec2 a = waypoints[segmentIndex].pos;
    Vec2 b = waypoints[(segmentIndex + 1) % n].pos;
    return Normalize(b - a);
}

Bounds Track::GetBounds(float margin) const {
    Bounds b;
    if (waypoints.empty()) {
        b.min = {0.0f, 0.0f};
        b.max = {0.0f, 0.0f};
        return b;
    }

    b.min = waypoints[0].pos;
    b.max = waypoints[0].pos;
    for (const auto& wp : waypoints) {
        float half = wp.width * 0.5f;
        b.min.x = std::min(b.min.x, wp.pos.x - half);
        b.min.y = std::min(b.min.y, wp.pos.y - half);
        b.max.x = std::max(b.max.x, wp.pos.x + half);
        b.max.y = std::max(b.max.y, wp.pos.y + half);
    }

    b.min.x -= margin; b.min.y -= margin;
    b.max.x += margin; b.max.y += margin;
    return b;
}

float Track::GetArcLengthProgress(int segmentIndex, float segmentT) const {
    if (cumulativeLength.empty()) return 0.0f;
    size_t n = waypoints.size();
    float base = cumulativeLength[segmentIndex];
    float segLen = (segmentIndex + 1 < (int)n)
        ? Distance(waypoints[segmentIndex].pos, waypoints[segmentIndex + 1].pos)
        : Distance(waypoints[segmentIndex].pos, waypoints[0].pos);
    return base + segLen * segmentT;
}
