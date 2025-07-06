/*******************************************************************************
 * 创建时间: 2025/5/1
 * 作者: EternalFuture゙
 * Github: https://github.com/2079541547
 * 版权声明: Copyright © 2024 EternalFuture. All rights reserved.
 * 许可证: Licensed under the Apache License, Version 2.0 (the "License");
 *         you may not use this file except in compliance with the License.
 *         You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 *         Unless required by applicable law or agreed to in writing, software
 *         distributed under the License is distributed on an "AS IS" BASIS,
 *         WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *         See the License for the specific language governing permissions and
 *         limitations under the License.
 *
 * 注意事项: 请严格遵守Apache License 2.0协议使用本代码。Apache License 2.0允许商业用途，无需额外授权。
 *******************************************************************************/

#pragma once

#include "EFMod.hpp"
#include <any>

namespace Item {

    namespace Lang {
        inline void* GetItemName; // (int id)
        inline void* LocalizedText_Key;
    }

    namespace ID {
        inline void* ShimmerTransformToItem = nullptr; // int[]
        inline std::unordered_map<int, std::pair<int, std::string>> ShimmerTransformToItem_api; // 微光物品API， ShimmerTransform = { type = <int>, name = <string> }
    }

    inline bool Registered = false;

    void init(EFModAPI* api);

    void SetDefaults(void *instance, int Type, bool noMatCheck, void *variant);

    void SetDefaults_Template(void *instance, int Type, bool noMatCheck, void *variant);

    inline void (*old_SetDefaults)(void *, int, bool, void *) = nullptr;

    inline HookTemplate T_SetDefaults = {
            (void *) SetDefaults_Template,
            {}
    };

    inline std::unordered_map<std::string, std::any> API; // API提供

    inline std::unordered_map<std::string, std::pair<void*, std::string>> Fields_Cache; // 缓存字段
    inline std::vector<std::pair<std::string, std::string>> Fields {
            // 基础属性
            {"type", "int"},             // 物品ID，决定物品的基本类型
            {"stack", "int"},            // 当前堆叠数量
            {"maxStack", "int"},         // 最大堆叠数量(默认9999)
            {"netID", "int"},            // 网络同步ID
            {"prefix", "byte"},           // 物品前缀ID(影响属性)
            {"favorited", "bool"},       // 是否被标记为收藏

            // 使用控制
            {"useStyle", "int"},         // 使用动作样式(1=挥动,2=饮用,3=刺戳等)
            {"useAnimation", "int"},     // 使用动画总帧数
            {"useTime", "int"},          // 实际使用所需时间(帧数)
            {"autoReuse", "bool"},       // 是否允许按住自动连续使用
            {"reuseDelay", "int"},       // 再次使用间隔时间(帧数)
            {"channel", "bool"},         // 是否为持续施法类物品(如魔法书)
            {"noUseGraphic", "bool"},    // 使用时不显示物品图形

            // 战斗属性
            {"damage", "int"},           // 基础伤害值
            {"knockBack", "float"},      // 击退力度
            {"crit", "int"},             // 暴击几率(百分比)
            {"melee", "bool"},           // 是否为近战武器
            {"magic", "bool"},           // 是否为魔法武器
            {"ranged", "bool"},          // 是否为远程武器
            {"summon", "bool"},          // 是否为召唤武器
            {"noMelee", "bool"},         // 是否禁用近战攻击动画

            // 射弹系统
            {"shoot", "int"},            // 发射的射弹类型ID
            {"shootSpeed", "float"},     // 射弹初始速度
            {"shootsEveryUse", "bool"},  // 是否每次使用都发射射弹

            // 弹药系统
            {"ammo", "int"},             // 物品作为弹药时的类型
            {"useAmmo", "int"},          // 使用消耗的弹药类型
            {"notAmmo", "bool"},         // 非弹药但可作为弹药使用

            // 工具属性
            {"pick", "int"},             // 镐力(采矿效率)
            {"axe", "int"},              // 斧力(砍树效率)
            {"hammer", "int"},           // 锤力(拆除背景墙)
            {"tileBoost", "int"},        // 放置范围加成

            // 装备槽位
            {"headSlot", "int"},         // 头部装备槽位ID(-1表示不是)
            {"bodySlot", "int"},         // 身体装备槽位ID
            {"legSlot", "int"},          // 腿部装备槽位ID
            {"handOnSlot", "int"},       // 主手装备槽位
            {"handOffSlot", "int"},      // 副手装备槽位
            {"wingSlot", "int"},         // 翅膀槽位
            {"shieldSlot", "int"},       // 盾牌槽位
            {"defense", "int"},          // 装备提供的防御力

            // 消耗品属性
            {"potion", "bool"},          // 是否为药水类物品
            {"consumable", "bool"},      // 是否为一次性消耗品
            {"healLife", "int"},         // 恢复的生命值量
            {"healMana", "int"},         // 恢复的魔力值量
            {"mana", "int"},             // 使用消耗的魔力值

            // 外观与显示
            {"width", "int"},            // 物品贴图宽度(像素)
            {"height", "int"},           // 物品贴图高度(像素)
            // {"color", "Color"},          // 物品染色颜色(RGBA)
            {"scale", "float"},          // 显示缩放比例(默认1.0)
            {"alpha", "int"},            // 透明度(0-255)
            {"glowMask", "int"},         // 发光掩码ID

            // 稀有度与模式
            {"rare", "int"},             // 稀有度等级(0-11)
            {"expert", "bool"},          // 是否专家模式物品
            {"master", "bool"},          // 是否大师模式物品
            {"expertOnly", "bool"},      // 仅专家模式可用

            // 价值系统
            {"value", "int"},            // 基础价值(铜币为单位)
            {"buy", "bool"},             // 是否可购买
            {"buyOnce", "bool"},         // 是否只能购买一次
            {"shopCustomPrice", "int"},  // 自定义商店价格

            // 钓鱼相关
            {"fishingPole", "int"},      // 钓鱼竿强度
            {"bait", "int"},             // 鱼饵力

            // 其他功能
            {"makeNPC", "int"},          // 使用后生成的NPC ID
            {"questItem", "bool"},       // 是否为任务物品
            {"vanity", "bool"},          // 是否为时装物品
            {"material", "bool"},        // 是否为合成材料
            {"flame", "bool"},           // 是否产生火焰粒子
            {"mech", "bool"},            // 是否为机械物品
            {"shimmered", "bool"},       // 是否经过微光转化
            {"shimmerTime", "float"}     // 微光转化时间
    };
}