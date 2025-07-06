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

#include "TerraFormats/LanguageManager.hpp"
#include "TerraFormats/LanguagePacks.hpp"
#include "TerraFormats/Log.hpp"

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>

#include <fstream>
#include <sstream>

std::vector<std::string> LanguageManager::languages_container {
        "English",
        "German",
        "Italian",
        "Portuguese",
        "Russian",
        "French",
        "Spanish"
};

std::vector<std::string> LanguageManager::languages_code {
        "en-US",    // English (United States)
        "de-DE",    // German (Germany)
        "it-IT",    // Italian (Italy)
        "pt-PT",    // Portuguese (Portugal)
        "ru-RU",    // Russian (Russia)
        "fr-FR",    // French (France)
        "es-ES"     // Spanish (Spain)
};

std::string LanguageManager::CodeToName(const std::string &code)  {
    for (size_t i = 0; i < languages_code.size(); ++i) {
        if (languages_code[i] == code) {
            return languages_container[i]; // 直接返回对应位置的name
        }
    }
    throw std::invalid_argument("未知语言代码: " + code);
}

std::string LanguageManager::NameToCode(const std::string& name) {
    for (size_t i = 0; i < languages_container.size(); ++i) {
        if (languages_container[i] == name) {
            return languages_code[i]; // 直接返回对应位置的code
        }
    }
    throw std::invalid_argument("未知语言名称: " + name);
}

void LanguageManager::init(const std::filesystem::path& path) {

    std::string path_str = path.string();
    const std::string wrong_str = "TEFModLoaLoader";
    const std::string correct_str = "TEFModLoader";

    size_t pos = path_str.find(wrong_str);
    if (pos != std::string::npos) {
        path_str.replace(pos, wrong_str.length(), correct_str);
        LOGI("纠正了路径的拼写错误：%s -> %s",
             path.string().c_str(), path_str.c_str());
    }

    std::filesystem::path corrected_path(path_str);

    LOGI("开始初始化语言管理器，路径: %s", corrected_path.c_str());

    try {
        LOGI("正在加载配置文件: %s/config.json", corrected_path.c_str());
        auto config_path = corrected_path / "config.json";
        if (!std::filesystem::exists(config_path)) {
            LOGE("配置文件不存在: %s", config_path.string().c_str());
            return;
        }

        auto json = nlohmann::json::parse(std::ifstream(config_path));
        LOGI("成功加载配置文件，包含 %zu 个语言包配置", json.size());

        for (const auto& item : json) {
            try {
                bool enable = item.value("enable", false);
                std::string pack_path = item.value("path", "");
                int priority = item.value("priority", 0);

                LOGD("处理语言包配置: enable=%s, path=%s, priority=%d",
                     enable ? "true" : "false", pack_path.c_str(), priority);

                if (enable && !pack_path.empty()) {
                    auto full_path = corrected_path / pack_path;
                    packs_info.push_back({full_path, priority});
                    LOGI("添加语言包: %s (优先级: %d)", full_path.string().c_str(), priority);
                }
            } catch (const std::exception& e) {
                LOGE("处理语言包配置时出错: %s", e.what());
            }
        }

        LOGI("正在排序语言包 (共 %zu 个)", packs_info.size());
        std::sort(packs_info.begin(), packs_info.end(),
                  [](const auto& a, const auto& b) { return a.priority > b.priority; });

        for (size_t i = 0; i < packs_info.size(); ++i) {
            LOGD("语言包排序后 [%zu]: %s (优先级: %d)",
                 i, packs_info[i].path.c_str(), packs_info[i].priority);
        }

        size_t num_packs = std::min(languages_code.size(), packs_info.size());
        LOGI("准备初始化 %zu 个语言包", num_packs);

        for (size_t i = 0; i < num_packs; ++i) {
            const auto& code = languages_code[i];
            const auto& pack_path = packs_info[i].path;

            try {
                LOGI("正在初始化语言包 [%s]: %s", code.c_str(), pack_path.c_str());
                packs[code] = new LanguagePacks(pack_path);
                LOGI("成功初始化语言包: %s", code.c_str());
            } catch (const std::exception& e) {
                LOGE("初始化语言包 [%s] 失败: %s", code.c_str(), e.what());
            }
        }

        LOGI("语言管理器初始化完成，共加载 %zu/%zu 个语言包",
             packs.size(), languages_code.size());
    } catch (const nlohmann::json::parse_error& e) {
        LOGE("解析JSON失败: %s", e.what());
    } catch (const std::exception& e) {
        LOGE("初始化语言管理器时发生未知错误: %s", e.what());
    }
}



