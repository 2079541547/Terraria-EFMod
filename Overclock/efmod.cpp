//
// Created by EternalFuture゙ on 2025/3/22.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>

#include <android/log.h>

#include "EFMod.hpp"

BNM::Field<int>* useTime;
BNM::Field<int>* useAnimation;

void SetDefaults(BNM::UnityEngine::Object* i, int t) {
    useTime->SetInstance(i);
    useTime->Set(1);
    useAnimation->SetInstance(i);
    useAnimation->Set(10);
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


class Overclock: public EFMod {

public:

    int initialize(EFModAPI *mod, std::filesystem::path path) override {

        old_SetDefaults = (void(*)(BNM::UnityEngine::Object*, int))mod->getAPI({
                                              "Assembly-CSharp.dll",
                                              "Terraria",
                                              "Item",
                                              "SetDefaults",
                                              "old_fun",
                                              1
        });

        useTime = static_cast<BNM::Field<int>*>(mod->getAPI({"Assembly-CSharp.dll",
                                                              "Terraria",
                                                              "Item",
                                                              "useTime",
                                                              "Field"}));

        useAnimation = static_cast<BNM::Field<int> *>(mod->getAPI({"Assembly-CSharp.dll",
                                                                   "Terraria",
                                                                   "Item",
                                                                   "useAnimation",
                                                                   "Field"}));

        return 0;
    }

    void Register(EFModAPI *mod, std::filesystem::path path) override {
        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            "useTime",
            "Field"
        });

        mod->registerModApiDescriptor({"Assembly-CSharp.dll",
                                       "Terraria",
                                       "Item",
                                       "useAnimation",
                                       "Field"});

        mod->registerModFuncDescriptor({
                                               "Assembly-CSharp.dll",
                                               "Terraria",
                                               "Item",
                                               "SetDefaults",
                                               "hook>>void",
                                               1,
                                               &T_SetDefaults,
                                               { (void*)SetDefaults }
        });
    }

    ModMetadata getInfo() override {
        return {
            "Overclock",
            "EternalFuture゙",
            "1.0.0"
        };
    }
};

EFMod* CreateMod() {
    static Overclock Mod;
    return &Mod;
}