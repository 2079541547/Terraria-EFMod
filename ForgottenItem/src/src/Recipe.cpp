//
// Created by EternalFuture on 2025/1/25.
//

#include <Recipe.hpp>
#include <log.hpp>
#include <IL2CppArray.hpp>
#include <unordered_map>

void Recipe::initialize() {
    Item_Class = BNM::Class("Terraria", "Item", BNM::Image("Assembly-CSharp.dll"));
    Recipe_Class = BNM::Class("Terraria", "Recipe", BNM::Image("Assembly-CSharp.dll"));
    createItem = Recipe_Class.GetField("createItem");
    currentRecipe = Recipe_Class.GetField("currentRecipe");
    ItemSetDefaults = Item_Class.GetMethod("SetDefaults");
    stack = Item_Class.GetField("stack");
    requiredItem = Recipe_Class.GetField("requiredItem");
    AddRecipe = Recipe_Class.GetMethod("AddRecipe");
    requiredTile = Recipe_Class.GetField("requiredTile");

    HOOK(Recipe_Class.GetMethod("SetupRecipeGroups", 0), SetupRecipeGroups, old_SetupRecipeGroups);
}

std::unordered_map<int, std::vector<int>> LuminousTools = {
        { 3458, { 2785, 2783, 2782 } },
        { 3456, { 2775, 2773, 2772 } },
        { 3457, { 2780, 2778, 2777 } },
        { 3459, { 3465, 3463, 3462 } }
};

std::vector<int> giftV = {
        599,
        600,
        601
};


void Recipe::SetupRecipeGroups(BNM::UnityEngine::Object *i) {

    old_SetupRecipeGroups(i);

    for(auto _: LuminousTools) {
        for (auto __: _.second) {
            requiredItem.SetInstance(currentRecipe.Get());
            createItem.SetInstance(currentRecipe.Get());
            requiredTile.SetInstance(currentRecipe.Get());

            IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem.GetPointer()));
            IL2CppArray<int> requiredTile_v(*((void**)requiredTile.GetPointer()));

            ItemSetDefaults[createItem.Get()](__);
            ItemSetDefaults[requiredItem_v.At(0)](_.first);
            stack[requiredItem_v.At(0)] = 7;
            ItemSetDefaults[requiredItem_v.At(1)](3467);
            stack[requiredItem_v.At(1)] = 6;
            requiredTile_v.At(0) = 412;
            AddRecipe();
        }
    }

    //骷髅头弓
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](4058);
        ItemSetDefaults[requiredItem_v.At(0)](1274);
        ItemSetDefaults[requiredItem_v.At(1)](154);
        stack[requiredItem_v.At(1)] = 50;
        requiredTile_v.At(0) = 18;
        AddRecipe();
    }

    //仅颜色染料
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3978);
        ItemSetDefaults[requiredItem_v.At(0)](3385);
        ItemSetDefaults[requiredItem_v.At(1)](3386);
        ItemSetDefaults[requiredItem_v.At(2)](3387);
        ItemSetDefaults[requiredItem_v.At(3)](3388);
        requiredTile_v.At(0) = 228;
        AddRecipe();
    }

    //苹果派切片
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](4010);
        stack[createItem.Get()] = 4;
        ItemSetDefaults[requiredItem_v.At(0)](4011);
        AddRecipe();
    }

    //苹果派
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](4011);
        ItemSetDefaults[requiredItem_v.At(0)](4010);
        stack[requiredItem_v.At(0)] = 4;
        AddRecipe();
    }

    //第一分形
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](4722);
        ItemSetDefaults[requiredItem_v.At(0)](757);
        ItemSetDefaults[requiredItem_v.At(1)](3827);
        ItemSetDefaults[requiredItem_v.At(2)](3787);
        ItemSetDefaults[requiredItem_v.At(3)](1570);
        stack[requiredItem_v.At(3)] = 2;
        ItemSetDefaults[requiredItem_v.At(4)](2880);
        requiredTile_v.At(0) = 134;
        AddRecipe();
    }

    //骨块
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](766);
        ItemSetDefaults[requiredItem_v.At(0)](154);
        requiredTile_v.At(0) = 18;
        AddRecipe();
    }

    //小妖魔雷管背包
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3851);
        ItemSetDefaults[requiredItem_v.At(0)](3822);
        stack[requiredItem_v.At(0)] = 50;
        AddRecipe();
    }

    //哥布林面具
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3848);
        ItemSetDefaults[requiredItem_v.At(0)](3822);
        stack[requiredItem_v.At(0)] = 50;
        AddRecipe();
    }

    //埃特尼亚标枪
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3848);
        ItemSetDefaults[requiredItem_v.At(0)](3822);
        stack[requiredItem_v.At(0)] = 70;
        AddRecipe();
    }

    //哥布林炸弹帽
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3849);
        ItemSetDefaults[requiredItem_v.At(0)](3822);
        stack[requiredItem_v.At(0)] = 50;
        AddRecipe();
    }

    //旧礼物
    for (auto _ : giftV) {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](_);
        ItemSetDefaults[requiredItem_v.At(0)](1869);
        AddRecipe();
    }

    //Fake_newchest1
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3705);
        ItemSetDefaults[requiredItem_v.At(0)](3886);
        AddRecipe();
    }

    //Fake_newchest2
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](3706);
        ItemSetDefaults[requiredItem_v.At(0)](3887);
        AddRecipe();
    }

    // 睡觉图标
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](5013);
        requiredTile_v.At(0) = 79;
        AddRecipe();
    }

    //蓝邪教徒战士旗
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](2903);
        ItemSetDefaults[requiredItem_v.At(0)](2901);
        ItemSetDefaults[requiredItem_v.At(0)](2902);
        AddRecipe();
    }

    //白邪教徒战士旗
    {
        requiredItem.SetInstance(currentRecipe.Get());
        createItem.SetInstance(currentRecipe.Get());
        requiredTile.SetInstance(currentRecipe.Get());
        IL2CppArray<BNM::UnityEngine::Object *> requiredItem_v(
                *((void **) requiredItem.GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void **) requiredTile.GetPointer()));
        ItemSetDefaults[createItem.Get()](2991);
        ItemSetDefaults[requiredItem_v.At(0)](2989);
        ItemSetDefaults[requiredItem_v.At(0)](2990);
        AddRecipe();
    }
}