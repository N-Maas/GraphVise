#include <gtest/gtest.h>
#include "model/GraphSaver.hpp"

using namespace graphvise;
TEST(GraphTest, CheckGetterAndInitialization) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{3, 6}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    EXPECT_EQ(graph.getVertices().at(0).getID(), 0);
    EXPECT_EQ(graph.getVertices().at(1).getID(), 1);
    EXPECT_EQ(graph.getVertices().at(2).getID(), 2);
    EXPECT_EQ(graph.getVertices().at(3).getID(), 3);

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

    EXPECT_EQ(graph.getEdges().size(), 1);
    EXPECT_STREQ(graph.getGroups().at(0).getName().c_str(), "Default-VertexGroup");
    EXPECT_THROW(graph.getGroups().at(2), std::out_of_range);
    EXPECT_EQ(graph.getCameraBookmarks().empty(), true);

    EXPECT_EQ(graph.getVertexByID(2).getCoordsVector().x, 5.2f);
    EXPECT_EQ(graph.getVertexByID(2).getCoordsVector().y, -3.1f);
    EXPECT_EQ(graph.getVertexByID(2).getCoordsVector().z, 2.0f);

    EXPECT_THROW(graph.getEdgeByID(1), std::out_of_range);
    EXPECT_STREQ(graph.getGroupByID(0).getName().c_str(), "Default-VertexGroup");
    EXPECT_THROW(graph.getCameraBookmarkByID(0), std::out_of_range);

    EXPECT_THROW(graph.getEdgeIDByConnectingVerticesIDs(0,1), std::out_of_range);
}

TEST(GraphTest, EdgeByConnectingIDsTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{0, 2}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    EXPECT_EQ(graph.getEdgeIDByConnectingVerticesIDs(0,3), 0);
    EXPECT_EQ(graph.getEdgeIDByConnectingVerticesIDs(0,2), 1);
    EXPECT_THROW(graph.getEdgeIDByConnectingVerticesIDs(0,0), std::out_of_range);
}

TEST(GraphTest, addEdgeTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{9, 2}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    EXPECT_EQ(graph.getEdges().at(0).getConnectingVerticesIDs().first, 0);
    EXPECT_EQ(graph.getEdges().at(0).getConnectingVerticesIDs().second, 3);
    EXPECT_EQ(graph.getEdges().at(0).getID(), 0);
    EXPECT_EQ(graph.getEdgeByID(0).getConnectingVerticesIDs().first, 0);
    EXPECT_EQ(graph.getEdgeByID(0).getConnectingVerticesIDs().second, 3);
    EXPECT_EQ(graph.getEdgeByID(0).getID(), 0);
    EXPECT_EQ(graph.getEdges().at(1).getConnectingVerticesIDs().first, 9);
    EXPECT_EQ(graph.getEdges().at(1).getConnectingVerticesIDs().second, 2);
    EXPECT_EQ(graph.getEdges().at(1).getID(), 1);
    EXPECT_EQ(graph.getEdgeByID(1).getConnectingVerticesIDs().first, 9);
    EXPECT_EQ(graph.getEdgeByID(1).getConnectingVerticesIDs().second, 2);
    EXPECT_EQ(graph.getEdgeByID(1).getID(), 1);
    EXPECT_THROW(graph.getEdges().at(2), std::out_of_range);
}

