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

#include "TerraFormats/Item.hpp"
#include "TerraFormats/Log.hpp"

#include "IL2CppArray.hpp"

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/Field.hpp>
#include <BNM/Method.hpp>

void Item::init(EFModAPI *api) {
    if (!Registered) {
        for (const auto& field: Fields) {
            api->registerModApiDescriptor({
                "Assembly-CSharp.dll",
                "Terraria",
                "Item",
                field.first,
                "Field"
            });
        }

        api->registerModFuncDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            "SetDefaults",
            "hook>>void",
            3,
            &T_SetDefaults,
            { (void*)SetDefaults }
        });

        api->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria.ID",
            "ItemID.Sets",
            "ShimmerTransformToItem",
            "Field"
        });

        api->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Lang",
            "GetItemName",
            "Methodc",
            1
        });

    } else {
        for (const auto& field: Fields) {
            Fields_Cache[field.first] = { api->getAPI({
                                                             "Assembly-CSharp.dll",
                                                             "Terraria",
                                                             "Item",
                                                             field.first,
                                                             "Field"
            }), field.second };
        }
        Fields.clear();

        old_SetDefaults = (void(*)(void *, int, bool, void *))api->getAPI({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            "SetDefaults",
            "old_fun",
            3
        });

        ID::ShimmerTransformToItem = api->getAPI({
            "Assembly-CSharp.dll",
            "Terraria.ID",
            "ItemID.Sets",
            "ShimmerTransformToItem",
            "Field"
        });

        Lang::GetItemName = api->getAPI({
                                                                  "Assembly-CSharp.dll",
                                                                  "Terraria",
                                                                  "Lang",
                                                                  "GetItemName",
                                                                  "Methodc",
                                                                  1
                                                          });
    }
}

void Item::SetDefaults(void *instance, int Type, bool noMatCheck, void *variant) {
    if (ID::ShimmerTransformToItem_api.empty()) {
        IL2CppArray<int> ShimmerTransformToItem_v(*((void**)static_cast<BNM::FieldBase*>(ID::ShimmerTransformToItem)->GetFieldPointer()));

        auto item_text_instance = static_cast<BNM::Method<BNM::IL2CPP::Il2CppObject*>*>(Lang::GetItemName)->Call(Type);
        auto itemName = static_cast<BNM::Field<BNM::Structures::Mono::String*>*>(Lang::LocalizedText_Key);

        auto ShimmerTransformToItem_vector = ShimmerTransformToItem_v.ToVector();
        for (int i = 0; i < ShimmerTransformToItem_vector.size(); ++i) {
            auto item = ShimmerTransformToItem_vector.at(i);
            if (item != -1) {
                auto shimmer_item_text_instance = static_cast<BNM::Method<BNM::IL2CPP::Il2CppObject*>*>(Lang::GetItemName)->Call(i);
                itemName->SetInstance(shimmer_item_text_instance);
                ID::ShimmerTransformToItem_api[i] = { ShimmerTransformToItem_vector.at(i), itemName->Get()->str() };
                LOGI("物品: %d -> %d(%s)", i, item, itemName->Get()->str().c_str());
            }
        }
    }

    API["ShimmerTransformToItem"] = ID::ShimmerTransformToItem_api[Type];

    for (const auto& field: Fields_Cache) {
        if (field.second.second == "int") {
            auto* field_ptr = static_cast<BNM::Field<int>*>(field.second.first);
            field_ptr->SetInstance(static_cast<BNM::IL2CPP::Il2CppObject*>(instance));
            API[field.first] = field_ptr->Get();
        } else if (field.second.second == "bool") {
            auto* field_ptr = static_cast<BNM::Field<bool>*>(field.second.first);
            field_ptr->SetInstance(static_cast<BNM::IL2CPP::Il2CppObject*>(instance));
            API[field.first] = field_ptr->Get();
        } else if (field.second.second == "float") {
            auto* field_ptr = static_cast<BNM::Field<float>*>(field.second.first);
            field_ptr->SetInstance(static_cast<BNM::IL2CPP::Il2CppObject*>(instance));
            API[field.first] = field_ptr->Get();
        } else if (field.second.second == "byte") {
            auto* field_ptr = static_cast<BNM::Field<uint8_t>*>(field.second.first);
            field_ptr->SetInstance(static_cast<BNM::IL2CPP::Il2CppObject*>(instance));
            API[field.first] = field_ptr->Get();
        }
    }
}

void Item::SetDefaults_Template(void *instance, int Type, bool noMatCheck, void *variant) {}