#include "Edge.hpp"
#include <stdexcept>
#include "GraphSaver.hpp"

namespace graphvise {
    std::uint32_t Edge::getID() const {
        return edgeID;
    }

    std::uint32_t Edge::getConnectedGroupID() const {
        return connectedGroupID;
    }

    void Edge::setGroup(const std::uint32_t groupID) {
        connectedGroupID = groupID;
    }

    std::pair<std::uint32_t, std::uint32_t> Edge::getConnectingVerticesIDs() const {
        return connectingVerticesIDs;
    }

    float Edge::getOwnTransparency() const {
        return ownTransparency.value_or(-1.0f);
    }

    void Edge::setOwnTransparency(float transparency) {
        if (transparency >= 0.0f && transparency <= 1.0f) {
            ownTransparency = transparency;
            return;
        }
        throw std::out_of_range("Transparency must be between 0 and 1");
    }

    void Edge::deleteOwnTransparency() {
        if (ownTransparency.has_value()) {
            ownTransparency.reset();
        }
    }

    void Edge::setLength(float newLength) {
        this->length = newLength;
    }

    float Edge::getLength() const {
        return length;
    }

    void Edge::setMatrix(const glm::mat4& newMatrix) {
        this->matrix = std::move(newMatrix);
    }

    const glm::mat4 &Edge::getMatrix() const {
        return matrix;
    }
}