TEST(GraphTest, addGroupTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    EXPECT_EQ(graph.getGroups().size(), 2);
    graph.addGroup("first-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{1,3}, std::vector<std::uint32_t>{0});
    EXPECT_EQ(graph.getGroups().at(1).getID(), 1);
    EXPECT_STREQ(graph.getGroups().at(2).getName().c_str(), "first-Group");
    EXPECT_STREQ(graph.getGroupByID(2).getName().c_str(), "first-Group");
    EXPECT_EQ(graph.getEdgeByID(0).getConnectedGroupID(), 2);
    EXPECT_EQ(graph.getVertexByID(0).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(1).getGroupID(), 2);
    EXPECT_EQ(graph.getVertexByID(2).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(3).getGroupID(), 2);
    EXPECT_THROW(graph.getGroups().at(3), std::out_of_range);
    graph.addGroup("second-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{1,2}, std::vector<std::uint32_t>{0});
    EXPECT_EQ(graph.getGroups().at(2).getID(), 2);
    EXPECT_STREQ(graph.getGroups().at(3).getName().c_str(), "second-Group");
    EXPECT_STREQ(graph.getGroupByID(3).getName().c_str(), "second-Group");
    EXPECT_EQ(graph.getEdgeByID(0).getConnectedGroupID(), 3);
    EXPECT_EQ(graph.getVertexByID(0).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(1).getGroupID(), 3);
    EXPECT_EQ(graph.getVertexByID(2).getGroupID(), 3);
    EXPECT_EQ(graph.getVertexByID(3).getGroupID(), 2);
    EXPECT_THROW(graph.getGroups().at(4), std::out_of_range);
}

TEST(GraphTest, addCameeraBookmarkTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{9, 2}}));
    Graph graph = GraphSaver::getInstance().getGraph();

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
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{0, 2}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    graph.highlightByID(std::vector<std::uint32_t>{0,3}, std::vector<std::uint32_t>{1});
    EXPECT_EQ(graph.getVertexByID(0).getVec4().w, 1);
    EXPECT_NEAR(graph.getVertexByID(1).getVec4().w, 0.2, 0.00001f);
    EXPECT_NEAR(graph.getVertexByID(2).getVec4().w, 0.2, 0.00001f);
    EXPECT_EQ(graph.getVertexByID(3).getVec4().w, 1);
    EXPECT_NEAR(graph.getEdgeByID(0).getVec4().w, 0.2, 0.00001f);
    EXPECT_EQ(graph.getEdgeByID(1).getVec4().w, 1);

    graph.removeAllHighlights();
    EXPECT_EQ(graph.getEdgeByID(0).getVec4().w, graph.getGroupByID(graph.getEdgeByID(0).getConnectedGroupID()).getVec4().w);
    EXPECT_EQ(graph.getEdgeByID(1).getVec4().w, graph.getGroupByID(graph.getEdgeByID(1).getConnectedGroupID()).getVec4().w);
    EXPECT_EQ(graph.getVertexByID(0).getVec4().w, graph.getGroupByID(graph.getVertexByID(0).getGroupID()).getVec4().w);
    EXPECT_EQ(graph.getVertexByID(1).getVec4().w, graph.getGroupByID(graph.getVertexByID(1).getGroupID()).getVec4().w);
    EXPECT_EQ(graph.getVertexByID(2).getVec4().w, graph.getGroupByID(graph.getVertexByID(2).getGroupID()).getVec4().w);
    EXPECT_EQ(graph.getVertexByID(3).getVec4().w, graph.getGroupByID(graph.getVertexByID(3).getGroupID()).getVec4().w);
}

