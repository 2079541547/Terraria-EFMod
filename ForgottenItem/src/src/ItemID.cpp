//
// Created by EternalFuture on 2025/1/25.
//

#include <ItemID.hpp>
#include <IL2CppArray.hpp>
#include <log.hpp>

void ItemID::initialize() {
    ItemID_Class = BNM::Class("Terraria.ID", "ItemID", BNM::Image("Assembly-CSharp.dll"));
    ItemID_Set = ItemID_Class.GetInnerClass("Sets");

    Count = ItemID_Class.GetField("Count");
    Deprecated = ItemID_Set.GetField("Deprecated");
    ItemsThatShouldNotBeInInventory = ItemID_Set.GetField("ItemsThatShouldNotBeInInventory");
    ShimmerTransformToItem = ItemID_Set.GetField("ShimmerTransformToItem");


    HOOK(ItemID_Set.GetMethod(".cctor", 0), cctor, old_cctor);
}

void ItemID::cctor(BNM::UnityEngine::Object *i) {

    old_cctor(i);

    IL2CppArray<bool> Deprecated_v(*((void**)Deprecated.GetPointer()));
    IL2CppArray<bool> ItemsThatShouldNotBeInInventory_v(*((void**)ItemsThatShouldNotBeInInventory.GetPointer()));
    IL2CppArray<int> ShimmerTransformToItem_v(*((void**)ShimmerTransformToItem.GetPointer()));

    for (int index = 0; index < Deprecated_v.Size(); ++index) {
        Deprecated_v.Set(index, false);
        ItemsThatShouldNotBeInInventory_v.Set(index, false);
    }

    //拜月
    ShimmerTransformToItem_v.Set(3357, 3331);
    ShimmerTransformToItem_v.Set(3331, 3357);

    //食人魔
    ShimmerTransformToItem_v.Set(3868, 3861);
    ShimmerTransformToItem_v.Set(3861, 3868);

    //黑暗魔法师
    ShimmerTransformToItem_v.Set(3862, 3867);
    ShimmerTransformToItem_v.Set(3867, 3862);

    //火星飞碟
    ShimmerTransformToItem_v.Set(2881, 3358);
    ShimmerTransformToItem_v.Set(3358, 2881);

    //食人魔面具
    ShimmerTransformToItem_v.Set(3847, 3865);
    ShimmerTransformToItem_v.Set(3865, 3847);

    //无趣弓
    ShimmerTransformToItem_v.Set(3854, 3853);
    ShimmerTransformToItem_v.Set(3853, 3854);

    //邪教徒
    ShimmerTransformToItem_v.Set(2901, 2989);
    ShimmerTransformToItem_v.Set(2989, 2901);

    ShimmerTransformToItem_v.Set(2990, 2902);
    ShimmerTransformToItem_v.Set(2902, 2990);

    //毒孢旗
    ShimmerTransformToItem_v.Set(3404, 1649);
    ShimmerTransformToItem_v.Set(1649, 3404);

    //残手旗
    ShimmerTransformToItem_v.Set(3398, 1648);
    ShimmerTransformToItem_v.Set(1648, 3398);

    LOGD("已解除物品移除限制");
}