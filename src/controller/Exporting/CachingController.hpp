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
        static bool hasCachedGraphs();
        static void cacheCurrentGraph();

        static std::vector<std::filesystem::path> getCachedGraphFilenames();

        static void cacheGraph(const Graph& graph);
        static void loadLastCachedGraph();
        static void loadDefaultGraph();
        static void loadCachedGraph(const std::filesystem::path& filename);
        static void clearCache();
        static std::filesystem::path getGraphCacheDir();

    private:
        static const std::string fileExtention;
        static const std::filesystem::path cacheDir;
        static const std::filesystem::path cachePath;
        static const std::filesystem::path graphCachePath;
        static const std::filesystem::path dataDir;
        static const std::filesystem::path settingsFile;
        static const std::string defaultCacheFilename;

    };
}


#endif //GRAPHVISE_CACHINGCONTROLLER_HPP
