/*******************************************************************************
 * 文件名称: recipe
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

#include "recipe.hpp"

#include <sstream>

#include "logger.hpp"
#include "utility.hpp"

void Recipe::AddRecipeGroup(const RecipeConfig &config) {
    {
   std::ostringstream oss;

    // 🏗️ 构建配方日志头部
    oss << "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
        << "┃ 🛠️ 开始配置配方组 → 产出物品ID: " << config.resultItemID << "\n"
        << "┃   所需工作台: " << config.requiredTileID
        << " | 材料数量: " << config.materials.size() << "\n"
        << "┃ 🔧 正在收集材料物品ID...\n";
        for (const auto& [itemID, stackSize] : config.materials) {
            materialItems.insert(itemID); // 将材料ID添加到集合
            oss << "┃    ├─ 已标记物品ID: " << itemID << " 为材料\n";
        }

    // 🔧 技术细节日志（T级）
    oss << "┃ 🔧 [技术细节] 正在获取配方实例指针...\n";
    auto recipePtr = currentRecipe->Get();
    oss << "┃    ├─ currentRecipe 地址: 0x" << std::hex << reinterpret_cast<uintptr_t>(recipePtr) << "\n";

    requiredItem->SetInstance(recipePtr);
    createItem->SetInstance(recipePtr);
    requiredTile->SetInstance(recipePtr);
    oss << "┃    └─ 实例设置完成\n";

    // 📦 解析材料数组
    oss << "┃ 🔧 [技术细节] 正在解析材料数组...\n";
    const auto requiredItem_v = ParseOtherArray(requiredItem->Get(currentRecipe->Get()));
    const auto requiredTile_v = ParseIntArray(requiredTile->Get(currentRecipe->Get()));
    oss << "┃    ├─ requiredItem 数组大小: " << std::dec << requiredItem_v->Size() << "\n"
        << "┃    └─ requiredTile 数组大小: " << requiredTile_v->Size() << "\n";

    // 🎯 设置产出物品
    auto createItemPtr = createItem->Get();
    oss << "┃ 🎯 设置产出物品 → ID: " << config.resultItemID << ", 数量: " << config.resultStack << "\n"
        << "┃    ├─ createItem 实例地址: 0x" << std::hex << reinterpret_cast<uintptr_t>(createItemPtr) << "\n";
    stack->Set(config.resultStack, createItemPtr);
    ItemSetDefaults->Call(createItemPtr, 1, config.resultItemID);
    oss << "┃    └─ 已调用 ItemSetDefaults\n";

    // ⚙️ 设置材料
    oss << "┃ ⚙️ 材料配置 → 需要 " << std::dec << config.materials.size() << " 种材料\n";
    for (size_t i = 0; i < config.materials.size(); ++i) {
        const auto& [itemID, stackSize] = config.materials[i];
        auto materialSlotPtr = requiredItem_v->at(i);

        oss << "┃    ├─ 材料" << i+1 << ": ID=" << itemID << " × " << stackSize << "\n"
            << "┃    │   ├─ 槽位地址: 0x" << std::hex << reinterpret_cast<uintptr_t>(materialSlotPtr) << "\n";

        ItemSetDefaults->Call(materialSlotPtr, 1, itemID);
        oss << "┃    │   ├─ 已设置基础属性\n";

        if (stackSize > 1) {
            stack->SetInstance(materialSlotPtr);
            stack->Set(stackSize);
            oss << "┃    │   └─ 已设置堆叠数量: " << std::dec << stackSize << "\n";
        }
    }

    // 🪚 设置工作台
    oss << "┃ 🪚 需要工作台 → ID: " << std::dec << config.requiredTileID << "\n"
        << "┃    ├─ 原始工作台值: " << requiredTile_v->at(0) << "\n";
    requiredTile_v->at(0) = config.requiredTileID;
    oss << "┃    └─ 新工作台值: " << requiredTile_v->at(0) << "\n";

    // 📝 提交配方
    oss << "┃ 🔧 [技术细节] 正在提交配方...\n";
    AddRecipe->Call(nullptr, 0, 0);
    oss << "┃ ✅ 配方提交完成\n"
        << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    logger->i(oss.str());
    }
}

void Recipe::SetupRecipeGroups(TEFMod::TerrariaInstance instance) {
    static std::unordered_map<int, std::vector<int>> LuminousTools = {
        {3458, {2785, 2783, 2782}},
        {3456, {2775, 2773, 2772}},
        {3457, {2780, 2778, 2777}},
        {3459, {3465, 3463, 3462}}
    };

    // 正确初始化配方列表
    const std::vector<RecipeConfig> allRecipes = {
        {4058, {{1274, 1}, {154, 50}}, 18},  // 骷髅头弓
        {3978, {{3385, 1}, {3386, 1}, {3387, 1}, {3388, 1}}, 228},
        {4010, {{4011, 1}}, 0, 4},  // 苹果派切片
        {4011, {{4010, 4}}},  // 苹果派
        {4722, {{757, 1}, {3827, 1}, {3787, 1}, {1570, 2}, {2880, 1}}, 134},
        {766, {{154, 1}}, 18},  // 骨块
        {3705, {{3886, 1}}},  // 假宝箱1
        {3706, {{3887, 1}}},  // 假宝箱2
        {5013, {}, 79}  // 睡觉图标
    };

    // 处理发光工具组
    for (const auto& [resultID, materials] : LuminousTools) {
        AddRecipeGroup({
            resultID,
            {{materials[0], 1}, {3467, 6}},
            412  // 恶魔祭坛
        });
    }

    // 处理旧礼物配方
    static std::vector giftV = {599, 600, 601};
    for (const int giftID : giftV) {
        AddRecipeGroup({giftID, {{1869, 1}}});
    }

    for (const auto& recipe : allRecipes) {
        AddRecipeGroup(recipe);
    }

    logger->i("🌈 所有配方组配置完成 (共" +
             std::to_string(LuminousTools.size() + allRecipes.size() + giftV.size()) +
             "个配方)");

    const auto is_a_material = ParseBoolArray(IsAMaterial->Get());
    for (const auto item: materialItems) {
        is_a_material->set(item, true);
    }

    is_a_material->clear();

    logger->i("已设置材料属性");
}

void Recipe::Template(const TEFMod::TerrariaInstance instance) {
    original_SetupRecipeGroups(instance);
    for (const auto fun : HookTemplate_SetupRecipeGroups.FunctionArray) {
        reinterpret_cast<void(*)(TEFMod::TerrariaInstance)>(fun)(instance);
    }
}

void Recipe::init(TEFMod::TEFModAPI *api) {
    static bool inited = false;

    if (!inited) {

        api->registerFunctionDescriptor({
            "Terraria",
            "Recipe",
            "SetupRecipeGroups",
            "hook>>void",
            0,
            &HookTemplate_SetupRecipeGroups,
            { reinterpret_cast<void*>(SetupRecipeGroups) }
        });

        const char* fields[] = {
            "createItem",
            "currentRecipe",
            "requiredItem",
            "requiredTile"
        };

        api->registerApiDescriptor({ "Terraria", "Item", "stack", "Field" });

        for (auto& name : fields) {
            TEFMod::ModApiDescriptor fieldDesc = {
                "Terraria",
            "Recipe",
            name,
            "Field"
            };

            logger->d("Registering field: ", name);
            api->registerApiDescriptor(fieldDesc);
            logger->d("Field registered: ", name);
        }

        api->registerApiDescriptor({"Terraria", "Recipe", "AddRecipe", "Method", 0});
        api->registerApiDescriptor({"Terraria", "Item", "SetDefaults", "Method", 1});

        api->registerApiDescriptor({"Terraria.ID", "ItemID.Sets", "IsAMaterial", "Field"});
        inited = true;
    } else {

        original_SetupRecipeGroups = api->GetAPI<decltype(original_SetupRecipeGroups)>({ "Terraria", "Recipe", "SetupRecipeGroups", "old_fun", 0 });

        AddRecipe = ParseVoidMethod(api->GetAPI<void*>({ "Terraria", "Recipe", "AddRecipe", "Method", 0 }));
        ItemSetDefaults = ParseVoidMethod(api->GetAPI<void*>({ "Terraria", "Item", "SetDefaults", "Method", 1 }));

        stack = ParseIntField(api->GetAPI<void*>({ "Terraria", "Item", "stack", "Field" }));
        createItem = ParseOtherField(api->GetAPI<void*>({ "Terraria", "Recipe", "createItem", "Field" }));
        currentRecipe = ParseOtherField(api->GetAPI<void*>({ "Terraria", "Recipe", "currentRecipe", "Field" }));
        requiredItem = ParseOtherField(api->GetAPI<void*>({ "Terraria", "Recipe", "requiredItem", "Field" }));
        requiredTile = ParseOtherField(api->GetAPI<void*>({ "Terraria", "Recipe", "requiredTile", "Field" }));

        IsAMaterial = ParseOtherField(api->GetAPI<void*>({"Terraria.ID", "ItemID.Sets", "IsAMaterial", "Field"}));
    }
}
