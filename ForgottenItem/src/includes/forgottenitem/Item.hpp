//
// Created by EternalFuture on 2025/1/25.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>

namespace Item {

    inline BNM::Class Item_Class;

    inline BNM::Field<int> useStyle;
    inline BNM::Field<int> useAnimation;
    inline BNM::Field<int> useTime;
    inline BNM::Field<int> width;
    inline BNM::Field<int> height;
    inline BNM::Field<int> shoot;
    inline BNM::Field<int> useAmmo;
    inline BNM::Field<int> damage;
    inline BNM::Field<float> shootSpeed;
    inline BNM::Field<bool> noMelee;
    inline BNM::Field<int> value;
    inline BNM::Field<bool> ranged;
    inline BNM::Field<bool> channel;
    inline BNM::Field<int> rare;
    inline BNM::Field<bool> autoReuse;
    inline BNM::Field<int> mana;
    inline BNM::Field<bool> magic;
    inline BNM::Field<bool> noUseGraphic;

    inline BNM::Method<void> DefaultToBow; //int singleShotTime, float shotVelocity, bool hasAutoReuse = false
    inline BNM::Method<void> SetWeaponValues; //int dmg, float knockback, int bonusCritChance = 0
    inline BNM::Method<void> DefaultToFood; //int newwidth, int newheight, int foodbuff, int foodbuffduration, bool useGulpSound = false, int animationTime = 17
    inline BNM::Method<int> buyPrice; //int platinum = 0, int gold = 0, int silver = 0, int copper = 0

    void initialize();

    inline void (*old_SetDefaults3)(BNM::UnityEngine::Object*,int);
    void SetDefaults3(BNM::UnityEngine::Object* i, int t);
    inline void (*old_SetDefaults4)(BNM::UnityEngine::Object*, int);
    void SetDefaults4(BNM::UnityEngine::Object* i, int t);
    inline void (*old_SetDefaults5)(BNM::UnityEngine::Object*, int);
    void SetDefaults5(BNM::UnityEngine::Object* i, int t);
    inline void(*old_SetFoodDefaults)(BNM::UnityEngine::Object*, int);
    void SetFoodDefaults(BNM::UnityEngine::Object* i, int t);
}

