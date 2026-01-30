#include <gtest/gtest.h>
#include "../src/model/Edge.hpp"
#include "../src/model/Graph.hpp"
#include "../ext/imgui/imgui.h"
#include "model/GraphSaver.hpp"

using namespace graphvise;
TEST(EdgeTest, CheckGetter) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)});
    GraphSaver::getGraphSaver().setGraph(graph);
    Edge edge1 = Edge(0,
        14,
        457);

    EXPECT_EQ(edge1.getEdgeID(), 0);
    EXPECT_EQ(edge1.getGroupID(), 0);
    EXPECT_EQ(edge1.getConnectingVerticesIDs().first, 14);
    EXPECT_EQ(edge1.getConnectingVerticesIDs().second, 457);
    EXPECT_NEAR(edge1.getEdgeVec4().x, 0.2, 0.0001);    //Value of the Default-Group
    EXPECT_NEAR(edge1.getEdgeVec4().y, 0, 0.0001);  //Value of the Default-Group
    EXPECT_NEAR(edge1.getEdgeVec4().z, 0.133333, 0.0001);  //Value of the Default-Group
    EXPECT_EQ(edge1.getEdgeVec4().w, 1);    //Value of the Default-Group
}

TEST(EdgeTest, TransparencyTest) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)});
    GraphSaver::getGraphSaver().setGraph(graph);
    Edge edge1 = Edge(0,
        14,
        457);

    EXPECT_NEAR(edge1.getEdgeVec4().w, 1.0f, 0.0f);   //Value of the Default-Group
    edge1.setOwnTransparency(0.6f);
    EXPECT_EQ(GraphSaver::getGraphSaver().getGraph().getGroupByID(0).getGroupVec4().w, 1);
    EXPECT_NEAR(edge1.getEdgeVec4().w, 0.6f, 0.0001f);
    edge1.deleteOwnTransparency();
    EXPECT_NEAR(edge1.getEdgeVec4().w, 1.0f, 0.0f);   //Value of the Default-Group
    EXPECT_EQ(GraphSaver::getGraphSaver().getGraph().getGroupByID(0).getGroupVec4().w, 1);
    EXPECT_THROW(edge1.setOwnTransparency(4.7f), std::out_of_range);
    EXPECT_NEAR(edge1.getEdgeVec4().w, 1.0f, 0.0f);
}

TEST(EdgeTest, SetGroupTest) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)});
    GraphSaver::getGraphSaver().setGraph(graph);
    Edge edge1 = Edge(0,
        14,
        457);

    edge1.setGroup(34);
    EXPECT_EQ(edge1.getGroupID(), 34);
    edge1.setGroup(4);
    EXPECT_EQ(edge1.getGroupID(), 4);
}