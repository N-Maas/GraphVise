//
// Created by Emile Brückner on 2/13/26.
//
#include "CachingController.hpp"


#include <fstream>
#include <iostream>


#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "model/GraphSaver.hpp"
#include "../../rendering/Renderer.hpp"
#include "../../rendering/Camera.hpp"

namespace graphvise
{
    const std::string CachingController::fileExtension = ".bin";
    const std::filesystem::path CachingController::cacheDir = sago::getCacheDir();
    const std::filesystem::path CachingController::cachePath = cacheDir / "graphVise";
    const std::filesystem::path CachingController::graphCachePath = cachePath / "graphs";
    const std::filesystem::path CachingController::exampleGraphsPath = "../resources/Example Graphs";

    const std::filesystem::path CachingController::dataDir = sago::getDataHome();
    const std::filesystem::path CachingController::settingsFile = dataDir / "GraphVise" / ("settings" + fileExtension);
    const std::string CachingController::defaultCacheFilename = "graph" + fileExtension;

    bool CachingController::hasCachedGraphs()
    {
        return !std::filesystem::is_empty(graphCachePath);
    }

    void CachingController::cacheCurrentGraph()
    {
        cacheGraph(GraphSaver::getInstance().getGraph());
    }

    std::vector<std::filesystem::path> CachingController::getExampleGraphNames()
    {

        std::vector<std::filesystem::path> filenames;
        for (const auto& file : std::filesystem::directory_iterator(exampleGraphsPath))
        {
            filenames.push_back(file.path());
        }
        return filenames;
    }


    std::vector<std::filesystem::path> CachingController::getCachedGraphFilenames()
    {
        if (!std::filesystem::exists(graphCachePath))
        {
            std::filesystem::create_directories(graphCachePath);
        }


        std::vector<std::filesystem::path> filenames;
        for (const auto& file : std::filesystem::directory_iterator(graphCachePath))
        {
            filenames.push_back(file.path());
        }
        return filenames;
    }


    void CachingController::cacheGraph(const Graph& graph)
    {
        if (!std::filesystem::exists(graphCachePath))
        {
            std::filesystem::create_directories(graphCachePath);
        }

        std::cout << "caching to" << graphCachePath << std::endl;

        std::ofstream ofs(graphCachePath / (graph.getName() + fileExtension));

        boost::archive::binary_oarchive oa(ofs);

        oa << graph;

        ofs.close();
    }

    void CachingController::loadLastCachedGraph()
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
        loadCachedGraph("../ext/example_graph.bin");
    }

    void CachingController::loadCachedGraph(const std::filesystem::path& filename)
    {
        std::cout << "Loading from" << filename << std::endl;

        if (!std::filesystem::exists(filename))
        {
            std::cerr << "Error: File does not exist: " << filename << std::endl;
            return;
        }

        // Check if file is empty
        if (std::filesystem::is_empty(filename))
        {
            std::cerr << "Error: File is empty: " << filename << std::endl;
            return;
        }
        std::ifstream ifs(filename, std::ios::binary);

        if (!ifs.is_open())
        {
            std::cerr << "Error: Could not open file: " << filename << std::endl;
            return;
        }


        boost::archive::binary_iarchive ia(ifs);

        auto newGraph = Graph({}, {}, "");
        ia >> newGraph;
        ifs.close();
        GraphSaver::getInstance().setGraph(newGraph);
        Renderer::getInstance()->m_camera().resetPosition();

        // clear vertex/edge picking
        if (auto controller = getButtonController()) {
        controller->clearSelection();
        }
        // Notify renderer
        auto renderer = Renderer::getInstance();
        renderer->clearInstanceData();

        std::cout << "Successfully loaded graph: " << newGraph.getName() << std::endl;
    }

    void CachingController::clearCache()
    {
        std::filesystem::remove_all(cachePath);
    }

    std::filesystem::path CachingController::getGraphCacheDir()
    {
        return graphCachePath;
    }

    void CachingController::setButtonController(std::shared_ptr<ButtonController> controller)
    {
        getButtonController() = controller;
    }
}
