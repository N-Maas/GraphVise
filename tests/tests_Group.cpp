#include <gtest/gtest.h>
#include "../src/model/Graph.hpp"
#include "../ext/imgui/imgui.h"

using namespace graphvise;
TEST(GroupTest, CheckGetter) {
    Group group = Group(4,
        "Test-Group",
        ImVec4(255, 99, 5, 0.8));

    EXPECT_STREQ("Test-Group", group.getName().c_str());
    EXPECT_EQ(group.getGroupID(), 4);
    EXPECT_EQ(group.getGroupVec4().x, 255);
    EXPECT_EQ(group.getGroupVec4().y, 99);
    EXPECT_EQ(group.getGroupVec4().z, 5);
    EXPECT_NEAR(group.getGroupVec4().w, 0.8f, 0.000001f);
}

TEST(GroupTest, SetNameTest) {
    Group group = Group(4,
        "Test-Group",
        ImVec4(255, 99, 5, 0.8));

    EXPECT_STREQ("Test-Group", group.getName().c_str());
    group.setName("Test-Group2");
    EXPECT_STREQ("Test-Group2", group.getName().c_str());
}

TEST(GroupTest, TransparencyTest) {
    Group group = Group(4,
        "Test-Group",
        ImVec4(255, 99, 5, 0.8));

    EXPECT_NEAR(group.getGroupVec4().w, 0.8f, 0.0f);
    group.setTransparency(0.5f);
    EXPECT_NEAR(group.getGroupVec4().w, 0.5f, 0.0f);
    EXPECT_THROW(group.setTransparency(1.5f), std::out_of_range);
    EXPECT_NEAR(group.getGroupVec4().w, 0.5f, 0.0f);
}

TEST(GroupTest, GroupVec4Test) {
    Group group = Group(4,
        "Test-Group",
        ImVec4(255, 99, 5, 0.8));

    EXPECT_EQ(group.getGroupVec4().x, 255);
    EXPECT_EQ(group.getGroupVec4().y, 99);
    EXPECT_EQ(group.getGroupVec4().z, 5);
    EXPECT_NEAR(group.getGroupVec4().w, 0.8, 0.000001f);
    group.setColor(ImVec4(3, 1, 128, 0.0));
    EXPECT_EQ(group.getGroupVec4().x, 3);
    EXPECT_EQ(group.getGroupVec4().y, 1);
    EXPECT_EQ(group.getGroupVec4().z, 128);
    EXPECT_NEAR(group.getGroupVec4().w, 0.8, 0.0001f);
    group.setTransparency(0.0f);
    EXPECT_EQ(group.getGroupVec4().x, 3);
    EXPECT_EQ(group.getGroupVec4().y, 1);
    EXPECT_EQ(group.getGroupVec4().z, 128);
    EXPECT_EQ(group.getGroupVec4().w, 0.0f);
}