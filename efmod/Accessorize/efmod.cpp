//
// Created by EternalFuture゙ on 2025/3/23.
//

#include "efmod_core.hpp"
#include "TEFMod.hpp"
#include "Logger.hpp"
#include "BaseType.hpp"

TEFMod::Logger* g_log;
TEFMod::TEFModAPI* g_api;

struct ItemFields {
    TEFMod::Field<int>* headSlot;
    TEFMod::Field<int>* bodySlot;
    TEFMod::Field<int>* legSlot;
    TEFMod::Field<bool>* accessory;
} g_item;

void (*original_SetDefaults)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance);
void SetDefaults_T(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance);
inline TEFMod::HookTemplate T_SetDefaults {
    reinterpret_cast<void*>(SetDefaults_T),
    {  }
};

void SetDefaults_T(TEFMod::TerrariaInstance i, int t, bool n, TEFMod::TerrariaInstance v) {
    original_SetDefaults(i, t, n, v);
    for (auto fun: T_SetDefaults.FunctionArray) {
        if(fun) reinterpret_cast<void(*)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance)>(fun)(i, t, n, v);
    }
}

void Hook_SetDefaults(TEFMod::TerrariaInstance self, int type, bool noMatCheck, TEFMod::TerrariaInstance variant) {
    if (g_item.headSlot->Get(self) >= 0 || g_item.bodySlot->Get(self) >= 0 || g_item.legSlot->Get(self) >= 0) {
        g_item.accessory->Set(true, self);
        g_log->i("将物品[", type, "]标记为饰品");
    }
}

class Accessorize final : public EFMod {
public:

    int Initialize(const std::string &path, MultiChannel *multiChannel) override { return 0; }

    int Load(const std::string &path, MultiChannel* channel) override {
        g_api = channel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        g_log = channel->receive<TEFMod::Logger*(*)(const std::string& Tag, const std::string& filePath, const std::size_t)>("TEFMod::CreateLogger")("Accessorize-EternalFuture゙", "", 0);

        g_log->init();

        return 0;
    }

    int UnLoad(const std::string &path, MultiChannel *multiChannel) override { return 0; }

    void Send(const std::string &path, MultiChannel* channel) override {
        channel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI")->registerFunctionDescriptor({
            "Terraria", "Item", "SetDefaults", "hook>>void", 3, &T_SetDefaults, { reinterpret_cast<void*>(Hook_SetDefaults) }
        });

        const char* fields[] = {"headSlot", "bodySlot", "legSlot", "accessory"};
        for (auto& name : fields) {
            channel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI")->registerApiDescriptor({"Terraria", "Item", name, "Field"});
        }
    }

    void Receive(const std::string &path, MultiChannel* channel) override {
        const auto ParseIntField = channel->receive<TEFMod::Field<int>*(*)(void*)>(
            "TEFMod::Field<Int>::ParseFromPointer");
        const auto ParseBoolField = channel->receive<TEFMod::Field<bool>*(*)(void*)>(
            "TEFMod::Field<Bool>::ParseFromPointer");

        g_item.headSlot = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "headSlot", "Field"}));
        g_item.bodySlot = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "bodySlot", "Field"}));
        g_item.legSlot = ParseIntField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "legSlot", "Field"}));
        g_item.accessory = ParseBoolField(g_api->GetAPI<void*>(
            {"Terraria", "Item", "accessory", "Field"}));

        original_SetDefaults = g_api->GetAPI<void(*)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance)>(
            {"Terraria", "Item", "SetDefaults", "old_fun", 3});
    }

    Metadata GetMetadata() override {
        return {
            "Accessorizec",
                "EternalFuture゙",
                "1.1.0",
                20250517,
                ModuleType::Game,
                {
                    false
                }
        };
    }
};

EFMod* CreateMod() {
    static Accessorize instance;
    return &instance;
}