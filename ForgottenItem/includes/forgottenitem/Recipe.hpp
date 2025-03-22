//
// Created by EternalFuture on 2025/1/25.
//

#pragma once

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>
#include <BNM/Method.hpp>
#include "EFMod.hpp"

namespace Recipe {
    inline BNM::Method<void>* ItemSetDefaults;
    inline BNM::Method<void>* AddRecipe;
    inline BNM::Field<int>* stack;
    inline BNM::Field<BNM::UnityEngine::Object*>* createItem;
    inline BNM::Field<BNM::UnityEngine::Object*>* currentRecipe;
    inline BNM::Field<BNM::Structures::Mono::Array<BNM::UnityEngine::Object*>*>* requiredItem;
    inline BNM::Field<BNM::Structures::Mono::Array<int>*>* requiredTile;

    struct CraftMaterial {
        int itemID;
        int stack;
        explicit CraftMaterial(int id, int count = 1) : itemID(id), stack(count) {}
    };

    struct GameRecipe {
        int resultID;
        std::vector<CraftMaterial> materials;
        int craftStation;
        explicit GameRecipe(int res, std::initializer_list<CraftMaterial> mats, int station = 0)
                : resultID(res), materials(mats), craftStation(station) {}
    };

    void RegisterRecipe(const GameRecipe& recipe);

    void SetupRecipeGroups(void* i);
    inline void* old_SetupRecipeGroups = nullptr;

    void Template(BNM::UnityEngine::Object* i);
    inline HookTemplate T_SetupRecipeGroups = {
            (void*)Template,
            {}
    };

    inline std::vector<std::string> Fields = {
            "stack",
            "createItem",
            "currentRecipe",
            "requiredItem",
            "requiredTile"
    };
}