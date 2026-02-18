#include <gtest/gtest.h>
#include "../src/model/Edge.hpp"
#include "../src/model/Graph.hpp"
#include "../ext/imgui/imgui.h"
#include "model/GraphSaver.hpp"

using namespace graphvise;
TEST(EdgeTest, CheckGetter) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3,4}});
    GraphSaver::getInstance().setGraph(graph);
    Graph graph1 = GraphSaver::getInstance().getGraph();
    Edge edge1 = graph1.getEdgeByID(0);
    EXPECT_EQ(edge1.getID(), 0);
    EXPECT_EQ(edge1.getConnectedGroupID(), 1);
    EXPECT_EQ(edge1.getConnectingVerticesIDs().first, 3);
    EXPECT_EQ(edge1.getConnectingVerticesIDs().second, 4);
    EXPECT_NEAR(graph1.getVertexVec4ByID(edge1.getID()).x, 1, 0.0001);    //Value of the Default-Group
    EXPECT_NEAR(graph1.getVertexVec4ByID(edge1.getID()).y, 0, 0.0001);  //Value of the Default-Group
    EXPECT_NEAR(graph1.getVertexVec4ByID(edge1.getID()).z, 0, 0.0001);  //Value of the Default-Group
    EXPECT_EQ(graph1.getVertexVec4ByID(edge1.getID()).w, 1);    //Value of the Default-Group
}

TEST(EdgeTest, TransparencyTest) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3,4}});
    GraphSaver::getInstance().setGraph(graph);
    Graph graph1 = GraphSaver::getInstance().getGraph();
    Edge edge1 = graph1.getEdgeByID(0);

    EXPECT_NEAR(graph1.getVertexVec4ByID(edge1.getID()).w, 1.0f, 0.0f);   //Value of the Default-Group
    graph1.getEdgeByID(0).setOwnTransparency(0.6f);
    EXPECT_EQ(GraphSaver::getInstance().getGraph().getGroupByID(0).getVec4().w, 1);
    EXPECT_NEAR(graph1.getEdgeVec4ByID(edge1.getID()).w, 0.6f, 0.0001f);
    graph1.getEdgeByID(0).deleteOwnTransparency();
    EXPECT_NEAR(graph1.getEdgeVec4ByID(edge1.getID()).w, 1.0f, 0.0f);   //Value of the Default-Group
    EXPECT_EQ(GraphSaver::getInstance().getGraph().getGroupByID(0).getVec4().w, 1);
    EXPECT_THROW(edge1.setOwnTransparency(4.7f), std::out_of_range);
    EXPECT_NEAR(graph1.getEdgeVec4ByID(edge1.getID()).w, 1.0f, 0.0f);
}

TEST(EdgeTest, SetGroupTest) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3,4}});
    GraphSaver::getInstance().setGraph(graph);
    Edge edge1 = Edge(0,
        14,
        457);

    edge1.setGroup(34);
    EXPECT_EQ(edge1.getConnectedGroupID(), 34);
    edge1.setGroup(4);
    EXPECT_EQ(edge1.getConnectedGroupID(), 4);
}