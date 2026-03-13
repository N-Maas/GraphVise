//
// Created by Emile Brückner on 2/13/26.
//

#ifndef GRAPHVISE_CACHINGCONTROLLER_HPP
#define GRAPHVISE_CACHINGCONTROLLER_HPP

#include <filesystem>

#include <sago/platform_folders.h>
#include "model/Graph.hpp"
#include "controller/ButtonController.hpp"

namespace graphvise
{

    class CachingController
    {
    public:
        static bool hasCachedGraphs();
        static void cacheCurrentGraph();

        static std::vector<std::filesystem::path> getExampleGraphNames();
        static std::vector<std::filesystem::path> getCachedGraphFilenames();

        static void cacheGraph(const Graph& graph);
        static void loadLastCachedGraph();
        static void loadDefaultGraph();
        static void loadCachedGraph(const std::filesystem::path& filename);
        static void clearCache();
        static std::filesystem::path getGraphCacheDir();
        static void setButtonController(std::shared_ptr<ButtonController> controller);

    private:
        static const std::string fileExtension;
        static const std::filesystem::path cacheDir;
        static const std::filesystem::path cachePath;
        static const std::filesystem::path graphCachePath;
        static const std::filesystem::path dataDir;
        static const std::filesystem::path settingsFile;
        static const std::string defaultCacheFilename;
        static const std::filesystem::path exampleGraphsPath;
        static std::shared_ptr<ButtonController>& getButtonController() {
            static std::shared_ptr<ButtonController> instance;
            return instance;
        }
    };
}


#endif //GRAPHVISE_CACHINGCONTROLLER_HPP
