#include <gtest/gtest.h>
#include "model/GraphSaver.hpp"

using namespace graphvise;
TEST(GraphTest, CheckGetterAndInitialization) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector<glm::vec3>{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    EXPECT_EQ(graph.getVertices().at(0).getVertexID(), 0);
    EXPECT_EQ(graph.getVertices().at(1).getVertexID(), 1);
    EXPECT_EQ(graph.getVertices().at(2).getVertexID(), 2);
    EXPECT_EQ(graph.getVertices().at(3).getVertexID(), 3);

    EXPECT_EQ(graph.getVertices().at(0).getCoordsVector().x, 1.0f);
    EXPECT_EQ(graph.getVertices().at(0).getCoordsVector().y, 3.0f);
    EXPECT_EQ(graph.getVertices().at(0).getCoordsVector().z, 2.5f);
    EXPECT_EQ(graph.getVertices().at(1).getCoordsVector().x, 0.1f);
    EXPECT_EQ(graph.getVertices().at(1).getCoordsVector().y, 0.0f);
    EXPECT_EQ(graph.getVertices().at(1).getCoordsVector().z, 7.9f);
    EXPECT_EQ(graph.getVertices().at(2).getCoordsVector().x, 5.2f);
    EXPECT_EQ(graph.getVertices().at(2).getCoordsVector().y, -3.1f);
    EXPECT_EQ(graph.getVertices().at(2).getCoordsVector().z, 2.0f);
    EXPECT_EQ(graph.getVertices().at(3).getCoordsVector().x, -1.9f);
    EXPECT_EQ(graph.getVertices().at(3).getCoordsVector().y, -0.3f);
    EXPECT_EQ(graph.getVertices().at(3).getCoordsVector().z, -1.2f);
    EXPECT_THROW(graph.getVertices().at(4), std::out_of_range);

    EXPECT_EQ(graph.getEdges().empty(), true);
    EXPECT_STREQ(graph.getGroups().at(0).getName().c_str(), "Default-Group");
    EXPECT_THROW(graph.getGroups().at(1), std::out_of_range);
    EXPECT_EQ(graph.getCameraBookmarks().empty(), true);

    EXPECT_EQ(graph.getVertexByID(2).getCoordsVector().x, 5.2f);
    EXPECT_EQ(graph.getVertexByID(2).getCoordsVector().y, -3.1f);
    EXPECT_EQ(graph.getVertexByID(2).getCoordsVector().z, 2.0f);

    EXPECT_THROW(graph.getEdgeByID(0), std::out_of_range);
    EXPECT_STREQ(graph.getGroupByID(0).getName().c_str(), "Default-Group");
    EXPECT_THROW(graph.getCameraBookmarkByID(0), std::out_of_range);

    EXPECT_THROW(graph.getEdgeIDByConnectingVerticesIDs(0,1), std::out_of_range);
}