void LanguageManager::LoadLanguage(void *instance, void *culture, bool processCopyCommands) {
    if (packs.empty()) {
        LOGW("未加载语言包");
        old_LoadLanguage(instance, culture, processCopyCommands);
        return;
    }

    static std::unordered_map<std::string, std::string> language_list = {};
    if (language_list.empty()) {
        LOGI("初始化语言列表");
        for (auto pack: packs) {
            auto languageName = pack.second->get_info().name + " by " + pack.second->get_info().author;
            language_list[CodeToName(pack.first)] = languageName;
            LOGI("将: %s, 放进: %s", languageName.c_str(), CodeToName(pack.first).c_str());
        }
    }

    static auto FromName =  static_cast<BNM::Method<BNM::UnityEngine::Object*>*>(GameCulture::FromName);

    static std::unordered_map<void*, LanguagePacks*> packs_cache;
    for (const auto& pack: packs) {
        packs_cache[FromName->Call(BNM::CreateMonoString(pack.first))] = pack.second;
    }
    LOGI("语言查找表初始化成功");

    auto pack = packs_cache.find(culture);
    if (pack == packs_cache.end()) {
        pack = packs_cache.find(FromName->Call(BNM::CreateMonoString("en-US")));
    }

    static auto loadLanguageFromFileText = static_cast<BNM::Method<void>*>(LoadLanguageFromFileText);

    LOGI("加载语言包: %p -> %p", pack->second, pack->first);

    auto pack_info = pack->second->get_info();
    LOGI("名称: %s, 类型: %s, 补全语言: %s, 标准: %d", pack_info.name.c_str(),
         pack_info.type.c_str(),
         pack_info.autocomplete.c_str(),
         pack_info.standard);

    old_LoadLanguage(instance, FromName->Call(BNM::CreateMonoString(pack_info.autocomplete)), processCopyCommands);
    LOGI("已加载补全语言: %s", pack_info.autocomplete.c_str());


    loadLanguageFromFileText->SetInstance(static_cast<BNM::IL2CPP::Il2CppObject *>(instance));

    if (pack->second->isCommon()) {
        for (const auto& fileName: pack->second->common_name) {
            auto data = pack->second->get_file_data("localization/" + fileName + ".json" );
            if (!data.empty()) {
                if (fileName == "Base") {
                    loadLanguageFromFileText->Call(BNM::CreateMonoString(
                        LanguagePacks::get_setLanguagesList(language_list,
                                                            std::string(data.begin(),
                                                                        data.end()))));
                } else {
                    loadLanguageFromFileText->Call(
                        BNM::CreateMonoString(std::string(data.begin(), data.end())));
                }
            }
        }
    }

    static auto processCopyCommandsInTexts = static_cast<BNM::Method<void>*>(ProcessCopyCommandsInTexts);
    processCopyCommandsInTexts->SetInstance(static_cast<BNM::IL2CPP::Il2CppObject *>(instance));
    processCopyCommandsInTexts->Call();

    static auto prepareAliases = static_cast<BNM::Method<void>*>(PrepareAliases);
    prepareAliases->Call();

    LOGI("已设置为语言包：%s", pack->second->get_info().name.c_str());
}
