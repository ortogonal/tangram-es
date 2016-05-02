#include "tangram.h"
#include "gl.h"
#include "platform.h"
#include "data/dataSource.h"
#include "scene/sceneLoader.h"
#include "scene/scene.h"
#include "style/style.h"
#include "scene/styleContext.h"
#include "util/mapProjection.h"
#include "tile/tile.h"
#include "tile/tileBuilder.h"
#include "tile/tileTask.h"
#include "text/fontContext.h"

#include <vector>
#include <iostream>
#include <fstream>

#include "benchmark/benchmark_api.h"
#include "benchmark/benchmark.h"

using namespace Tangram;

struct TestContext {

    MercatorProjection s_projection;
    const char* sceneFile = "scene.yaml";

    std::shared_ptr<Scene> scene;
    StyleContext styleContext;

    std::shared_ptr<DataSource> source;

    std::vector<char> rawTileData;

    TileBuilder tileBuilder;

    void loadScene(const char* sceneFile) {
        auto sceneRelPath = setResourceRoot(sceneFile);
        auto sceneString = stringFromFile(sceneRelPath.c_str(), PathType::resource);

        YAML::Node sceneNode;

        try { sceneNode = YAML::Load(sceneString); }
        catch (YAML::ParserException e) {
            LOGE("Parsing scene config '%s'", e.what());
            return;
        }
        scene = std::make_shared<Scene>();
        SceneLoader::applyConfig(sceneNode, *scene);

        styleContext.initFunctions(*scene);
        styleContext.setKeywordZoom(0);

        source = scene->dataSources()[0];
        tileBuilder.setScene(scene);
    }

    void loadTile(const char* path){
        std::ifstream resource(path, std::ifstream::ate | std::ifstream::binary);
        if(!resource.is_open()) {
            LOGE("Failed to read file at path: %s", path.c_str());
            return;
        }

        size_t _size = resource.tellg();
        resource.seekg(std::ifstream::beg);

        rawTileData.resize(_size);

        resource.read(&rawTileData[0], _size);
        resource.close();
    }

    void processTile() {
        Tile tile(TileID{0,0,10,10,0}, s_projection);
        source = scene->dataSources()[0];
        auto task = source->createTask(tile.getID());
        auto& t = dynamic_cast<DownloadTileTask&>(*task);
        t.rawTileData = std::make_shared<std::vector<char>>(rawTileData);


        bool ok = tileBuilder.build(*task);

        benchmark::DoNotOptimize(ok);

        LOG("ok %d / bytes - %d", ok, task->tile()->getMemoryUsage());

    }
};

class TileLoadingFixture : public benchmark::Fixture {
public:
    TestContext ctx;
    MercatorProjection s_projection;
    const char* sceneFile = "scene.yaml";

    std::shared_ptr<Tile> result;

    void SetUp() override {
        LOG("SETUP");
        ctx.loadScene(sceneFile);
        ctx.loadTile("tile.mvt");
        LOG("Ready");
    }
    void TearDown() override {
        result.reset();
        LOG("TEARDOWN");
    }
};

BENCHMARK_DEFINE_F(TileLoadingFixture, BuildTest)(benchmark::State& st) {

    while (st.KeepRunning()) {
        ctx.processTile();
    }
}

BENCHMARK_REGISTER_F(TileLoadingFixture, BuildTest);



BENCHMARK_MAIN();
