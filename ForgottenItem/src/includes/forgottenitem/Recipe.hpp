//
// Created by EternalFuture on 2025/1/25.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>
#include <BNM/Method.hpp>

namespace Recipe {
    inline BNM::Method<void> ItemSetDefaults;
    inline BNM::Method<void> AddRecipe;
    inline BNM::Class Item_Class;
    inline BNM::Class Recipe_Class;
    inline BNM::Field<int> stack;
    inline BNM::Field<BNM::UnityEngine::Object*> createItem;
    inline BNM::Field<BNM::UnityEngine::Object*> currentRecipe;
    inline BNM::Field<BNM::Structures::Mono::Array<BNM::UnityEngine::Object*>> requiredItem;
    inline BNM::Field<BNM::Structures::Mono::Array<int>> requiredTile;

    void initialize();

    inline void (*old_SetupRecipeGroups)(BNM::UnityEngine::Object*);
    void SetupRecipeGroups(BNM::UnityEngine::Object* i);
}