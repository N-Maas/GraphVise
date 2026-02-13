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
        std::ifstream ifs("../ext/example_graph.bin");

        boost::archive::binary_iarchive ia(ifs);

        auto newGraph = Graph({}, {}, "");
        ia >> newGraph;
        ifs.close();

        GraphSaver::getInstance().setGraph(newGraph);
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
