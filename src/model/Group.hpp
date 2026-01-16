//
// Created by jannis on 1/15/26.
//

#ifndef THESIS_FRAMEWORK_GROUP_HPP
#define THESIS_FRAMEWORK_GROUP_HPP
#include <string>
#include "imgui/imgui.h"


class Group {
    public:
        Group(int groupID, const std::string& name, const ImVec4& groupVec4);
        [[nodiscard]] const std::string& getName() const;
        void setName(const std::string& name);
        bool setTransparency(float transparency);
        [[nodiscard]] float getTransparency() const;
        void setGroupVec4(const ImVec4& groupVec4);
        [[nodiscard]] const ImVec4& getGroupVec4() const;
        void deleteGroup();

    private:
        const int groupID;
        std::string name;
        ImVec4 groupVec4;
};


#endif //THESIS_FRAMEWORK_GROUP_HPP