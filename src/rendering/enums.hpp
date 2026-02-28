//
// Created by helen on 2/6/26.
//

#pragma once

namespace graphvise {
    // Define ALL enums here - this is the single source of truth
    // to ensure no cyclical dependencies occur on account of the enums
    enum class CameraFocusMode {
        CENTER_OF_MASS,
        FREE,
        ORIGIN,
        FOCUS_MODE_COUNT_LAST_ITEM, // pattern to keep track of the number of items in the enum, for toggling (all manually available options above this pls)
        SELECTED_VERTEX
    };

    enum class LightSourceMovementBehaviour {
        FIXED_POSITION,
        ORBIT_AROUND_GRAPH,
        FOLLOW_CAMERA,
        MOVE_BEHAVIOUR_COUNT_LAST_ITEM // pattern to keep track of the number of items in the enum, for toggling
    };

    enum class PerformanceMode {
        QUALITY,
        BALANCE,
        PERFORMANCE,
        PERFORMANCE_MODE_COUNT_LAST_ITEM // pattern to keep track of the number of items in the enum, for toggling
    };
}