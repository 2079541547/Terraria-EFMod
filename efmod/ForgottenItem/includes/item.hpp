/*******************************************************************************
 * 文件名称: item
 * 项目名称: ForgottenItem
 * 创建时间: 25-5-24
 * 作者: EternalFuture゙
 * Gitlab: https://github.com/2079541547/
 * 协议: Apache License 2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#pragma once

#include "TEFMod.hpp"
#include "BaseType.hpp"

namespace Item {

    inline TEFMod::Field<int>* useStyle;
    inline TEFMod::Field<int>* useAnimation;
    inline TEFMod::Field<int>* useTime;
    inline TEFMod::Field<int>* width;
    inline TEFMod::Field<int>* height;
    inline TEFMod::Field<int>* shoot;
    inline TEFMod::Field<int>* useAmmo;
    inline TEFMod::Field<int>* damage;
    inline TEFMod::Field<float>* shootSpeed;
    inline TEFMod::Field<bool>* noMelee;
    inline TEFMod::Field<int>* value;
    inline TEFMod::Field<bool>* ranged;
    inline TEFMod::Field<bool>* channel;
    inline TEFMod::Field<int>* rare;
    inline TEFMod::Field<bool>* autoReuse;
    inline TEFMod::Field<int>* mana;
    inline TEFMod::Field<bool>* magic;
    inline TEFMod::Field<bool>* noUseGraphic;

    inline TEFMod::Method<void>* DefaultToBow; //int singleShotTime, float shotVelocity, bool hasAutoReuse = false
    inline TEFMod::Method<void>* SetWeaponValues; //int dmg, float knockback, int bonusCritChance = 0
    inline TEFMod::Method<void>* DefaultToFood; //int newwidth, int newheight, int foodbuff, int foodbuffduration, bool useGulpSound = false, int animationTime = 17
    inline TEFMod::Method<int>* buyPrice; //int platinum = 0, int gold = 0, int silver = 0, int copper = 0

    inline void (*original_SetDefaults)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance);
    void Hook_SetDefaults(TEFMod::TerrariaInstance item, int type, bool noMatCheck, TEFMod::TerrariaInstance variant);

    void SetDefaults_T(TEFMod::TerrariaInstance i, int t, bool n, TEFMod::TerrariaInstance v);

    inline TEFMod::HookTemplate HookTemplate_SetDefaults {
        reinterpret_cast<void*>(SetDefaults_T),
        {  }
    };

    void init(TEFMod::TEFModAPI* api);
}