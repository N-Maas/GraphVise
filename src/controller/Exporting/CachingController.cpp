//
// Created by Emile Brückner on 2/13/26.
//
#include "CachingController.hpp"


#include <fstream>


#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>

#include "model/GraphSaver.hpp"
#include "rendering/renderer.hpp"

namespace graphvise
{
    bool CachingController::hasCachedGraphs() const
    {
        return !std::filesystem::is_empty(graphCachePath);
    }

    void CachingController::cacheCurrentGraph() const
    {
        cacheGraph(GraphSaver::getInstance().getGraph());
    }

    void CachingController::cacheGraph(const Graph& graph) const
    {
        if (!std::filesystem::exists(graphCachePath))
        {
            std::filesystem::create_directories(graphCachePath);
        }

        std::ofstream ofs(graphCachePath / (graph.getName() + fileExtention));

        boost::archive::binary_oarchive oa(ofs);

        oa << graph;

        ofs.close();
    }

    void CachingController::loadLastCachedGraph() const
    {
        std::filesystem::file_time_type mostRecentWriteTime;
        std::filesystem::path mostRecentFile;

        bool first = true;

        for (const auto& file : std::filesystem::directory_iterator(graphCachePath))
        {
            if (first)
            {
                mostRecentWriteTime = std::filesystem::last_write_time(file);
                mostRecentFile = file.path();
                first = false;
            }
            else if (std::filesystem::last_write_time(file) > mostRecentWriteTime)
            {
                mostRecentWriteTime = std::filesystem::last_write_time(file);
                mostRecentFile = file.path();
            }
        }

        loadCachedGraph(mostRecentFile);
    }

    void CachingController::loadDefaultGraph()
    {
        auto const welcomeGraph = Graph({
                                       {-12, 3, 0},
                                       {-11, 0, 0},
                                       {-10, 1, 0},
                                       {-9, 0, 0},
                                       {-8, 3, 0},
                                       {-7, 3, 0},
                                       {-5, 3, 0},
                                       {-7, 1.5, 0},
                                       {-6, 1.5, 0},
                                       {-7, 0, 0},
                                       {-5, 0, 0},
                                       {-4, 3, 0},
                                       {-4, 0, 0},
                                       {-2, 0, 0},
                                       {-1, 3, 0},
                                       {1, 3, 0},
                                       {-1, 0, 0},
                                       {1, 0, 0},
                                       {2, 3, 0},
                                       {4, 3, 0},
                                       {2, 0, 0},
                                       {4, 0, 0},
                                       {5, 0, 0},
                                       {6, 3, 0},
                                       {7, 2, 0},
                                       {8, 3, 0},
                                       {9, 0, 0},
                                       {10, 3, 0},
                                       {12, 3, 0},
                                       {10, 1.5, 0},
                                       {11, 1.5, 0},
                                       {10, 0, 0},
                                       {12, 0, 0}

                                   },
                                   {
                                       {0, 1},
                                       {1, 2},
                                       {2, 3},
                                       {3, 4},
                                       {5, 6},
                                       {5,7},
                                       {7, 8},
                                       {7, 9},
                                       {9, 10},
                                       {11, 12},
                                       {12, 13},
                                       {14, 15},
                                       {14, 16},
                                       {16, 17},
                                       {18, 19},
                                       {18, 20},
                                       {19, 21},
                                       {20, 21},
                                       {22, 23},
                                       {23,24},
                                       {24,25},
                                       {25,26},
                                       {27,28},
                                       {27,29},
                                       {29,30},
                                       {29,31},
                                       {31,32}
                                   },

                                   "Welcome to GraphVise");


        std::ifstream ifs("../ext/example_graph.bin");

        boost::archive::binary_iarchive ia(ifs);

        auto newGraph = Graph({}, {}, "");
        ia >> newGraph;
        ifs.close();

        GraphSaver::getInstance().setGraph(welcomeGraph);
        Renderer::getInstance()->m_camera().position_world_space=glm::vec3(0,0, 15);
    }

    void CachingController::loadCachedGraph(std::filesystem::path filename)
    {
        std::ifstream ifs(filename);

        boost::archive::binary_iarchive ia(ifs);

        auto newGraph = Graph({}, {}, "");
        ia >> newGraph;
        ifs.close();

        GraphSaver::getInstance().setGraph(newGraph);
    }

    void CachingController::clearCache() const
    {
        std::filesystem::remove_all(cachePath);
    }
}
