/*******************************************************************************
 * 文件名称: efmod
 * 项目名称: VeinMiner
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

#include "efmod_core.hpp"

#include "tefmod.hpp"

#include "vein_miner.hpp"
#include "tile_data.hpp"

TEFMod::TEFModAPI* api;

inline void (*old_UpdateTime)(TEFMod::TerrariaInstance) = nullptr;
void T_UpdateTime(TEFMod::TerrariaInstance instance);
void HookUpdateTime(TEFMod::TerrariaInstance instance) {
    vein_miner::Update();
}
inline TEFMod::HookTemplate HookTemplate_UpdateTime = {
    reinterpret_cast<void*>(T_UpdateTime),
    {   }
};
void T_UpdateTime(TEFMod::TerrariaInstance instance) {
    old_UpdateTime(instance);
    for (const auto fun: HookTemplate_UpdateTime.FunctionArray) if (fun) reinterpret_cast<decltype(old_UpdateTime)>(fun)(instance);
}

inline int (*old_AddDamage)(TEFMod::TerrariaInstance instance, int tileId, int damageAmount, bool updateAmount) = nullptr;
int HookAddDamage(TEFMod::TerrariaInstance instance, int tileId, int damageAmount, bool updateAmount) { return 0; }
int T_AddDamage(TEFMod::TerrariaInstance instance, int tileId, int damageAmount, bool updateAmount) {
    const int damage = old_AddDamage(instance, tileId, damageAmount, updateAmount);
    if (damage >= 100) {
        if (TileData::pick->Get(TileData::HeldItem->Get(TileData::get_LocalPlayer->Call(nullptr, 0))) <= 0) { // 检查是否使用镐类工具
            const auto hitTileObjectData = ParseIntArray(TileData::HitTileObjectData->Get(instance));
            const int objectIndex = tileId * 7;
            const int x = hitTileObjectData->at(objectIndex);
            const int y = hitTileObjectData->at(objectIndex + 1);
            vein_miner::Mine(x, y);
        }
    }

    return damage;
}


inline TEFMod::HookTemplate HookTemplate_AddDamage = {
    reinterpret_cast<void*>(T_AddDamage),
    {  }
};

class VeinMiner final : public EFMod {
public:

    void Send(const std::string &path, MultiChannel *multiChannel) override {
        api = multiChannel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        TileData::init(api);

        api->registerFunctionDescriptor({
            "Terraria",
            "Main",
            "UpdateTime",
            "hook>>void",
            0,
            &HookTemplate_UpdateTime,
            { reinterpret_cast<void*>(HookUpdateTime) }
        });

        api->registerFunctionDescriptor({
                    "Terraria",
                    "HitTile",
                    "AddDamage",
                    "hook>>void",
                    3,
                    &HookTemplate_AddDamage,
                    { reinterpret_cast<void*>(HookAddDamage) }
        });

    }

    void Receive(const std::string &path, MultiChannel *multiChannel) override {
        api = multiChannel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        TileData::init(api);

        old_AddDamage = api->GetAPI<decltype(old_AddDamage)>({
                "Terraria",
                    "HitTile",
                    "AddDamage",
                    "old_fun",
                    3,
        });

        old_UpdateTime = api->GetAPI<decltype(old_UpdateTime)>({
            "Terraria",
                "Main",
                "UpdateTime",
            "old_fun",
            0
        });
    }

    int Load(const std::string &path, MultiChannel *multiChannel) override {
        logger = multiChannel->receive<TEFMod::Logger*(*)(const std::string& Tag, const std::string& filePath, const std::size_t)>("TEFMod::CreateLogger")("VeinMiner-EternalFuture゙", "", 0);
        logger->init();

        ParseIntField = multiChannel->receive<decltype(ParseIntField)>("TEFMod::Field<Int>::ParseFromPointer");
        ParseBoolField = multiChannel->receive<decltype(ParseBoolField)>("TEFMod::Field<Bool>::ParseFromPointer");
        ParseOtherField = multiChannel->receive<decltype(ParseOtherField)>("TEFMod::Field<Other>::ParseFromPointer");
        ParseFloatField = multiChannel->receive<decltype(ParseFloatField)>("TEFMod::Field<Float>::ParseFromPointer");
        ParseUShortMethod = multiChannel->receive<decltype(ParseUShortMethod)>("TEFMod::Field<UShort>::ParseFromPointer");

        ParseIntMethod = multiChannel->receive<decltype(ParseIntMethod)>("TEFMod::Method<Int>::ParseFromPointer");
        ParseVoidMethod = multiChannel->receive<decltype(ParseVoidMethod)>("TEFMod::Method<Void>::ParseFromPointer");
        ParseUShortMethod = multiChannel->receive<decltype(ParseUShortMethod)>("TEFMod::Method<UShort>::ParseFromPointer");
        ParseOtherMethod = multiChannel->receive<decltype(ParseOtherMethod)>("TEFMod::Method<Other>::ParseFromPointer");

        ParseIntArray = multiChannel->receive<decltype(ParseIntArray)>("TEFMod::Array<Int>::ParseFromPointer");
        ParseBoolArray = multiChannel->receive<decltype(ParseBoolArray)>("TEFMod::Array<Bool>::ParseFromPointer");
        ParseOtherArray = multiChannel->receive<decltype(ParseOtherArray)>("TEFMod::Array<Other>::ParseFromPointer");

        return 0;
    }

    Metadata GetMetadata() override {
        return {
            "VeinMiner",
            "EternalFuture゙",
            "1.0.0",
            20250517,
            ModuleType::Game,
            { false }
        };
    }

    int UnLoad(const std::string &path, MultiChannel *multiChannel) override { return 0; }
    int Initialize(const std::string &path, MultiChannel *multiChannel) override { return 0; }
};

EFMod* CreateMod() {
    static VeinMiner instance;
    return &instance;
}