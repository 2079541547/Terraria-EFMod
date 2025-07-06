/*******************************************************************************
 * 文件名称: item_id
 * 项目名称: ForgottenItem
 * 创建时间: 25-5-18
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

#include "item_id.hpp"

#include "logger.hpp"
#include "utility.hpp"

#include <filesystem>

void ItemID::cctor_HookT(TEFMod::TerrariaInstance i) {
    logger->d("Entering cctor_HookT");
    logger->d("TerrariaInstance: ", reinterpret_cast<uintptr_t>(&i));

    logger->i("Calling original .cctor");
    original_cctor(i);
    logger->i("Original .cctor completed");

    logger->d("HookTemplate_cctor contains ", HookTemplate_cctor.FunctionArray.size(), " hook functions");
    for (const auto fun : HookTemplate_cctor.FunctionArray) {
        logger->d("Calling hook function: ", reinterpret_cast<uintptr_t>(fun));
        reinterpret_cast<void(*)(TEFMod::TerrariaInstance)>(fun)(i);
        logger->d("Hook function completed");
    }

    logger->d("Exiting cctor_HookT");
}

void ItemID::hook_cctor(TEFMod::TerrariaInstance i) {
    if (auto* it = ParseBoolArray(Deprecated->Get())) {
        it->fill(false);
    }

    if (auto *it = ParseBoolArray(ItemsThatShouldNotBeInInventory->Get())) {
        it->fill(false);
    }

    logger->i("已解除禁装限制");

    if (auto *it = ParseIntArray(ShimmerTransformToItem->Get())) {
        // 拜月
        logger->d("Setting up 拜月 transformation: 3357 <-> 3331");
        it->set(3357, 3331);
        it->set(3331, 3357);

        // 食人魔
        logger->d("Setting up 食人魔 transformation: 3868 <-> 3861");
        it->set(3868, 3861);
        it->set(3861, 3868);

        // 黑暗魔法师
        logger->d("Setting up 黑暗魔法师 transformation: 3862 <-> 3867");
        it->set(3862, 3867);
        it->set(3867, 3862);

        // 火星飞碟
        logger->d("Setting up 火星飞碟 transformation: 2881 <-> 3358");
        it->set(2881, 3358);
        it->set(3358, 2881);

        // 食人魔面具
        logger->d("Setting up 食人魔面具 transformation: 3847 <-> 3865");
        it->set(3847, 3865);
        it->set(3865, 3847);

        // 无趣弓
        logger->d("Setting up 无趣弓 transformation: 3854 <-> 3853");
        it->set(3854, 3853);
        it->set(3853, 3854);

        // 邪教徒
        logger->d("Setting up 邪教徒 transformation: 2901 <-> 2989 and 2990 <-> 2902");
        it->set(2901, 2989);
        it->set(2989, 2901);
        it->set(2990, 2902);
        it->set(2902, 2990);

        // 毒孢旗
        logger->d("Setting up 毒孢旗 transformation: 3404 <-> 1649");
        it->set(3404, 1649);
        it->set(1649, 3404);

        // 残手旗
        logger->d("Setting up 残手旗 transformation: 3398 <-> 1648");
        it->set(3398, 1648);
        it->set(1648, 3398);

        // 地牢玩具
        logger->d("Setting up 地牢玩具 transformation: 1569 <-> 1571");
        it->set(1569, 1571);
        it->set(1571, 1569);

        logger->i("已设置转换关系");
    }


}

void ItemID::init(TEFMod::TEFModAPI *api) {
    static bool inited = false;

    logger->d("Entering ItemID::init");
    logger->d("API pointer: ", reinterpret_cast<uintptr_t>(api));
    logger->d("Current initialization status: ", inited ? "already initialized" : "not initialized");

    if (!inited) {
        logger->i("First time initialization started");

        // 注册 .cctor 钩子
        TEFMod::ModFuncDescriptor cctorDesc = {
            "Terraria.ID",
            "ItemID.Sets",
            ".cctor",
            "hook>>void",
            0,
            &HookTemplate_cctor,
            { reinterpret_cast<void*>(hook_cctor) }
        };

        logger->d("Registering .cctor hook with descriptor:");
        logger->d("- Namespace: ", cctorDesc.Namespace);
        logger->d("- Class: ", cctorDesc.Class);
        logger->d("- Method: ", cctorDesc.Name);

        api->registerFunctionDescriptor(cctorDesc);
        logger->i(".cctor hook registered successfully");

        // 注册字段
        const char* fields[] = {"ShimmerTransformToItem", "Deprecated", "ItemsThatShouldNotBeInInventory"};
        logger->d("Registering fields count: ", sizeof(fields)/sizeof(fields[0]));

        for (auto& name : fields) {
            TEFMod::ModApiDescriptor fieldDesc = {
                "Terraria.ID",
                "ItemID.Sets",
                name,
                "Field"
            };

            logger->d("Registering field: ", name);
            api->registerApiDescriptor(fieldDesc);
            logger->d("Field registered: ", name);
        }

        inited = true;
        logger->i("Initialization completed successfully");
    } else {
        logger->i("Subsequent initialization started");

        // 获取原始 .cctor 函数
        TEFMod::ModApiDescriptor originalDesc = {
            "Terraria.ID",
            "ItemID.Sets",
            ".cctor",
            "old_fun",
            0,
        };

        logger->d("Retrieving original .cctor function");
        original_cctor = api->GetAPI<void(*)(TEFMod::TerrariaInstance)>(originalDesc);
        logger->d("Original .cctor function pointer: ", reinterpret_cast<uintptr_t>(original_cctor));

        // 获取并解析各个字段
        logger->d("Starting to parse fields");

        // ShimmerTransformToItem
        TEFMod::ModApiDescriptor shimmerDesc = {
            "Terraria.ID",
            "ItemID.Sets",
            "ShimmerTransformToItem",
            "Field"
        };
        logger->d("Retrieving ShimmerTransformToItem field");
        void* shimmerField = api->GetAPI<void*>(shimmerDesc);
        logger->d("ShimmerTransformToItem raw pointer: ", reinterpret_cast<uintptr_t>(shimmerField));
        ShimmerTransformToItem = ParseOtherField(shimmerField);
        logger->d("ShimmerTransformToItem parsed successfully");

        // Deprecated
        TEFMod::ModApiDescriptor deprecatedDesc = {
            "Terraria.ID",
            "ItemID.Sets",
            "Deprecated",
            "Field"
        };
        logger->d("Retrieving Deprecated field");
        void* deprecatedField = api->GetAPI<void*>(deprecatedDesc);
        logger->d("Deprecated raw pointer: ", reinterpret_cast<uintptr_t>(deprecatedField));
        Deprecated = ParseOtherField(deprecatedField);
        logger->d("Deprecated parsed successfully");

        // ItemsThatShouldNotBeInInventory
        TEFMod::ModApiDescriptor itemsDesc = {
            "Terraria.ID",
            "ItemID.Sets",
            "ItemsThatShouldNotBeInInventory",
            "Field"
        };
        logger->d("Retrieving ItemsThatShouldNotBeInInventory field");
        auto itemsField = api->GetAPI<void*>(itemsDesc);
        logger->d("ItemsThatShouldNotBeInInventory raw pointer: ", reinterpret_cast<uintptr_t>(itemsField));
        ItemsThatShouldNotBeInInventory = ParseOtherField(itemsField);
        logger->d("ItemsThatShouldNotBeInInventory parsed successfully");

        logger->i("All fields parsed successfully");
    }

    logger->d("Exiting ItemID::init");
}