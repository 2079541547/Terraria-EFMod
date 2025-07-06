/*******************************************************************************
 * 文件名称: efmod
 * 项目名称: InstantRespawn
 * 创建时间: 25-5-18
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
#include "BaseType.hpp"
#include "TEFMod.hpp"
#include "Logger.hpp"

TEFMod::Logger* g_log;
TEFMod::TEFModAPI* g_api;

TEFMod::Field<int>*(*ParseIntField)(void*);

TEFMod::Field<int>* player_respawnTimer;
TEFMod::Field<int>* player_whoAmI;

void (*original_PlayerUpdateDead)(TEFMod::TerrariaInstance self);
void PlayerUpdateDead_T(TEFMod::TerrariaInstance i);

inline TEFMod::HookTemplate HookTemplate_PlayerUpdateDead {
    (void*) PlayerUpdateDead_T,
    {  }
};

void PlayerUpdateDead_T(TEFMod::TerrariaInstance i) {
    original_PlayerUpdateDead(i);
    for (const auto fun: HookTemplate_PlayerUpdateDead.FunctionArray) {
        if(fun) reinterpret_cast<void(*)(TEFMod::TerrariaInstance)>(fun)(i);
    }
}

void Hook_PlayerUpdateDead(TEFMod::TerrariaInstance self) {
    int playerId = player_whoAmI->Get(self);

    if (const int currentTimer = player_respawnTimer->Get(self); currentTimer > 180) {
        player_respawnTimer->Set(180, self);
        g_log->d("玩家[", playerId, "] 复活冷却已重置");
    }

    player_respawnTimer->Set(0, self);
}

class InstantRespawn final : public EFMod {
public:
    int Initialize(const std::string &path, MultiChannel *multiChannel) override { return 0; }

    void Send(const std::string &path, MultiChannel *multiChannel) override {
        g_api->registerFunctionDescriptor({
                  "Terraria",
                  "Player",
                  "UpdateDead",
                  "hook>>void",
                  0,
                  &HookTemplate_PlayerUpdateDead,
                  { (void*)Hook_PlayerUpdateDead }
              });

        g_api->registerApiDescriptor({"Terraria", "Player", "respawnTimer", "Field"});
        g_api->registerApiDescriptor({"Terraria", "Player", "whoAmI", "Field"});
    }

    void Receive(const std::string &path, MultiChannel *multiChannel) override {

        original_PlayerUpdateDead = g_api->GetAPI<void(*)(TEFMod::TerrariaInstance)>({
        "Terraria",
            "Player",
        "UpdateDead",
        "old_fun",
        0
        });

        // 解析字段
        player_respawnTimer = ParseIntField(g_api->GetAPI<void*>({
            "Terraria",
            "Player",
            "respawnTimer",
            "Field"
        }));

        player_whoAmI = ParseIntField(g_api->GetAPI<void*>({
            "Terraria",
            "Player",
            "whoAmI",
            "Field"
        }));
    }

    int Load(const std::string &path, MultiChannel *multiChannel) override {
        g_log = multiChannel->receive<TEFMod::Logger*(*)(const std::string& Tag, const std::string& filePath, const std::size_t)>("TEFMod::CreateLogger")("InstantRespawn-EternalFuture゙", "", 0);
        g_api = multiChannel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");

        g_log->init();

        ParseIntField = multiChannel->receive<TEFMod::Field<int>*(*)(void*)>("TEFMod::Field<Int>::ParseFromPointer");

        return 0;
    }

    int UnLoad(const std::string &path, MultiChannel *multiChannel) override {
        return 0;
    }

    Metadata GetMetadata() override {
        return {
        "InstantRespawn",
            "EternalFuture゙",
            "1.0.0",
            20250517,
            ModuleType::Game,
            {
                false
            }
        };
    }
};

EFMod* CreateMod() {
    static InstantRespawn mod;
    return &mod;
}
