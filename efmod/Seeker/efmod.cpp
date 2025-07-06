//
// Created by EternalFuture゙ on 2025/3/22.
//

#include "efmod_core.hpp"
#include "TEFMod.hpp"
#include "BaseType.hpp"
#include "Logger.hpp"

// 全局组件
TEFMod::Logger* g_log;
TEFMod::TEFModAPI* g_api;

// 弹幕字段
struct ProjectileFields {
    TEFMod::Field<int>* aiStyle;    // AI类型
    TEFMod::Field<bool>* friendly;  // 是否友好
    TEFMod::Field<bool>* minion;    // 是否为召唤物
    TEFMod::Field<bool>* sentry;    // 是否为哨兵
} g_proj;

// 原始函数
void (*original_SetDefaults)(TEFMod::TerrariaInstance, int);

void SetDefaults_T(TEFMod::TerrariaInstance i, int t);

TEFMod::HookTemplate HookTemplate_SetDefaults {
    (void*)SetDefaults_T,
    {}
};

void SetDefaults_T(TEFMod::TerrariaInstance i, int t)  {
    original_SetDefaults(i, t);
    for (auto fun: HookTemplate_SetDefaults.FunctionArray) {
        if(fun) ((void(*)(TEFMod::TerrariaInstance, int))fun)(i, t);
    }
}

void Hook_SetDefaults(TEFMod::TerrariaInstance proj, int type) {

    int currentStyle = g_proj.aiStyle->Get(proj);
    const bool isFriendly = g_proj.friendly->Get(proj);
    const bool isMinion = g_proj.minion->Get(proj);
    const bool isSentry = g_proj.sentry->Get(proj);

    const bool shouldModify = (
        isFriendly &&
        currentStyle != 7 &&
        currentStyle != 26 &&
        !isMinion &&
        !isSentry &&
        type != 933 &&
        currentStyle != 165 &&
        type != 927
    );

    if (shouldModify) {
        g_log->d("修正弹幕AI id=", type, " 原aiStyle=", currentStyle);
        g_proj.aiStyle->Set(9, proj); // 设置为追踪型AI
    }
}

class Seeker final : public EFMod {
public:

    int Initialize(const std::string &path, MultiChannel *multiChannel) override {return 0;}
    int UnLoad(const std::string &path, MultiChannel *multiChannel) override {return 0;}

    int Load(const std::string &path, MultiChannel* channel) override {
        g_log = channel->receive<TEFMod::Logger*(*)(const std::string& Tag, const std::string& filePath, const std::size_t)>("TEFMod::CreateLogger")("Seeker-EternalFuture゙", "", 0);
        g_api = channel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        g_log->init();
        g_log->i("弹幕优化Mod已加载");
        return 0;
    }

    void Send(const std::string &path, MultiChannel* channel) override {
        // 注册Hook和字段
        const char* fields[] = {"aiStyle", "friendly", "minion", "sentry"};
        for (auto& name : fields) {
            g_api->registerApiDescriptor({"Terraria", "Projectile", name, "Field"});
            g_log->d("已注册字段: ", name);
        }

        g_api->registerFunctionDescriptor({
            "Terraria", "Projectile", "SetDefaults", "hook>>void", 1,
            &HookTemplate_SetDefaults,
            { (void*)Hook_SetDefaults }
        });
    }

    void Receive(const std::string &path, MultiChannel* channel) override {
        // 获取字段解析器
        const auto ParseInt = channel->receive<TEFMod::Field<int>*(*)(void*)>(
            "TEFMod::Field<Int>::ParseFromPointer");
        const auto ParseBool = channel->receive<TEFMod::Field<bool>*(*)(void*)>(
            "TEFMod::Field<Bool>::ParseFromPointer");

        // 初始化字段
        g_proj.aiStyle = ParseInt(g_api->GetAPI<void*>(
            {"Terraria", "Projectile", "aiStyle", "Field"}));
        g_proj.friendly = ParseBool(g_api->GetAPI<void*>(
            {"Terraria", "Projectile", "friendly", "Field"}));
        g_proj.minion = ParseBool(g_api->GetAPI<void*>(
            {"Terraria", "Projectile", "minion", "Field"}));
        g_proj.sentry = ParseBool(g_api->GetAPI<void*>(
            {"Terraria", "Projectile", "sentry", "Field"}));

        // 获取原始函数
        original_SetDefaults = g_api->GetAPI<void(*)(TEFMod::TerrariaInstance proj, int type)>(
            {"Terraria", "Projectile", "SetDefaults", "old_fun", 1});

        g_log->i("已初始化 ", sizeof(ProjectileFields)/8, " 个关键字段");
    }

    Metadata GetMetadata() override {
        return {
            "Seeker",
            "EternalFuture゙",
            "1.2.0",
            20250517,
            ModuleType::Game,
            { false }
        };
    }
};

EFMod* CreateMod() {
    static Seeker instance;
    return &instance;
}