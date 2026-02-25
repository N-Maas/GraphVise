//
// Created by helen on 2/6/26.
//

#pragma once

namespace graphvise {
    // Define ALL enums here - this is the single source of truth
    // to ensure no cyclical dependencies occur on account of the enums
    enum class CameraFocusMode {
        CENTER_OF_MASS,
        ORIGIN,
        SELECTED_VERTEX,
        FREE
    };

    enum class LightSourceMovementBehaviour {
        FIXED_POSITION,
        ORBIT_AROUND_GRAPH,
        FOLLOW_CAMERA
    };

    enum class PerformanceMode {
        QUALITY,
        BALANCE,
        PERFORMANCE
    };
}