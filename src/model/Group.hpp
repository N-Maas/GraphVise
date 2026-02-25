#ifndef THESIS_FRAMEWORK_GROUP_HPP
#define THESIS_FRAMEWORK_GROUP_HPP
#include <string>
#include <utility>
#include "imgui/imgui.h"

namespace graphvise
{
    class Group
    {
    public:
        explicit Group(const std::uint32_t groupID, std::string name, const ImVec4& groupVec4)
            : groupID(groupID), name(std::move(name)), groupVec4(groupVec4)
        {
        }
        Group(){}

        [[nodiscard]] const std::string& getName() const;
        void setName(const std::string& name);
        [[nodiscard]] std::uint32_t getID() const;
        void setTransparency(float transparency);
        void setColor(const ImVec4& groupVec4);
        [[nodiscard]] ImVec4 getVec4() const;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & groupID;
            ar & name;
            ar & groupVec4.w;
            ar & groupVec4.x;
            ar & groupVec4.y;
            ar & groupVec4.z;
        }

    private:
        std::uint32_t groupID;
        std::string name;
        ImVec4 groupVec4;
    };
}
#endif
