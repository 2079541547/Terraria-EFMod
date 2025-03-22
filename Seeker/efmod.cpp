//
// Created by EternalFuture゙ on 2025/3/22.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>

#include <android/log.h>

#include "EFMod.hpp"

BNM::Field<int>* aiStyle;
BNM::Field<bool>* friendly;

void SetDefaults(BNM::UnityEngine::Object* i, int t) {
    aiStyle->SetInstance(i);
    friendly->SetInstance(i);
    if (friendly->Get()) {
        aiStyle->Set(9);
    }
}

void (*old_SetDefaults)(BNM::UnityEngine::Object*, int);
void SetDefaults_T(BNM::UnityEngine::Object* i, int t);

inline HookTemplate T_SetDefaults {
        (void*) SetDefaults_T,
        {  }
};

void SetDefaults_T(BNM::UnityEngine::Object* i, int t) {
    ((void(*)(BNM::UnityEngine::Object*, int))old_SetDefaults)(i, t);
    for (auto fun: T_SetDefaults.FunArray) {
        if(fun) ((void(*)(void*, int))fun)(i, t);
    }
}


class Seeker: public EFMod {

public:

    int initialize(EFModAPI *mod, std::filesystem::path path) override {

        old_SetDefaults = (void(*)(BNM::UnityEngine::Object*, int))mod->getAPI({
                                              "Assembly-CSharp.dll",
                                              "Terraria",
                                              "Projectile",
                                              "SetDefaults",
                                              "old_fun",
                                              1
        });

        aiStyle = static_cast<BNM::Field<int> *>(mod->getAPI({"Assembly-CSharp.dll",
                                                              "Terraria",
                                                              "Projectile",
                                                              "aiStyle",
                                                              "Field"}));

        friendly = static_cast<BNM::Field<bool> *>(mod->getAPI({"Assembly-CSharp.dll",
                                                                "Terraria",
                                                                "Projectile",
                                                                "friendly",
                                                                "Field"}));

        return 0;
    }

    void Register(EFModAPI *mod, std::filesystem::path path) override {
        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Projectile",
            "aiStyle",
            "Field"
        });

        mod->registerModApiDescriptor({"Assembly-CSharp.dll",
                                       "Terraria",
                                       "Projectile",
                                       "friendly",
                                       "Field"
        });

        mod->registerModFuncDescriptor({
                                               "Assembly-CSharp.dll",
                                               "Terraria",
                                               "Projectile",
                                               "SetDefaults",
                                               "hook>>void",
                                               1,
                                               &T_SetDefaults,
                                               { (void*)SetDefaults }
        });
    }

    ModMetadata getInfo() override {
        return {
            "Seeker",
            "EternalFuture゙",
            "1.0.0"
        };
    }
};

EFMod* CreateMod() {
    static Seeker Mod;
    return &Mod;
}