#include "EFMod.hpp"
#include "json.hpp"
#include <fstream>
#include <any>
#include "LuckyDamage.hpp"
#include "ItemRoulette.hpp"

nlohmann::json config;
void initialize(const std::filesystem::path& p) {
    if (!config.empty()) return;
    if (!std::filesystem::exists(p)) {
        throw std::runtime_error("File does not exist:" + p.string());
    }
    std::ifstream ifs(p);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file:" + p.string());
    }
    ifs >> config;
}


class TideTruns : public EFMod {

    int run(EFModAPI *mod) override {
        return 0;
    }

    void RegisterAPI(EFModAPI *mod) override {

    }

    void RegisterExtend(EFModAPI *mod) override {
        LuckyDamage(mod, config);
        ItemRoulette(mod, config);

    }

    ModMetadata getInfo() override {
        return {
                "时来运转",
            "EternalFuture゙",
            "1.0.0"
        };
    }

    void getPrivate(std::filesystem::path Path) override {
        initialize(Path / "config.json");
    }
};

EFMod* CreateMod() {

    return new TideTruns();
}