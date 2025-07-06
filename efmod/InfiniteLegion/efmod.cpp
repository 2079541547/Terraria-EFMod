/*******************************************************************************
 * 文件名称: efmod
 * 项目名称: InfiniteLegionc
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

#include "BaseType.hpp"
#include "TEFMod.hpp"
#include "Logger.hpp"

TEFMod::Logger *g_log;
TEFMod::TEFModAPI *g_api;

// 字段解析器
TEFMod::Field<int> *(*ParseIntField)(void *);

// 游戏字段
TEFMod::Field<int> *maxTurrets;
TEFMod::Field<int> *maxTurretsOld;
TEFMod::Field<int> *maxMinions;

// Hook声明
void (*original_ResetEffects)(TEFMod::TerrariaInstance);

void ResetEffects_T(TEFMod::TerrariaInstance i);

inline TEFMod::HookTemplate HookTemplate_PlayerUpdateDead{
    (void *) ResetEffects_T,
    {}
};

void ResetEffects_T(TEFMod::TerrariaInstance i) {
    original_ResetEffects(i);
    for (const auto fun: HookTemplate_PlayerUpdateDead.FunctionArray) {
        if (fun) reinterpret_cast<void(*)(TEFMod::TerrariaInstance)>(fun)(i);
    }
}

void Hook_ResetEffects(TEFMod::TerrariaInstance self) {
    // 记录原始值
    int oldTurrets = maxTurrets->Get(self);
    int oldMinions = maxMinions->Get(self);

    if (oldTurrets != 999 || oldMinions != 999) {
        // 设置无限数量
        maxTurrets->Set(999, self);
        maxTurretsOld->Set(999, self);
        maxMinions->Set(999, self);
        g_log->i("[玩家] 召唤限制解除: 炮台 ", oldTurrets,
                 "->无限 仆从 ", oldMinions, "->无限");
    }
}

class InfiniteLegionc final : public EFMod {
public:
    int Initialize(const std::string &path, MultiChannel *multiChannel) override {
        return 0;
    }

    void Send(const std::string &path, MultiChannel *multiChannel) override {
        g_api->registerFunctionDescriptor({
            "Terraria",
            "Player",
            "ResetEffects",
            "hook>>void",
            0,
            &HookTemplate_PlayerUpdateDead,
            {(void *) Hook_ResetEffects}
        });

        g_api->registerApiDescriptor({"Terraria", "Player", "maxTurrets", "Field"});
        g_api->registerApiDescriptor({"Terraria", "Player", "maxTurretsOld", "Field"});
        g_api->registerApiDescriptor({"Terraria", "Player", "maxMinions", "Field"});
    }

    void Receive(const std::string &path, MultiChannel *multiChannel) override {
        original_ResetEffects = g_api->GetAPI<void(*)(TEFMod::TerrariaInstance)>({
            "Terraria",
            "Player",
            "ResetEffects",
            "old_fun",
            0
        });

        maxTurrets = ParseIntField(g_api->GetAPI<void *>({
            "Terraria",
            "Player",
            "maxTurrets",
            "Field"
        }));

        maxMinions = ParseIntField(g_api->GetAPI<void *>({
            "Terraria",
            "Player",
            "maxMinions",
            "Field"
        }));

        maxTurretsOld = ParseIntField(g_api->GetAPI<void *>({
            "Terraria",
            "Player",
            "maxTurretsOld",
            "Field"
        }));
    }

    int Load(const std::string &path, MultiChannel *multiChannel) override {
        g_api = multiChannel->receive<TEFMod::TEFModAPI *>("TEFMod::TEFModAPI");
        g_log = multiChannel->receive<TEFMod::Logger*(*)(const std::string &Tag, const std::string &filePath,
                                                         const std::size_t)>("TEFMod::CreateLogger")(
            "InfiniteLegionc-EternalFuture゙", "", 0);
        g_log->init();
        ParseIntField = multiChannel->receive<TEFMod::Field<int>*(*)(void *)>("TEFMod::Field<Int>::ParseFromPointer");

        return 0;
    }

    int UnLoad(const std::string &path, MultiChannel *multiChannel) override {
        return 0;
    }

    Metadata GetMetadata() override {
        return {
            "InfiniteLegionc",
            "EternalFuture゙",
            "1.0.1",
            20250524,
            ModuleType::Game,
            {false}
        };
    }
};

EFMod *CreateMod() {
    static InfiniteLegionc mod;
    return &mod;
}
