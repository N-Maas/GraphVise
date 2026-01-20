#ifndef THESIS_FRAMEWORK_GROUP_HPP
#define THESIS_FRAMEWORK_GROUP_HPP
#include <string>
#include <utility>
#include "imgui/imgui.h"


class Group {
    public:
        Group(const std::uint32_t  groupID, std::string  name, const ImVec4& groupVec4)
            : groupID(groupID), name(std::move(name)), groupVec4(groupVec4) {}

        [[nodiscard]] const std::string& getName() const;
        [[nodiscard]] std::uint32_t  getGroupID() const;
        void setName(const std::string& name);
        bool setTransparency(float transparency);
        [[nodiscard]] float getTransparency() const;
        void setGroupVec4(const ImVec4& groupVec4);
        [[nodiscard]] const ImVec4& getGroupVec4() const;

        Group& operator=(const Group& other) {
            if (this == &other) return *this;
            name = other.name;
            groupVec4 = other.groupVec4;
            return *this;
        }

    private:
        const std::uint32_t  groupID;
        std::string name;
        ImVec4 groupVec4;
};


#endif