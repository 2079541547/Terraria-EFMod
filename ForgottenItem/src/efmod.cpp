//
// Created by EternalFuture on 2025/1/25.
//

#include <EFMod.hpp>
#include <ItemID.hpp>
#include <log.hpp>
#include <Recipe.hpp>
#include <Item.hpp>

class ForgottenItem: public EFMod {
public:

    int initialize(EFModAPI *mod, std::filesystem::path path) override {

        //=== ItemID ===//

        ItemID::old_cctor = mod->getAPI({
                                                "Assembly-CSharp.dll",
                                                "Terraria.ID",
                                                "ItemID.Sets",
                                                ".cctor",
                                                "old_fun",
                                                0
                                        });

        for (auto field: ItemID::Fields) {
            auto ptr = mod->getAPI({
                "Assembly-CSharp.dll",
                "Terraria.ID",
                "ItemID.Sets",
                field,
                "Field"
            });

            LOGD("字段 %s 获取到的地址: %p", field.c_str(), ptr);
            if (field == "Deprecated") ItemID::Deprecated = static_cast<BNM::Field<BNM::Structures::Mono::Array<bool> *> *>(ptr);
            else if (field == "ItemsThatShouldNotBeInInventory") ItemID::ItemsThatShouldNotBeInInventory = static_cast<BNM::Field<BNM::Structures::Mono::Array<bool> *> *>(ptr);
            else if (field == "ShimmerTransformToItem") ItemID::ShimmerTransformToItem = static_cast<BNM::Field<BNM::Structures::Mono::Array<int> *> *>(ptr);
            else LOGE("未知字段: %s", field.c_str());
        }

        //=== Recipe ===//

        Recipe::old_SetupRecipeGroups = mod->getAPI({
            "Assembly-CSharp.dll",
            "Terraria",
            "Recipe",
            "SetupRecipeGroups",
            "old_fun",
            0,
        });

        for (auto field: Recipe::Fields) {
            auto ptr = mod->getAPI({
                                           "Assembly-CSharp.dll",
                                           "Terraria",
                                           "Recipe",
                                           field,
                                           "Field"
                                   });

            LOGD("字段 %s 获取到的地址: %p", field.c_str(), ptr);
            if (field == "stack") Recipe::stack = static_cast<BNM::Field<int> *>(ptr);
            else if (field == "createItem") Recipe::createItem = static_cast<BNM::Field<BNM::UnityEngine::Object *> *>(ptr);
            else if (field == "currentRecipe") Recipe::currentRecipe = static_cast<BNM::Field<BNM::UnityEngine::Object *> *>(ptr);
            else if (field == "requiredItem") Recipe::requiredItem = static_cast<BNM::Field<BNM::Structures::Mono::Array<BNM::UnityEngine::Object *> *> *>(ptr);
            else if (field == "requiredTile") Recipe::requiredTile = static_cast<BNM::Field<BNM::Structures::Mono::Array<int> *> *>(ptr);
            else LOGE("未知字段: %s", field.c_str());
        }

        Recipe::AddRecipe = static_cast<BNM::Method<void> *>(mod->getAPI({
                                                                                 "Assembly-CSharp.dll",
                                                                                 "Terraria",
                                                                                 "Recipe",
                                                                                 "AddRecipe",
                                                                                 "Method"
                                                                         }));

        Recipe::ItemSetDefaults = static_cast<BNM::Method<void> *>(mod->getAPI({
                                                                                       "Assembly-CSharp.dll",
                                                                                       "Terraria",
                                                                                       "Item",
                                                                                       "SetDefaults",
                                                                                       "Method"
                                                                               }));

        Recipe::stack = static_cast<BNM::Field<int> *>(mod->getAPI({
                                                                           "Assembly-CSharp.dll",
                                                                           "Terraria",
                                                                           "Item",
                                                                           "stack",
                                                                           "Field"
                                                                   }));


        //=== Item ===//
        Item::Initialize(mod);

        return 0;
    }

    void Register(EFModAPI *mod, std::filesystem::path path) override {
        //=== ItemID ===//
        mod->registerModFuncDescriptor({
            "Assembly-CSharp.dll",
            "Terraria.ID",
            "ItemID.Sets",
            ".cctor",
            "hook>>void",
            0,
            &ItemID::T_cctor,
            { (void*)ItemID::cctor }
        });

        for (auto field: ItemID::Fields) {
            mod->registerModApiDescriptor({
                "Assembly-CSharp.dll",
                "Terraria.ID",
                "ItemID.Sets",
                field,
                "Field"
            });
        }

        //=== Recipe ===//

        mod->registerModFuncDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Recipe",
            "SetupRecipeGroups",
            "hook>>void",
            0,
            &Recipe::T_SetupRecipeGroups,
            { (void*)Recipe::SetupRecipeGroups }
        });


        for (auto field: Recipe::Fields) {
            mod->registerModApiDescriptor({
                                                  "Assembly-CSharp.dll",
                                                  "Terraria",
                                                  "Recipe",
                                                  field,
                                                  "Field"
                                          });
        }

        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Recipe",
            "AddRecipe",
            "Method",
            0
        });

        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            "SetDefaults",
            "Method",
            1
        });

        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            "stack",
            "Field"
        });

        //=== Item ===//
        Item::Register(mod);
    }

    ModMetadata getInfo() override {
        return {
            "ForgottenItem",
            "EternalFuture゙",
            "1.0.1"
        };
    }

};

EFMod* CreateMod() {
    static ForgottenItem Mod;
    return &Mod;
}