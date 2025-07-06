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

#include <filesystem>
#include <vector>

#include "LanguagePacks.hpp"

#include "../EFMod.hpp"

namespace LanguageManager {

    struct ConfigEntry {
        std::string path;
        int priority;

        // 用于排序的比较运算符
        bool operator<(const ConfigEntry& other) const {
            return priority < other.priority;
        }
    };

    inline bool isCommon = true;
    inline std::vector<ConfigEntry> packs_info;

    inline std::unordered_map<std::string, LanguagePacks*> packs;
    extern std::vector<std::string> languages_container;
    extern std::vector<std::string> languages_code;

    static std::string CodeToName(const std::string& code);
    static std::string NameToCode(const std::string& name);

    void init(const std::filesystem::path& path);

    namespace GameCulture {
        inline void* FromName;
    }

    inline void* LoadLanguageFromFileText;
    inline void* ProcessCopyCommandsInTexts;
    inline void* PrepareAliases;

    inline void(*old_LoadLanguage)(void*, void*, bool);
    void LoadLanguage(void* instance, void* culture, bool processCopyCommands);
    /*
     *   void LoadLanguageFromFileText_Hooked(void* instance, void* content);
     *   void LoadLanguageFromFileText_Template(void* instance, void* content);
     *   inline void(*old_LoadLanguageFromFileText)(void*, void*) = nullptr;
     *
     *   inline HookTemplate T_LoadLanguageFromFileText {
     *           (void *)LoadLanguageFromFileText_Template,
     *           {}
};
*/

}
