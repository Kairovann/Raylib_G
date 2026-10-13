#pragma once
#include "Math.h"
#include <vector>

struct Waypoint {
    Vec2 pos;
    float width; // track width at this point
};

struct OffTrackResult {
    float distance = 0.0f;   // perpendicular distance from centerline
    float penalty = 0.0f;    // max(0, distance - width/2)
    int segmentIndex = 0;    // nearest segment start index
    float segmentT = 0.0f;   // 0..1 position along that segment
};

struct Bounds {
    Vec2 min;
    Vec2 max;
};

class Track {
public:
    std::vector<Waypoint> waypoints; // closed loop, last connects back to first

    void AddWaypoint(Vec2 pos, float width);

    OffTrackResult GetOffTrackInfo(Vec2 p) const;
    float GetArcLengthProgress(int segmentIndex, float segmentT) const;
    float TotalLength() const;
    Vec2 GetSegmentDirection(int segmentIndex) const; // normalized direction of travel along a segment

    // axis-aligned box around the whole track, padded by margin - a hard play-area limit
    Bounds GetBounds(float margin) const;

private:
    std::vector<float> cumulativeLength;
    void RebuildCumulativeLength();
};
