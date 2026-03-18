#ifndef THESIS_FRAMEWORK_CAMERABOOKMARK_HPP
#define THESIS_FRAMEWORK_CAMERABOOKMARK_HPP
#include <string>
#include <glm/vec3.hpp>
#include <utility>

namespace graphvise {
    class CameraBookmark {
    public:
        explicit CameraBookmark(std::string name, const glm::vec3& coords, const float pitch, const float yaw)
            : name(std::move(name)), coordsVector(coords), pitch(pitch), yaw(yaw) {}
        CameraBookmark(){}

        [[nodiscard]] const std::string& getName() const;
        void setName(const std::string& name);
        [[nodiscard]] glm::vec3 getCoordsVector() const;
        [[nodiscard]] float getPitch() const;
        [[nodiscard]] float getYaw() const;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar & name;
            ar & coordsVector.x;
            ar & coordsVector.y;
            ar & coordsVector.z;
            ar & pitch;
            ar & yaw;
        }

    private:
        std::string name;
        glm::vec3 coordsVector;
        float pitch;
        float yaw;
    };
}
#endif