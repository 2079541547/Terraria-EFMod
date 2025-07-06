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

#include "TerraFormats/TerraFormats.hpp"
#include "TerraFormats/LanguageManager.hpp"

int TerraFormats::initialize(EFModAPI *mod, std::filesystem::path path) {
    LanguageManager::old_LoadLanguageFromFileText = (void(*)(void*, void*))mod->getAPI({
        "Assembly-CSharp.dll",
        "Terraria.Localization",
        "LanguageManager",
        "LoadLanguageFromFileText",
        "old_fun",
        1
    });

    return 0;
}

void TerraFormats::Register(EFModAPI *mod, std::filesystem::path path) {

    LanguageManager::init(path);

    mod->registerModFuncDescriptor({
        "Assembly-CSharp.dll",
        "Terraria.Localization",
        "LanguageManager",
        "LoadLanguageFromFileText",
        "hook>>void",
        1,
        &LanguageManager::T_LoadLanguageFromFileText,
        { (void*)LanguageManager::LoadLanguageFromFileText_Hooked }
    });
}

EFMod* CreateMod() {
    static TerraFormats mod;
    return &mod;
}