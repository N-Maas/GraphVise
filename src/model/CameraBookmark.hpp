#ifndef THESIS_FRAMEWORK_CAMERABOOKMARK_HPP
#define THESIS_FRAMEWORK_CAMERABOOKMARK_HPP
#include <string>
#include <glm/vec3.hpp>
#include <utility>


class CameraBookmark {
    public:
        CameraBookmark(const std::uint32_t  cameraBookmarkID, std::string name, const glm::vec3& coords, const float pitch, const float yaw)
            : cameraBookmarkID(cameraBookmarkID), name(std::move(name)),
            coordsVector(coords), pitch(pitch), yaw(yaw) {}

        [[nodiscard]] std::uint32_t  getCameraBookmarkID() const;
        [[nodiscard]] const std::string& getName() const;
        void setName(const std::string& name);
        [[nodiscard]] const glm::vec3& getCoordsVector() const;
        [[nodiscard]] float getPitch() const;
        [[nodiscard]] float getYaw() const;
        void deleteCameraBookmark();

        CameraBookmark& operator=(const CameraBookmark& other) {
            if (this == &other) return *this;
            name = other.name;
            return *this;
        }

    private:
        const std::uint32_t  cameraBookmarkID;
        std::string name;
        const glm::vec3 coordsVector;
        const float pitch;
        const float yaw;

};


#endif