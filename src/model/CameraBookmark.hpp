//
// Created by jannis on 1/15/26.
//

#ifndef THESIS_FRAMEWORK_CAMERABOOKMARK_HPP
#define THESIS_FRAMEWORK_CAMERABOOKMARK_HPP
#include <string>
#include <glm/vec3.hpp>


class CameraBookmark {
    public:
        CameraBookmark(int cameraBookmarkID, const std::string& name, const glm::vec3& coords, float pitch, float yaw)
            : cameraBookmarkID(cameraBookmarkID), name(name),
            coordsVector(coords), pitch(pitch), yaw(yaw) {}

        [[nodiscard]] int getCameraBookmarkID() const;
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
        const int cameraBookmarkID;
        std::string name;
        const glm::vec3 coordsVector;
        const float pitch;
        const float yaw;

};


#endif