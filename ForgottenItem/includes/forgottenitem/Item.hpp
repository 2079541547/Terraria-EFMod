//
// Created by EternalFuture on 2025/1/25.
//

#pragma once

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>
#include "EFMod.hpp"

namespace Item {

    inline BNM::Field<int>* useStyle;
    inline BNM::Field<int>* useAnimation;
    inline BNM::Field<int>* useTime;
    inline BNM::Field<int>* width;
    inline BNM::Field<int>* height;
    inline BNM::Field<int>* shoot;
    inline BNM::Field<int>* useAmmo;
    inline BNM::Field<int>* damage;
    inline BNM::Field<float>* shootSpeed;
    inline BNM::Field<bool>* noMelee;
    inline BNM::Field<int>* value;
    inline BNM::Field<bool>* ranged;
    inline BNM::Field<bool>* channel;
    inline BNM::Field<int>* rare;
    inline BNM::Field<bool>* autoReuse;
    inline BNM::Field<int>* mana;
    inline BNM::Field<bool>* magic;
    inline BNM::Field<bool>* noUseGraphic;

    inline BNM::Method<void>* DefaultToBow; //int singleShotTime, float shotVelocity, bool hasAutoReuse = false
    inline BNM::Method<void>* SetWeaponValues; //int dmg, float knockback, int bonusCritChance = 0
    inline BNM::Method<void>* DefaultToFood; //int newwidth, int newheight, int foodbuff, int foodbuffduration, bool useGulpSound = false, int animationTime = 17
    inline BNM::Method<int>* buyPrice; //int platinum = 0, int gold = 0, int silver = 0, int copper = 0

    void SetDefaults3(void* instance, int t);
    void SetDefaults4(void* instance, int t);
    void SetDefaults5(void* instance, int t);
    void SetFoodDefaults(void* instance, int t);

    void SetDefaults3_Template(BNM::UnityEngine::Object* i, int t);
    inline void* old_SetDefaults3 = nullptr;

    void SetDefaults4_Template(BNM::UnityEngine::Object* i, int t);
    inline void* old_SetDefaults4 = nullptr;

    void SetDefaults5_Template(BNM::UnityEngine::Object* i, int t);
    inline void* old_SetDefaults5 = nullptr;

    void SetFoodDefaults_Template(BNM::UnityEngine::Object* i, int t);
    inline void* old_SetFoodDefaults = nullptr;


    void Register(EFModAPI* mod);
    void Initialize(EFModAPI* mod);

    inline std::unordered_map<std::string, int> Methods = {
            { "DefaultToBow", 3  },
            { "SetWeaponValues", 3 },
            { "DefaultToFood", 6 },
            { "buyPrice", 4 }
    };

    inline std::vector<std::string> Fields = {
            "useStyle",
            "useAnimation",
            "useTime",
            "width",
            "height",
            "shoot",
            "useAmmo",
            "damage",
            "shootSpeed",
            "noMelee",
            "value",
            "ranged",
            "channel",
            "rare",
            "autoReuse",
            "mana",
            "magic",
            "noUseGraphic"
    };

    inline HookTemplate T_SetDefaults3 {
            (void*) Item::SetDefaults3_Template,
            {  }
    };

    inline HookTemplate T_SetDefaults4 {
            (void*) Item::SetDefaults4_Template,
            {  }
    };

    inline HookTemplate T_SetDefaults5 {
            (void*) Item::SetDefaults5_Template,
            {  }
    };

    inline HookTemplate T_SetFoodDefaults {
            (void*) Item::SetFoodDefaults_Template,
            {  }
    };

    inline std::unordered_map<std::string, std::pair<std::pair<std::string, HookTemplate*>, std::pair<int, void*>>> Hooks = {
             { "SetDefaults3", {{ "hook>>void", &T_SetDefaults3 }, { 1, (void*)SetDefaults3 } } },
             { "SetDefaults4", { { "hook>>void", &T_SetDefaults4 }, { 1, (void*)SetDefaults4 } } },
             { "SetDefaults5", { { "hook>>void", &T_SetDefaults5 }, { 1, (void*)SetDefaults5 } } },
             { "SetFoodDefaults", { { "hook>>void", &T_SetFoodDefaults }, { 1, (void*)SetFoodDefaults } } },
    };
}

