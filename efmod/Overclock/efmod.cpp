//
// Created by EternalFuture゙ on 2025/3/22.
//

#include "efmod_core.hpp"
#include "TEFMod.hpp"
#include "BaseType.hpp"
#include "Logger.hpp"

TEFMod::Logger* g_log;
TEFMod::TEFModAPI* g_api;

struct ItemFields {
    TEFMod::Field<int>* useTime;
    TEFMod::Field<int>* useAnimation;
    TEFMod::Field<int>* damage;
    TEFMod::Field<int>* createTile;
    TEFMod::Field<int>* createWall;
} g_item;

void (*original_SetDefaults)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance);
void SetDefaults_T(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance);
inline TEFMod::HookTemplate HookTemplate_SetDefaults {
    reinterpret_cast<void*>(SetDefaults_T),
    {  }
};

void SetDefaults_T(TEFMod::TerrariaInstance i, int t, bool n, TEFMod::TerrariaInstance v) {
    original_SetDefaults(i, t, n, v);
    for (const auto fun: HookTemplate_SetDefaults.FunctionArray) {
        if(fun) reinterpret_cast<void(*)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance)>(fun)(i, t, n, v);
    }
}

void Hook_SetDefaults(TEFMod::TerrariaInstance item, int type, bool noMatCheck, TEFMod::TerrariaInstance variant) {
    const bool shouldOverclock = (
        g_item.damage->Get(item) > 0 ||
        g_item.createTile->Get(item) > 0 ||
        g_item.createWall->Get(item) > 0 ||
        type == 1291 ||
        type == 29 ||
        type == 109
    );

    if (shouldOverclock) {
        g_log->d("加速物品 id=", type);
        g_item.useTime->Set(1, item);     // 设置为1帧使用时间
        g_item.useAnimation->Set(5, item); // 设置为5帧动画
    }
}

class Overclock final : public EFMod {
public:

    int Initialize(const std::string &path, MultiChannel *multiChannel) override {return 0;}
    int UnLoad(const std::string &path, MultiChannel *multiChannel) override {return 0;}

    int Load(const std::string &path, MultiChannel* channel) override {
        g_api = channel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        g_log = channel->receive<TEFMod::Logger*(*)(const std::string& Tag, const std::string& filePath, const std::size_t)>("TEFMod::CreateLogger")("Overclock-EternalFuture゙", "", 0);

        g_log->init();

        return 0;
    }

    void Send(const std::string &path, MultiChannel* channel) override {
        g_api->registerFunctionDescriptor({
            "Terraria", "Item", "SetDefaults", "hook>>void", 3,
            &HookTemplate_SetDefaults,
            { (void*)Hook_SetDefaults }
        });

        const char* fields[] = {
            "useTime", "useAnimation", "damage",
            "createTile", "createWall"
        };

        for (auto& name : fields) {
            g_api->registerApiDescriptor({"Terraria", "Item", name, "Field"});
        }
    }

    void Receive(const std::string &path, MultiChannel* channel) override {
        const auto ParseIntField = channel->receive<TEFMod::Field<int>*(*)(void*)>(
            "TEFMod::Field<Int>::ParseFromPointer");

        // 初始化字段
        g_item.useTime = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "useTime", "Field"}));
        g_item.useAnimation = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "useAnimation", "Field"}));
        g_item.damage = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "damage", "Field"}));
        g_item.createTile = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "createTile", "Field"}));
        g_item.createWall = ParseIntField(g_api->GetAPI<void*>(
                    {"Terraria", "Item", "createWall", "Field"}));
        original_SetDefaults = g_api->GetAPI<void(*)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance)>(
            {"Terraria", "Item", "SetDefaults", "old_fun", 3});
    }

    Metadata GetMetadata() override {
        return {
            "Overclock",
            "EternalFuture゙",
            "1.1.1",
            20250517,
            ModuleType::Game,
            { false }
        };
    }
};

EFMod* CreateMod() {
    static Overclock instance;
    return &instance;
}