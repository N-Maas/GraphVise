#include "Group.hpp"
#include <stdexcept>

namespace graphvise {
    const std::string& Group::getName() const {
        return name;
    }

    std::uint32_t Group::getID() const {
        return groupID;
    }

    void Group::setName(const std::string& new_name) {
        name = new_name;
    }

    void Group::setTransparency(const float transparency) {
        if (transparency >= 0.0f && transparency <= 1.0f) {
            groupVec4.w = transparency;
        }
    }

    void Group::setColor(const ImVec4& new_groupVec4) {
        groupVec4.x = new_groupVec4.x;
        groupVec4.y = new_groupVec4.y;
        groupVec4.z = new_groupVec4.z;
    }

    ImVec4 Group::getVec4() const {
        return groupVec4;
    }
}