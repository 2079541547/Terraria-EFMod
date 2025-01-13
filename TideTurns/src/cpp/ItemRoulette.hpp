#pragma once

#include "BNM/UserSettings/GlobalSettings.hpp"
#include "EFMod.hpp"
#include "json.hpp"
#include "BNM/UnityStructures.hpp"
#include "BNM/Field.hpp"

class ItemRoulette {
private:
    inline static size_t number;
    inline static size_t index;
    inline static EFModAPI* API;
    inline static BNM::Field<BNM::Structures::Mono::Array<bool>>* Deprecated;
public:
    ItemRoulette(EFModAPI* e, const nlohmann::json& j);
    static void New_ItemIDSets(BNM::UnityEngine::Object* i);
};