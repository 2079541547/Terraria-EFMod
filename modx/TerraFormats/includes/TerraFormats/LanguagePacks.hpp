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

#include <string>
#include <variant>
#include <vector>

#include "../json.hpp"

class LanguagePacks {
public:

    inline static std::vector<std::string> common_name = {
            "Base",
            "Game",
            "Items",
            "Legacy",
            "Mobile",
            "NPCs",
            "Projectiles",
            "Town"
    };

    struct LanguageInfo {
        std::string name;
        std::string author;
        std::string autocomplete;
        std::string type;
        int standard;
    };

    LanguagePacks(const std::string& path);
    LanguagePacks() {}

    LanguageInfo get_info();

    bool isCommon();

    std::vector<uint8_t> get_file_data(const std::string& name);
    std::unordered_map<std::string, std::vector<uint8_t>> get_data() { return _data; };

    static std::string get_setLanguagesList(const std::unordered_map<std::string, std::string> &list, const std::string& data);

private:

    std::unordered_map<std::string, std::vector<uint8_t>> _data;
    LanguageInfo _info;
};