TEST(GraphTest, deleteAllGroupsTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
        glm::vec3(1.0f, 3.0f, 2.5f),
        glm::vec3(0.1f, 0.0f, 7.9f),
        glm::vec3(5.2f, -3.1f, 2.0f),
        glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{9, 2}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    graph.addGroup("first-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{0,1}, std::vector<std::uint32_t>{});
    graph.addGroup("second-Group", ImVec4{0,53,12,134}, std::vector<std::uint32_t>{2,3}, std::vector<std::uint32_t>{});
    EXPECT_EQ(graph.getGroups().size(), 4);
    graph.deleteAllGroups();
    EXPECT_EQ(graph.getGroups().size(), 2);
    EXPECT_EQ(graph.getVertexByID(0).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(1).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(2).getGroupID(), 0);
    EXPECT_EQ(graph.getVertexByID(3).getGroupID(), 0);
}

TEST(GraphTest, deleteCameraBookmarksTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
       glm::vec3(1.0f, 3.0f, 2.5f),
       glm::vec3(0.1f, 0.0f, 7.9f),
       glm::vec3(5.2f, -3.1f, 2.0f),
       glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{9, 2}}));
    Graph graph = GraphSaver::getInstance().getGraph();

    graph.addCameraBookmark("first-CameraBookmark", glm::vec3(5.2f, -3.1f, 2.0f), 0.5f, 4.3f);
    graph.addCameraBookmark("second-CameraBookmark", glm::vec3(5.2f, -3.1f, 2.0f), 0.5f, 4.3f);
    EXPECT_EQ(graph.getCameraBookmarks().size(), 2);
    EXPECT_THROW(graph.deleteCameraBookmarks(2), std::out_of_range);
    graph.deleteCameraBookmarks(1);
    EXPECT_EQ(graph.getCameraBookmarks().size(), 1);
    EXPECT_THROW(graph.deleteCameraBookmarks(1), std::out_of_range);
}

TEST(GraphTest, sortedEdgesAndVertexTest) {
    GraphSaver::getInstance().setGraph(Graph(std::vector{
       glm::vec3(1.0f, 3.0f, 2.5f),
       glm::vec3(0.1f, 0.0f, 7.9f),
       glm::vec3(5.2f, -3.1f, 2.0f),
       glm::vec3(-1.9f, -0.3f, -1.2f)}, std::vector{std::pair<std::uint32_t, std::uint32_t>{0,3},{9, 2}, {1,2},{7, 5}}));
    Graph &graph = GraphSaver::getInstance().getGraph();

    for (Vertex* vertex : graph.getVerticesSortedByTransparency()) {
        EXPECT_EQ(vertex->getVec4().w, 1);
    }
    for (Edge* edge : graph.getEdgesSortedByTransparency()) {
        EXPECT_EQ(edge->getVec4().w, 1);
    }
    graph.highlightByID(std::vector<std::uint32_t>{1}, std::vector<std::uint32_t>{1});
    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(0)->getID(), 1);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(1)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(2)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(3)->getVec4().w, 0.2, 0.0001);

    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(0)->getID(), 1);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(1)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(2)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(3)->getVec4().w, 0.2, 0.0001);

    graph.addGroup("first", ImVec4(), std::vector<std::uint32_t>{1,3}, std::vector<std::uint32_t>{1,3});
    graph.getGroupByID(2).setTransparency(0.5);

    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(0)->getID(), 1);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(1)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(2)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(3)->getVec4().w, 0.2, 0.0001);

    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(0)->getID(), 1);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(1)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(2)->getVec4().w, 0.2, 0.0001);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(3)->getVec4().w, 0.2, 0.0001);

    graph.removeAllHighlights();

    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(1)->getVec4().w, 1);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(2)->getVec4().w, 0.5, 0.0001);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(3)->getVec4().w, 0.5, 0.0001);

    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(1)->getVec4().w, 1);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(2)->getVec4().w, 0.5, 0.0001);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(3)->getVec4().w, 0.5, 0.0001);

    graph.getGroupByID(2).setTransparency(0.4);

    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getVerticesSortedByTransparency().at(1)->getVec4().w, 1);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(2)->getVec4().w, 0.4, 0.0001);
    EXPECT_NEAR(graph.getVerticesSortedByTransparency().at(3)->getVec4().w, 0.4, 0.0001);

    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(0)->getVec4().w, 1);
    EXPECT_EQ(graph.getEdgesSortedByTransparency().at(1)->getVec4().w, 1);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(2)->getVec4().w, 0.4, 0.0001);
    EXPECT_NEAR(graph.getEdgesSortedByTransparency().at(3)->getVec4().w, 0.4, 0.0001);
}