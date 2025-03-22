//
// Created by EternalFuture on 2025/1/25.
//

#include <Recipe.hpp>
#include <log.hpp>
#include <IL2CppArray.hpp>
#include <unordered_map>

void Recipe::RegisterRecipe(const Recipe::GameRecipe &recipe) {
    // 隔离作用域
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        auto& itemArray = *reinterpret_cast<IL2CppArray<BNM::UnityEngine::Object*>*>(requiredItem->GetPointer());
        auto& tileArray = *reinterpret_cast<IL2CppArray<int>*>(requiredTile->GetPointer());

        // 设置产物
        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(recipe.resultID);

        // 设置材料
        for (size_t i = 0; i < recipe.materials.size(); ++i) {
            ItemSetDefaults->SetInstance(itemArray.At(i));
            ItemSetDefaults->Call(recipe.materials[i].itemID);

            stack->SetInstance(itemArray.At(i));
            stack->Set(recipe.materials[i].stack);
        }

        // 设置工作站
        if (recipe.craftStation != 0) {
            tileArray.At(0) = recipe.craftStation;
        }

        AddRecipe->Call();
    }
}

void Recipe::SetupRecipeGroups(void* i) {

    // 流明工具系列
    const std::vector<GameRecipe> luminousRecipes = {
            GameRecipe(2785, {CraftMaterial(3458,7), CraftMaterial(3467,6)}, 412),
            GameRecipe(2783, {CraftMaterial(3458,7), CraftMaterial(3467,6)}, 412),
            GameRecipe(2782, {CraftMaterial(3458,7), CraftMaterial(3467,6)}, 412),
            GameRecipe(3465, {CraftMaterial(3459,7), CraftMaterial(3467,6)}, 412),
            GameRecipe(3463, {CraftMaterial(3459,7), CraftMaterial(3467,6)}, 412),
            GameRecipe(3462, {CraftMaterial(3459,7), CraftMaterial(3467,6)}, 412)
    };

    // 核心配方集合
    const std::vector<GameRecipe> coreRecipes = {
            // 骷髅头弓
            GameRecipe(4058, {CraftMaterial(1274), CraftMaterial(154,50)}, 18),

            // 颜色染料
            GameRecipe(3978, {CraftMaterial(3385), CraftMaterial(3386),
                              CraftMaterial(3387), CraftMaterial(3388)}, 228),

            // 苹果派系列
            GameRecipe(4010, {CraftMaterial(4011)}, 0),
            GameRecipe(4011, {CraftMaterial(4010,4)}, 0),

            // 第一分形
            GameRecipe(4722, {CraftMaterial(757), CraftMaterial(3827),
                              CraftMaterial(3787), CraftMaterial(1570,2),
                              CraftMaterial(2880)}, 134),

            // 骨块合成
            GameRecipe(766, {CraftMaterial(154)}, 18),

            // 哥布林装备
            GameRecipe(3851, {CraftMaterial(3822,50)}, 0),  // 雷管背包
            GameRecipe(3848, {CraftMaterial(3822,50)}, 0),  // 面具
            GameRecipe(3849, {CraftMaterial(3822,50)}, 0),  // 炸弹帽
            GameRecipe(3848, {CraftMaterial(3822,70)}, 0),  // 埃特尼亚标枪

            // 礼物系列
            GameRecipe(599, {CraftMaterial(1869)}, 0),
            GameRecipe(600, {CraftMaterial(1869)}, 0),
            GameRecipe(601, {CraftMaterial(1869)}, 0),

            // 伪装宝箱
            GameRecipe(3705, {CraftMaterial(3886)}, 0),
            GameRecipe(3706, {CraftMaterial(3887)}, 0),

            // 特殊物品
            GameRecipe(5013, {}, 79)  // 睡觉图标
    };

    // 批量注册
    for (const auto& recipe : luminousRecipes) {
        RegisterRecipe(recipe);
    }
    for (const auto& recipe : coreRecipes) {
        RegisterRecipe(recipe);
    }
}

void Recipe::Template(BNM::UnityEngine::Object *i) {
    ((void(*)(BNM::UnityEngine::Object*)) old_SetupRecipeGroups)(i);
    for (auto fun: T_SetupRecipeGroups.FunArray) {
        if(fun) ((void(*)(void*))fun)(i);
    }
}