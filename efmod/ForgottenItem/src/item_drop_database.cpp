/*******************************************************************************
 * 文件名称: item_drop_database
 * 项目名称: ForgottenItem
 * 创建时间: 25-6-28
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

#include "item_drop_database.hpp"
#include "utility.hpp"

void ItemDropDatabase::Populate_HookT(TEFMod::TerrariaInstance i) {
    original_Populate(i);
    for (const auto fun: HookTemplate_Populate.FunctionArray) {
        if (fun) reinterpret_cast<decltype(original_Populate)>(fun)(i);
    }
}

void ItemDropDatabase::hook_Populate(TEFMod::TerrariaInstance i) {
    RegisterToNPC->Call(i, 2, 395, Common->Call(nullptr, 4, 2881, 50, 1, 1)); // 相位扭曲

    // 无趣弓
    RegisterToNPC->Call(i, 2, 564, Common->Call(nullptr, 4, 3853, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 565, Common->Call(nullptr, 4, 3853, 10, 1, 1));

    // 埃特尼亚标枪
    RegisterToNPC->Call(i, 2, 561, Common->Call(nullptr, 4, 3850, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 562, Common->Call(nullptr, 4, 3850, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 563, Common->Call(nullptr, 4, 3850, 10, 1, 1));

    // 哥布林面具
    RegisterToNPC->Call(i, 2, 552, Common->Call(nullptr, 4, 3848, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 553, Common->Call(nullptr, 4, 3848, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 554, Common->Call(nullptr, 4, 3848, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 555, Common->Call(nullptr, 4, 3848, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 556, Common->Call(nullptr, 4, 3848, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 557, Common->Call(nullptr, 4, 3848, 10, 1, 1));

    // 哥布林炸弹帽
    RegisterToNPC->Call(i, 2, 555, Common->Call(nullptr, 4, 3849, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 556, Common->Call(nullptr, 4, 3849, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 557, Common->Call(nullptr, 4, 3849, 10, 1, 1));

    // 小妖魔雷管背包
    RegisterToNPC->Call(i, 2, 572, Common->Call(nullptr, 4, 3851, 10, 1, 1));
    RegisterToNPC->Call(i, 2, 573, Common->Call(nullptr, 4, 3851, 10, 1, 1));
}

void ItemDropDatabase::init(TEFMod::TEFModAPI *api) {
    static bool inited = false;
    if (!inited) {
        api->registerApiDescriptor({
            "Terraria.GameContent.ItemDropRules",
            "ItemDropRule",
            "Common",
            "Method",
            4
        });

        api->registerApiDescriptor({
            "Terraria.GameContent.ItemDropRules",
            "ItemDropDatabase",
            "RegisterToNPC",
            "Method",
            2
        });

        api->registerFunctionDescriptor({
            "Terraria.GameContent.ItemDropRules",
            "ItemDropDatabase",
            "Populate",
            "hook>>void",
            0,
            &HookTemplate_Populate,
            { reinterpret_cast<void*>(hook_Populate) }
        });

        inited = true;
    } else {
        original_Populate = api->GetAPI<decltype(original_Populate)>({
            "Terraria.GameContent.ItemDropRules",
            "ItemDropDatabase",
            "Populate",
            "old_fun",
            0
        });

        Common = ParseOtherMethod(api->GetAPI<void*>({
            "Terraria.GameContent.ItemDropRules",
            "ItemDropRule",
            "Common",
            "Method",
            4
        }));

        RegisterToNPC = ParseOtherMethod(api->GetAPI<void*>({
            "Terraria.GameContent.ItemDropRules",
            "ItemDropDatabase",
            "RegisterToNPC",
            "Method",
            2
        }));
    }
}
