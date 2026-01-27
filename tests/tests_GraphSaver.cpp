#include <gtest/gtest.h>
#include "../ext/imgui/imgui.h"
#include "model/GraphSaver.hpp"

TEST(GraphSaverTest, getSetGraphTest) {
    GraphSaver graphSaver = GraphSaver::getGraphSaver();

    /*This test case works in isolation. However, because
    singletons are synchronized via Test, no error is thrown here due to the order of the tests. To ensure that this
    test does not throw a false error, it is commented out. */
    //EXPECT_THROW(graphSaver.getGraph(), std::logic_error);

    Graph graph = Graph(std::vector{glm::vec3(1.0f, 3.0f, 2.5f)});

    graphSaver.setGraph(graph);
    EXPECT_NO_THROW(graphSaver.getGraph());
}