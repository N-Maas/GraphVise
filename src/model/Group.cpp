#include "Group.hpp"
#include <stdexcept>

namespace graphvise {
    const std::string& Group::getName() const {
        return name;
    }

    std::uint32_t  Group::getGroupID() const {
        return groupID;
    }

    void Group::setName(const std::string& new_name) {
        name = new_name;
    }

    void Group::setTransparency(const float transparency) {
        if (transparency >= 0.0f && transparency <= 1.0f) {
            groupVec4.w = transparency;
            return;
        }
        throw std::out_of_range("Transparency must be between 0 and 1");
    }

    float Group::getTransparency() const {
        return groupVec4.w;
    }

    void Group::setGroupVec4(const ImVec4& new_groupVec4) {
        groupVec4 = new_groupVec4;
    }

    const ImVec4& Group::getGroupVec4() const {
        return groupVec4;
    }
}