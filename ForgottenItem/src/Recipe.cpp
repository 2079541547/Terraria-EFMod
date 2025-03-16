//
// Created by EternalFuture on 2025/1/25.
//

#include <Recipe.hpp>
#include <log.hpp>
#include <IL2CppArray.hpp>
#include <unordered_map>


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

void Recipe::SetupRecipeGroups(void *i) {

    for(auto _: LuminousTools) {
        for (auto __: _.second) {
            requiredItem->SetInstance(currentRecipe->Get());
            createItem->SetInstance(currentRecipe->Get());
            requiredTile->SetInstance(currentRecipe->Get());

            IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
            IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

            ItemSetDefaults->SetInstance(createItem->Get());
            ItemSetDefaults->Call(__);

            ItemSetDefaults->SetInstance(requiredItem_v.At(0));
            ItemSetDefaults->Call(_.first);

            stack->SetInstance(requiredItem_v.At(0));
            stack->Set(7);

            ItemSetDefaults->SetInstance(requiredItem_v.At(1));
            ItemSetDefaults->Call(3467);

            stack->SetInstance(requiredItem_v.At(1));
            stack->Set(6);

            requiredTile_v.At(0) = 412;
            AddRecipe->Call();
        }
    }

    // 骷髅头弓
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(4058);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(1274);

        ItemSetDefaults->SetInstance(requiredItem_v.At(1));
        ItemSetDefaults->Call(154);

        stack->SetInstance(requiredItem_v.At(1));
        stack->Set(50);

        requiredTile_v.At(0) = 18;
        AddRecipe->Call();
    }

// 仅颜色染料
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3978);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3385);

        ItemSetDefaults->SetInstance(requiredItem_v.At(1));
        ItemSetDefaults->Call(3386);

        ItemSetDefaults->SetInstance(requiredItem_v.At(2));
        ItemSetDefaults->Call(3387);

        ItemSetDefaults->SetInstance(requiredItem_v.At(3));
        ItemSetDefaults->Call(3388);

        requiredTile_v.At(0) = 228;
        AddRecipe->Call();
    }

// 苹果派切片
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(4010);

        stack->SetInstance(createItem->Get());
        stack->Set(4);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(4011);

        AddRecipe->Call();
    }

// 苹果派
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(4011);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(4010);

        stack->SetInstance(requiredItem_v.At(0));
        stack->Set(4);

        AddRecipe->Call();
    }

// 第一分形
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(4722);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(757);

        ItemSetDefaults->SetInstance(requiredItem_v.At(1));
        ItemSetDefaults->Call(3827);

        ItemSetDefaults->SetInstance(requiredItem_v.At(2));
        ItemSetDefaults->Call(3787);

        ItemSetDefaults->SetInstance(requiredItem_v.At(3));
        ItemSetDefaults->Call(1570);

        stack->SetInstance(requiredItem_v.At(3));
        stack->Set(2);

        ItemSetDefaults->SetInstance(requiredItem_v.At(4));
        ItemSetDefaults->Call(2880);

        requiredTile_v.At(0) = 134;
        AddRecipe->Call();
    }

// 骨块
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(766);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(154);

        requiredTile_v.At(0) = 18;
        AddRecipe->Call();
    }

// 小妖魔雷管背包
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3851);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3822);

        stack->SetInstance(requiredItem_v.At(0));
        stack->Set(50);

        AddRecipe->Call();
    }

// 哥布林面具
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3848);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3822);

        stack->SetInstance(requiredItem_v.At(0));
        stack->Set(50);

        AddRecipe->Call();
    }

// 埃特尼亚标枪
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3848);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3822);

        stack->SetInstance(requiredItem_v.At(0));
        stack->Set(70);

        AddRecipe->Call();
    }

// 哥布林炸弹帽
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3849);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3822);

        stack->SetInstance(requiredItem_v.At(0));
        stack->Set(50);

        AddRecipe->Call();
    }

// 旧礼物
    for (auto _ : giftV) {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(_);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(1869);

        AddRecipe->Call();
    }

// Fake_newchest1
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3705);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3886);

        AddRecipe->Call();
    }

// Fake_newchest2
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(3706);

        ItemSetDefaults->SetInstance(requiredItem_v.At(0));
        ItemSetDefaults->Call(3887);

        AddRecipe->Call();
    }

    // 睡觉图标
    {
        requiredItem->SetInstance(currentRecipe->Get());
        createItem->SetInstance(currentRecipe->Get());
        requiredTile->SetInstance(currentRecipe->Get());

        IL2CppArray<BNM::UnityEngine::Object*> requiredItem_v(*((void**)requiredItem->GetPointer()));
        IL2CppArray<int> requiredTile_v(*((void**)requiredTile->GetPointer()));

        ItemSetDefaults->SetInstance(createItem->Get());
        ItemSetDefaults->Call(5013);

        requiredTile_v.At(0) = 79;
        AddRecipe->Call();
    }

}

void Recipe::Template(BNM::UnityEngine::Object *i) {
    ((void(*)(BNM::UnityEngine::Object*)) old_SetupRecipeGroups)(i);
    for (auto fun: T_SetupRecipeGroups.FunArray) {
        if(fun) ((void(*)(void*))fun)(i);
    }
}