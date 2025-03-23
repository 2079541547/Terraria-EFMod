//
// Created by EternalFuture゙ on 2025/3/23.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>

#include <android/log.h>

#include "EFMod.hpp"

BNM::Field<int>* headSlot;
BNM::Field<int>* bodySlot;
BNM::Field<int>* legSlot;
BNM::Field<bool>* accessory;

void SetDefaults(BNM::UnityEngine::Object* i, int t) {
    headSlot->SetInstance(i);
    bodySlot->SetInstance(i);
    legSlot->SetInstance(i);
    if (headSlot->Get() >= 0 || bodySlot->Get() >= 0 || legSlot->Get() >= 0) {
        accessory->SetInstance(i);
        accessory->Set(true);
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


class Accessorize: public EFMod {

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

        legSlot = static_cast<BNM::Field<int>*>(mod->getAPI({"Assembly-CSharp.dll",
                                                              "Terraria",
                                                              "Item",
                                                              "legSlot",
                                                              "Field"}));

        bodySlot = static_cast<BNM::Field<int> *>(mod->getAPI({"Assembly-CSharp.dll",
                                                                   "Terraria",
                                                                   "Item",
                                                                   "bodySlot",
                                                                   "Field"}));

        headSlot = static_cast<BNM::Field<int> *>(mod->getAPI({"Assembly-CSharp.dll",
                                                               "Terraria",
                                                               "Item",
                                                               "headSlot",
                                                               "Field"}));

        accessory = static_cast<BNM::Field<bool> *>(mod->getAPI({"Assembly-CSharp.dll",
                                                                "Terraria",
                                                                "Item",
                                                                "accessory",
                                                                "Field"}));

        return 0;
    }

    void Register(EFModAPI *mod, std::filesystem::path path) override {

        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            "bodySlot",
            "Field"
        });

        mod->registerModApiDescriptor({"Assembly-CSharp.dll",
                                       "Terraria",
                                       "Item",
                                       "legSlot",
                                       "Field"});

        mod->registerModApiDescriptor({"Assembly-CSharp.dll",
                                       "Terraria",
                                       "Item",
                                       "headSlot",
                                       "Field"});

        mod->registerModApiDescriptor({"Assembly-CSharp.dll",
                                       "Terraria",
                                       "Item",
                                       "accessory",
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
            "Accessorize",
            "EternalFuture゙",
            "1.0.0"
        };
    }
};

EFMod* CreateMod() {
    static Accessorize Mod;
    return &Mod;
}