TEST(GraphTest, EdgeByConnectingIDsTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector<glm::vec3>{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    EXPECT_THROW(graph.getEdgeIDByConnectingVerticesIDs(0,1), std::out_of_range);
    graph.addEdge(0, 3);
    EXPECT_EQ(graph.getEdgeIDByConnectingVerticesIDs(0,3), 0);
    graph.addEdge(0, 2);
    EXPECT_EQ(graph.getEdgeIDByConnectingVerticesIDs(0,2), 1);
    EXPECT_THROW(graph.getEdgeIDByConnectingVerticesIDs(0,0), std::out_of_range);
}

TEST(GraphTest, addEdgeTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    EXPECT_EQ(graph.getEdges().empty(), true);
    graph.addEdge(0, 3);
    EXPECT_EQ(graph.getEdges().at(0).getConnectingVerticesIDs().at(0), 0);
    EXPECT_EQ(graph.getEdges().at(0).getConnectingVerticesIDs().at(1), 3);
    EXPECT_EQ(graph.getEdges().at(0).getEdgeID(), 0);
    EXPECT_EQ(graph.getEdgeByID(0).getConnectingVerticesIDs().at(0), 0);
    EXPECT_EQ(graph.getEdgeByID(0).getConnectingVerticesIDs().at(1), 3);
    EXPECT_EQ(graph.getEdgeByID(0).getEdgeID(), 0);
    EXPECT_THROW(graph.getEdges().at(1), std::out_of_range);
    graph.addEdge(9, 2);
    EXPECT_EQ(graph.getEdges().at(1).getConnectingVerticesIDs().at(0), 9);
    EXPECT_EQ(graph.getEdges().at(1).getConnectingVerticesIDs().at(1), 2);
    EXPECT_EQ(graph.getEdges().at(1).getEdgeID(), 1);
    EXPECT_EQ(graph.getEdgeByID(1).getConnectingVerticesIDs().at(0), 9);
    EXPECT_EQ(graph.getEdgeByID(1).getConnectingVerticesIDs().at(1), 2);
    EXPECT_EQ(graph.getEdgeByID(1).getEdgeID(), 1);
    EXPECT_THROW(graph.getEdges().at(2), std::out_of_range);
}

TEST(GraphTest, addGroupTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    EXPECT_EQ(graph.getGroups().size(), 1);
    graph.addEdge(0, 3);
    graph.addGroup("first-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{1,3}, std::vector<std::uint32_t>{0});
    EXPECT_EQ(graph.getGroups().at(1).getGroupID(), 1);
    EXPECT_STREQ(graph.getGroups().at(1).getName().c_str(), "first-Group");
    EXPECT_STREQ(graph.getGroupByID(1).getName().c_str(), "first-Group");
    EXPECT_EQ(graph.getEdgeByID(0).getGroupID(), 1);
    EXPECT_EQ(graph.getVertexByID(0).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(1).getGroupID(), 1);
    EXPECT_EQ(graph.getVertexByID(2).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(3).getGroupID(), 1);
    EXPECT_THROW(graph.getGroups().at(2), std::out_of_range);
    graph.addGroup("second-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{1,2}, std::vector<std::uint32_t>{0});
    EXPECT_EQ(graph.getGroups().at(2).getGroupID(), 2);
    EXPECT_STREQ(graph.getGroups().at(2).getName().c_str(), "second-Group");
    EXPECT_STREQ(graph.getGroupByID(2).getName().c_str(), "second-Group");
    EXPECT_EQ(graph.getEdgeByID(0).getGroupID(), 2);
    EXPECT_EQ(graph.getVertexByID(0).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(1).getGroupID(), 2);
    EXPECT_EQ(graph.getVertexByID(2).getGroupID(), 2);
    EXPECT_EQ(graph.getVertexByID(3).getGroupID(), 1);
    EXPECT_THROW(graph.getGroups().at(3), std::out_of_range);
}

TEST(GraphTest, addCameeraBookmarkTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    EXPECT_EQ(graph.getCameraBookmarks().empty(), true);
    graph.addCameraBookmark("first-CameraBookmark", glm::vec3(5.2f, -3.1f, 2.0f), 0.5f, 4.3f);
    EXPECT_STREQ(graph.getCameraBookmarks().at(0).getName().c_str(), "first-CameraBookmark");
    EXPECT_STREQ(graph.getCameraBookmarkByID(0).getName().c_str(), "first-CameraBookmark");
    EXPECT_THROW(graph.getCameraBookmarks().at(1), std::out_of_range);
    graph.addCameraBookmark("second-CameraBookmark", glm::vec3(5.2f, -3.1f, 2.0f), 0.5f, 4.3f);
    EXPECT_STREQ(graph.getCameraBookmarks().at(1).getName().c_str(), "second-CameraBookmark");
    EXPECT_STREQ(graph.getCameraBookmarkByID(1).getName().c_str(), "second-CameraBookmark");
    EXPECT_THROW(graph.getCameraBookmarks().at(2), std::out_of_range);
}

TEST(GraphTest, highlightRemoveHighlightTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector<glm::vec3>{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();
    graph.addEdge(0, 3);
    graph.addEdge(0, 2);

    graph.highlightByID(std::vector<std::uint32_t>{0,3}, std::vector<std::uint32_t>{1});
    EXPECT_EQ(graph.getVertexByID(0).getTransparency(), 1);
    EXPECT_NEAR(graph.getVertexByID(1).getTransparency(), 0.2, 0.00001f);
    EXPECT_NEAR(graph.getVertexByID(2).getTransparency(), 0.2, 0.00001f);
    EXPECT_EQ(graph.getVertexByID(3).getTransparency(), 1);
    EXPECT_NEAR(graph.getEdgeByID(0).getTransparency(), 0.2, 0.00001f);
    EXPECT_EQ(graph.getEdgeByID(1).getTransparency(), 1);

    graph.removeAllHighlights();
    EXPECT_EQ(graph.getEdgeByID(0).getTransparency(), graph.getGroupByID(graph.getEdgeByID(0).getGroupID()).getTransparency());
    EXPECT_EQ(graph.getEdgeByID(1).getTransparency(), graph.getGroupByID(graph.getEdgeByID(1).getGroupID()).getTransparency());
    EXPECT_EQ(graph.getVertexByID(0).getTransparency(), graph.getGroupByID(graph.getVertexByID(0).getGroupID()).getTransparency());
    EXPECT_EQ(graph.getVertexByID(1).getTransparency(), graph.getGroupByID(graph.getVertexByID(1).getGroupID()).getTransparency());
    EXPECT_EQ(graph.getVertexByID(2).getTransparency(), graph.getGroupByID(graph.getVertexByID(2).getGroupID()).getTransparency());
    EXPECT_EQ(graph.getVertexByID(3).getTransparency(), graph.getGroupByID(graph.getVertexByID(3).getGroupID()).getTransparency());
}

TEST(GraphTest, deleteAllGroupsTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    graph.addGroup("first-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{0,1}, std::vector<std::uint32_t>{});
    graph.addGroup("second-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{2,3}, std::vector<std::uint32_t>{});
    EXPECT_EQ(graph.getGroups().size(), 3);
    graph.deleteAllGroups();
    EXPECT_EQ(graph.getGroups().size(), 1);
    EXPECT_EQ(graph.getVertexByID(0).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(1).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(2).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(3).getGroupID(), 0);
}

TEST(GraphTest, deleteCameraBookmarksTest) {
    GraphSaver::getGraphSaver().setGraph(Graph(std::vector{
       glm::vec3(1.0f, 3.0f, 2.5f),
       glm::vec3(0.1f, 0.0f, 7.9f),
       glm::vec3(5.2f, -3.1f, 2.0f),
       glm::vec3(-1.9f, -0.3f, -1.2f)}));
    Graph graph = GraphSaver::getGraphSaver().getGraph();

    graph.addCameraBookmark("first-CameraBookmark", glm::vec3(5.2f, -3.1f, 2.0f), 0.5f, 4.3f);
    graph.addCameraBookmark("second-CameraBookmark", glm::vec3(5.2f, -3.1f, 2.0f), 0.5f, 4.3f);
    EXPECT_EQ(graph.getCameraBookmarks().size(), 2);
    EXPECT_THROW(graph.deleteCameraBookmarks(2), std::out_of_range);
    graph.deleteCameraBookmarks(1);
    EXPECT_EQ(graph.getCameraBookmarks().size(), 1);
    EXPECT_THROW(graph.deleteCameraBookmarks(1), std::out_of_range);
}