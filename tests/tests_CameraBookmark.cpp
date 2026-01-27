#include <glm/vec3.hpp>
#include <gtest/gtest.h>
#include "../src/model/CameraBookmark.hpp"

TEST(CameraBookmarkTest, CheckGetter) {
    CameraBookmark cameraBookmark = CameraBookmark(
        0,
        "TestBookMark",
        glm::vec3(0.0f, 1.0f, 0.5f),
        1.0f,
        2.0f);

    EXPECT_EQ(cameraBookmark.getCameraBookmarkID(), 0);
    EXPECT_STREQ(cameraBookmark.getName().c_str(), "TestBookMark");
    EXPECT_NEAR(cameraBookmark.getCoordsVector().x, 0.0f, 0.0f);
    EXPECT_NEAR(cameraBookmark.getCoordsVector().y, 1.0f, 0.0f);
    EXPECT_NEAR(cameraBookmark.getCoordsVector().z, 0.5f, 0.0f);
    EXPECT_NEAR(cameraBookmark.getPitch(), 1.0f, 0.0f);
    EXPECT_NEAR(cameraBookmark.getYaw(), 2.0f, 0.0f);

    CameraBookmark cameraBookmark2 = CameraBookmark(
        14,
        "TestBookMark2",
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.4f,
        0.091f);

    EXPECT_EQ(cameraBookmark2.getCameraBookmarkID(), 14);
    EXPECT_STREQ(cameraBookmark2.getName().c_str(), "TestBookMark2");
    EXPECT_NEAR(cameraBookmark2.getCoordsVector().x, 0.0f, 0.0f);
    EXPECT_NEAR(cameraBookmark2.getCoordsVector().y, 0.0f, 0.0f);
    EXPECT_NEAR(cameraBookmark2.getCoordsVector().z, 0.0f, 0.0f);
    EXPECT_NEAR(cameraBookmark2.getPitch(), 1.4f, 0.0f);
    EXPECT_NEAR(cameraBookmark2.getYaw(), 0.091f, 0.0f);
}

TEST(CameraBookmarkTest, SetNameTest) {
    CameraBookmark cameraBookmark = CameraBookmark(
        0,
        "TestBookMark",
        glm::vec3(0.0f, 1.0f, 0.5f),
        1.0f,
        2.0f);

    EXPECT_STREQ(cameraBookmark.getName().c_str(), "TestBookMark");
    cameraBookmark.setName("TestBookMark3");
    EXPECT_STREQ(cameraBookmark.getName().c_str(), "TestBookMark3");

    CameraBookmark cameraBookmark2 = CameraBookmark(
        14,
        "TestBookMark2",
        glm::vec3(0.0f, 0.0f, 0.0f),
        1.4f,
        0.091f);

    EXPECT_STREQ(cameraBookmark2.getName().c_str(), "TestBookMark2");
    cameraBookmark2.setName("TestBookMark5");
    EXPECT_STREQ(cameraBookmark2.getName().c_str(), "TestBookMark5");
}