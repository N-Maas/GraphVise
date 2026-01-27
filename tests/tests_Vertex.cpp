#include <gtest/gtest.h>
#include "../src/model/Vertex.hpp"
#include "../src/model/Graph.hpp"
#include "../ext/imgui/imgui.h"
#include "model/GraphSaver.hpp"

using namespace graphvise;
TEST(VertexTest, CheckGetter) {
    Graph graph = Graph(std::vector<glm::vec3>{glm::vec3(1.0f, 3.0f, 2.5f)});
    GraphSaver::getGraphSaver().setGraph(graph);
    Vertex vertex = graph.getVertexByID(0);

    EXPECT_EQ(vertex.getVertexID(), 0);
    EXPECT_EQ(vertex.getGroupID(), 0);
    EXPECT_EQ(vertex.getCoordsVector().x, 1.0f);
    EXPECT_EQ(vertex.getCoordsVector().y, 3.0f);
    EXPECT_EQ(vertex.getCoordsVector().z, 2.5f);
    EXPECT_EQ(vertex.getVertexVec4().x, 0);    //Value of the Default-Group
    EXPECT_EQ(vertex.getVertexVec4().y, 134);  //Value of the Default-Group
    EXPECT_EQ(vertex.getVertexVec4().z, 139);  //Value of the Default-Group
    EXPECT_EQ(vertex.getVertexVec4().w, 1);    //Value of the Default-Group
    EXPECT_EQ(vertex.getTransparency(), 1);
}

TEST(VertexTest, TransparencyTest) {
    Graph graph = Graph(std::vector<glm::vec3>{glm::vec3(1.0f, 3.0f, 2.5f)});
    GraphSaver::getGraphSaver().setGraph(graph);
    Vertex vertex = graph.getVertexByID(0);

    EXPECT_EQ(vertex.getTransparency(), 1.0f);   //Value of the Default-Group
    vertex.setOwnTransparency(0.6f);
    EXPECT_EQ(GraphSaver::getGraphSaver().getGraph().getGroupByID(0).getGroupVec4().w, 1);
    EXPECT_EQ(vertex.getTransparency(), 0.6f);
    vertex.deleteOwnTransparency();
    EXPECT_EQ(vertex.getTransparency(), 1.0f);   //Value of the Default-Group
    EXPECT_EQ(GraphSaver::getGraphSaver().getGraph().getGroupByID(0).getGroupVec4().w, 1);
    EXPECT_THROW(vertex.setOwnTransparency(4.7f), std::out_of_range);
    EXPECT_EQ(vertex.getTransparency(), 1.0f);
}

TEST(VertexTest, SetGroupTest) {
    Graph graph = Graph(std::vector<glm::vec3>{glm::vec3(1.0f, 3.0f, 2.5f)});
    GraphSaver::getGraphSaver().setGraph(graph);
    Vertex vertex = graph.getVertexByID(0);

    vertex.setGroup(34);
    EXPECT_EQ(vertex.getGroupID(), 34);
    vertex.setGroup(6);
    EXPECT_EQ(vertex.getGroupID(), 6);
}