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

#include "item.hpp"

#include "logger.hpp"
#include "recipe.hpp"
#include "utility.hpp"

void Item::Hook_SetDefaults(TEFMod::TerrariaInstance item, int type, bool noMatCheck,
    TEFMod::TerrariaInstance variant) {

    switch (type) {

        case 2881: {
            useStyle->Set(5, item);
            useAnimation->Set(20, item);
            useTime->Set(80, item);
            shootSpeed->Set(500.0f, item);
            width->Set(20, item);
            height->Set(12, item);
            damage->Set(550, item);
            shoot->Set(79, item);
            mana->Set(100, item);
            rare->Set(8, item);
            value->Set(buyPrice->Call(nullptr, 4, 0, 10, 0, 0), item);
            noMelee->Set(true, item);
            channel->Set(true, item);
            magic->Set(true, item);
        } break;

        case 3850: {
            useStyle->Set(1, item);
            shootSpeed->Set(55.5f, item);
            shoot->Set(680, item);
            damage->Set(71, item);
            width->Set(0, item);
            height->Set(30, item);
            useAnimation->Set(17, item);
            useTime->Set(17, item);
            noUseGraphic->Set(true, item);
            noMelee->Set(true, item);
            value->Set(buyPrice->Call(nullptr, 4, 0, 5, 0, 0), item);
            ranged->Set(true, item);
            rare->Set(6, item);
        } break;

        case 3853: {
            useStyle->Set(5, item);
            useAnimation->Set(25, item);
            useAmmo->Set(40, item);
            width->Set(20, item);
            height->Set(34, item);
            shoot->Set(1, item);
            damage->Set(72, item);
            shootSpeed->Set(50.0f, item);
            noMelee->Set(true, item);
            value->Set(180000, item);
            ranged->Set(true, item);
            useTime->Set(12, item);
            channel->Set(true, item);
            rare->Set(6, item);
            autoReuse->Set(true, item);
        } break;

        case 4058: {
            damage->Set(40, item);
            SetWeaponValues->Call(item, 3, 30, 5.0f, 0);
        } break;


        case 4722: {
            noMelee->Set(false, item);
            damage->Set(130, item);
        } break;


        case 4010: {
            DefaultToFood->Call(item, 6, 22, 22, 207, 14400, false, 17);
            rare->Set(3, item);
            value->Set(buyPrice->Call(nullptr, 4, 0, 0, 70, 5), item);
        } break;

        default:
            break;
    }


}

void Item::SetDefaults_T(TEFMod::TerrariaInstance i, int t, bool n, TEFMod::TerrariaInstance v) {
    original_SetDefaults(i, t, n, v);
    for (const auto fun: HookTemplate_SetDefaults.FunctionArray) {
        if(fun) reinterpret_cast<void(*)(TEFMod::TerrariaInstance, int, bool, TEFMod::TerrariaInstance)>(fun)(i, t, n, v);
    }
}

void Item::init(TEFMod::TEFModAPI *api) {
    static bool inited = false;


    static const char* fields[] = { "useStyle", "useAnimation", "useTime", "width", "height", "shoot", "useAmmo", "damage",
                   "shootSpeed", "noMelee", "value", "ranged", "channel", "rare", "autoReuse", "mana", "magic",
                   "noUseGraphic"
    };

    static std::unordered_map<std::string, int> methods = {
        { "DefaultToBow", 3  },
        { "SetWeaponValues", 3 },
        { "DefaultToFood", 6 },
        { "buyPrice", 4 }
    };


    if (!inited) {

        api->registerFunctionDescriptor({
           "Terraria", "Item", "SetDefaults", "hook>>void", 3,
           &HookTemplate_SetDefaults,
           { reinterpret_cast<void*>(Hook_SetDefaults) }
       });

        for (auto& name : fields) {
            TEFMod::ModApiDescriptor fieldDesc = {
                "Terraria",
                "Item",
                name,
                "Field"
            };

            logger->d("Registering field: ", name);
            api->registerApiDescriptor(fieldDesc);
            logger->d("Field registered: ", name);
        }

        for (const auto& method: methods) {
            TEFMod::ModApiDescriptor fieldDesc = {
                "Terraria",
                "Item",
                method.first,
                "Method",
                method.second
            };

            logger->d("Registering mield: ", method.first);
            api->registerApiDescriptor(fieldDesc);
            logger->d("Field registered: ", method.first);
        }

        inited = true;
    } else {
        original_SetDefaults = api->GetAPI<decltype(original_SetDefaults)>({"Terraria", "Item", "SetDefaults", "old_fun", 3});

        DefaultToBow = ParseVoidMethod(api->GetAPI<void*>({"Terraria", "Item", "DefaultToBow", "Method", 3})); // int, float, bool
        SetWeaponValues = ParseVoidMethod(api->GetAPI<void*>({"Terraria", "Item", "SetWeaponValues", "Method", 3})); // int, float, int
        DefaultToFood = ParseVoidMethod(api->GetAPI<void*>({"Terraria", "Item", "DefaultToFood", "Method", 6})); // int, int, int, int, bool, int
        buyPrice = ParseIntMethod(api->GetAPI<void*>({"Terraria", "Item", "buyPrice", "Method", 4}));

        useStyle = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "useStyle", "Field"}));
        useAnimation = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "useAnimation", "Field"}));
        useTime = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "useTime", "Field"}));
        width = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "width", "Field"}));
        height = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "height", "Field"}));
        shoot = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "shoot", "Field"}));
        useAmmo = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "useAmmo", "Field"}));
        damage = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "damage", "Field"}));
        shootSpeed = ParseFloatField(api->GetAPI<void*>({"Terraria", "Item", "shootSpeed", "Field"}));
        noMelee = ParseBoolField(api->GetAPI<void*>({"Terraria", "Item", "noMelee", "Field"}));
        value = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "value", "Field"}));
        ranged = ParseBoolField(api->GetAPI<void*>({"Terraria", "Item", "ranged", "Field"}));
        channel = ParseBoolField(api->GetAPI<void*>({"Terraria", "Item", "channel", "Field"}));
        rare = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "rare", "Field"}));
        autoReuse = ParseBoolField(api->GetAPI<void*>({"Terraria", "Item", "autoReuse", "Field"}));
        mana = ParseIntField(api->GetAPI<void*>({"Terraria", "Item", "mana", "Field"}));
        magic = ParseBoolField(api->GetAPI<void*>({"Terraria", "Item", "magic", "Field"}));
        noUseGraphic = ParseBoolField(api->GetAPI<void*>({"Terraria", "Item", "noUseGraphic", "Field"}));
    }
}
