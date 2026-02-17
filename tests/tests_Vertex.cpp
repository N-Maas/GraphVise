#include <gtest/gtest.h>
#include "../src/model/Vertex.hpp"
#include "../src/model/Graph.hpp"
#include "../ext/imgui/imgui.h"
#include "model/GraphSaver.hpp"

using namespace graphvise;
TEST(VertexTest, CheckGetter) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3,4}});
    GraphSaver::getInstance().setGraph(graph);
    Vertex vertex = graph.getVertexByID(0);

    EXPECT_EQ(vertex.getID(), 0);
    EXPECT_EQ(vertex.getConnectedGroupID(), 0);
    EXPECT_EQ(vertex.getCoordsVector().x, 1.0f);
    EXPECT_EQ(vertex.getCoordsVector().y, 3.0f);
    EXPECT_EQ(vertex.getCoordsVector().z, 2.5f);
    EXPECT_NEAR(graph.getVertexVec4ByID(vertex.getID()).x, 1, 0.0001);    //Value of the Default-Group
    EXPECT_NEAR(graph.getVertexVec4ByID(vertex.getID()).y, 0, 0.0001);  //Value of the Default-Group
    EXPECT_NEAR(graph.getVertexVec4ByID(vertex.getID()).z, 0, 0.0001);  //Value of the Default-Group
    EXPECT_EQ(graph.getVertexVec4ByID(vertex.getID()).w, 1);    //Value of the Default-Group
}

TEST(VertexTest, TransparencyTest) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3,4}});
    GraphSaver::getInstance().setGraph(graph);
    Graph graph1 = GraphSaver::getInstance().getGraph();
    Vertex vertex = graph1.getVertexByID(0);

    EXPECT_EQ(graph1.getVertexVec4ByID(vertex.getID()).w, 1.0f);   //Value of the Default-Group
    graph1.getVertexByID(0).setOwnTransparency(0.6f);
    EXPECT_EQ(GraphSaver::getInstance().getGraph().getGroupByID(0).getVec4().w, 1);
    EXPECT_EQ(graph1.getVertexVec4ByID(vertex.getID()).w, 0.6f);
    graph1.getVertexByID(0).deleteOwnTransparency();
    EXPECT_EQ(graph1.getVertexVec4ByID(vertex.getID()).w, 1.0f);   //Value of the Default-Group
    EXPECT_EQ(GraphSaver::getInstance().getGraph().getGroupByID(0).getVec4().w, 1);
    EXPECT_THROW(vertex.setOwnTransparency(4.7f), std::out_of_range);
    EXPECT_EQ(graph1.getVertexVec4ByID(vertex.getID()).w, 1.0f);
}

TEST(VertexTest, SetGroupTest) {
    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3,4}});
    GraphSaver::getInstance().setGraph(graph);
    Vertex vertex = graph.getVertexByID(0);

    vertex.setGroup(34);
    EXPECT_EQ(vertex.getConnectedGroupID(), 34);
    vertex.setGroup(6);
    EXPECT_EQ(vertex.getConnectedGroupID(), 6);
}