/*******************************************************************************
* 文件名称: prefix_legacy
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

#include "prefix_legacy.hpp"

#include "logger.hpp"
#include "utility.hpp"

void PrefixLegacy::cctor_HookT(TEFMod::TerrariaInstance i) {
    logger->d("Entering cctor_HookT");
    logger->d("TerrariaInstance: ", reinterpret_cast<uintptr_t>(&i));

    logger->i("Calling original .cctor");
    original_cctor(i);
    logger->i("Original .cctor completed");

    logger->d("HookTemplate_cctor contains ", HookTemplate_cctor.FunctionArray.size(), " hook functions");
    for (const auto fun: HookTemplate_cctor.FunctionArray) {
        logger->d("Calling hook function: ", reinterpret_cast<uintptr_t>(fun));
        reinterpret_cast<void(*)(TEFMod::TerrariaInstance)>(fun)(i);
        logger->d("Hook function completed");
    }

    logger->d("Exiting cctor_HookT");
}

void PrefixLegacy::hook_cctor(TEFMod::TerrariaInstance i) {
    const auto SwordsHammersAxesPicks_array = ParseBoolArray(SwordsHammersAxesPicks->Get());
    const auto GunsBows_array = ParseBoolArray(GunsBows->Get());
    const auto MagicAndSummon_array = ParseBoolArray(MagicAndSummon->Get());

    SwordsHammersAxesPicks_array->set(4722, true); // 最初分形
    MagicAndSummon_array->set(2881, true);         // 相位扭曲器
    GunsBows_array->set(3850, true);               // 埃特尼亚标枪
    GunsBows_array->set(3853, true);               // 无趣弓
    GunsBows_array->set(4058, true);               // 骷髅头弓


}

void PrefixLegacy::init(TEFMod::TEFModAPI *api) {
    static bool inited = false;
    if (!inited) {
        const char* fields[] = {
            "SwordsHammersAxesPicks",
            "GunsBows",
            "MagicAndSummon"
        };

        for (const auto field: fields) {
            api->registerApiDescriptor({
            "Terraria.GameContent.Prefixes",
            "PrefixLegacy.ItemSets",
            field,
            "Field"
            });
        }

        api->registerFunctionDescriptor({
            "Terraria.GameContent.Prefixes",
            "PrefixLegacy.ItemSets",
            ".cctor",
            "hook>>void",
            0,
            &HookTemplate_cctor,
            { reinterpret_cast<void*>(hook_cctor) }
        });

        inited = true;
    } else {
        original_cctor = api->GetAPI<decltype(original_cctor)>({
            "Terraria.GameContent.Prefixes",
            "PrefixLegacy.ItemSets",
            ".cctor",
            "old_fun",
            0
        });

        SwordsHammersAxesPicks = ParseOtherField(api->GetAPI<void*>({
            "Terraria.GameContent.Prefixes",
            "PrefixLegacy.ItemSets",
            "SwordsHammersAxesPicks",
            "Field"
        }));

        GunsBows = ParseOtherField(api->GetAPI<void*>({
            "Terraria.GameContent.Prefixes",
            "PrefixLegacy.ItemSets",
            "GunsBows",
            "Field"
        }));

        MagicAndSummon = ParseOtherField(api->GetAPI<void*>({
            "Terraria.GameContent.Prefixes",
            "PrefixLegacy.ItemSets",
            "MagicAndSummon",
            "Field"
        }));
    }
}
