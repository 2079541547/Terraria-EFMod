#pragma once

#include "EFMod.hpp"
#include "json.hpp"
#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>

class LuckyDamage {
private:
    inline static uint8_t mode;
    inline static void* old_DamageVar;
    inline static EFModAPI* API;
public:
    LuckyDamage(EFModAPI* e, const nlohmann::json& j);
    static int DamageVar(BNM::UnityEngine::Object* i, float dmg, float luck = 0);
};