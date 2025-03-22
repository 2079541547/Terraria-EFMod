//
// Created by EternalFuture on 2025/1/25.
//

#include <ItemID.hpp>
#include <IL2CppArray.hpp>
#include <log.hpp>

void ItemID::cctor(void* i) {

    auto& Deprecated_v = *reinterpret_cast<IL2CppArray<bool>*>(Deprecated->GetPointer());
    auto& ItemsThatShouldNotBeInInventory_v = *reinterpret_cast<IL2CppArray<bool>*>(ItemsThatShouldNotBeInInventory->GetPointer());
    auto& ShimmerTransformToItem_v = *reinterpret_cast<IL2CppArray<int>*>(ShimmerTransformToItem->GetPointer());

    std::ranges::fill(Deprecated_v, false);
    std::ranges::fill(ItemsThatShouldNotBeInInventory_v, false);

    // 定义物品转换关系表
    constexpr std::array transformPairs = {
            // 双向转换对 (源物品ID, 目标物品ID)
            std::pair{3357, 3331},  // 拜月相关
            std::pair{3868, 3861},  // 食人魔
            std::pair{3862, 3867},  // 黑暗魔法师
            std::pair{2881, 3358},  // 火星飞碟
            std::pair{3847, 3865},  // 食人魔面具
            std::pair{3854, 3853},  // 无趣弓
            std::pair{2901, 2989},  // 邪教徒
            std::pair{2990, 2902},   // 邪教徒分支
            std::pair{3404, 1649},  // 毒孢旗
            std::pair{3398, 1648}   // 残手旗
    };

    // 批量设置双向转换关系
    for (const auto& [src, dest] : transformPairs) { // 结构化绑定解包
        ShimmerTransformToItem_v.Set(src, dest);
        ShimmerTransformToItem_v.Set(dest, src); // 自动生成反向映射
    }

    LOGD("已解除物品移除限制");
}

void ItemID::Template(BNM::UnityEngine::Object* i) {
    ((void(*)(void*))old_cctor)(i);
    for (auto fun: T_cctor.FunArray) {
        ((void(*)(void*))fun)(i);
    }
}