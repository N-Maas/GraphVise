//
// Created by Emile Brückner on 2/13/26.
//

#ifndef GRAPHVISE_CACHINGCONTROLLER_HPP
#define GRAPHVISE_CACHINGCONTROLLER_HPP
#include <filesystem>

#include "platform_folders.h"
#include "model/Graph.hpp"

namespace graphvise
{
    class CachingController
    {
    public:
        bool hasCachedGraphs() const;

        void cacheCurrentGraph() const;
        void cacheGraph(Graph graph) const;
        void loadLastCachedGraph() const;
        static void loadDefaultGraph();
        static void loadCachedGraph(std::filesystem::path filename);
        void clearCache() const;

    private:


        const std::filesystem::path cacheDir = sago::getCacheDir();
        const std::filesystem::path cachePath = cacheDir / "graphVise";
        const std::filesystem::path graphCachePath = cachePath / "graphs";
        const std::filesystem::path settings = static_cast<std::filesystem::path>(sago::getDataHome()) / "settings";
        const std::string defaultCacheFilename = "cached_graph.bin";

    };

}


#endif //GRAPHVISE_CACHINGCONTROLLER_HPP
