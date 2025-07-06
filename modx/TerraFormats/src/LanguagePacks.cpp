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

#include "TerraFormats/LanguagePacks.hpp"
#include "TerraFormats/Log.hpp"

#include "miniz.h"

LanguagePacks::LanguagePacks(const std::string &path) {

    LOGI("开始加载ZIP格式语言包: %s", path.c_str());
    mz_zip_archive zip_archive = {};

    if (!mz_zip_reader_init_file(&zip_archive, path.c_str(), 0)) {
        LOGE("无法初始化ZIP读取器，文件可能损坏或不存在: %s", path.c_str());
        return;
    }

    int file_count = (int)mz_zip_reader_get_num_files(&zip_archive);
    LOGD("ZIP包中包含 %d 个文件", file_count);

    size_t total_loaded = 0;
    size_t total_size = 0;

    for (int i = 0; i < file_count; i++) {
        mz_zip_archive_file_stat file_stat;

        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            LOGW("无法获取ZIP中第 %d 个文件的信息", i);
            continue;
        }

        if (mz_zip_reader_is_file_a_directory(&zip_archive, i)) {
            LOGD("跳过目录: %s", file_stat.m_filename);
            continue;
        }

        std::string filename(file_stat.m_filename);
        LOGD("处理文件 [%d/%d]: %s (压缩大小: %lu 字节)",
             i+1, file_count, filename.c_str(), file_stat.m_comp_size);

        size_t uncomp_size;
        void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncomp_size, 0);
        if (!p) {
            LOGE("提取文件失败: %s", filename.c_str());
            continue;
        }

        LOGD("成功提取文件: %s (原始大小: %lu 字节, 解压后大小: %zu 字节)",
             filename.c_str(), file_stat.m_uncomp_size, uncomp_size);

        _data[filename] = std::vector<uint8_t>(static_cast<uint8_t*>(p), static_cast<uint8_t*>(p) + uncomp_size);

        total_loaded++;
        total_size += uncomp_size;

        mz_free(p);
        LOGD("已释放文件内存: %s", filename.c_str());
    }

    mz_zip_reader_end(&zip_archive);

    LOGI("完成ZIP包加载: %s (共加载 %zu 个文件, 总大小: %.2f MB)",
         path.c_str(), total_loaded, total_size / (1024.0f * 1024.0f));

    LOGD("当前数据列表中共有 %zu 个文件", _data.size());

    auto config = nlohmann::json::parse(std::string(_data["config.json"].begin(), _data["config.json"].end()));

    _info.name = config["name"].get<std::string>();
    _info.autocomplete = config["autocomplete"].get<std::string>();
    _info.type = config["type"].get<std::string>();
    _info.standard = config["standard"].get<int>();
    _info.author = config["author"].get<std::string>();
}

bool LanguagePacks::isCommon() {
    return _info.type == "Common";
}

std::vector<uint8_t> LanguagePacks::get_file_data(const std::string& name) {    return _data[name]; }
LanguagePacks::LanguageInfo LanguagePacks::get_info() {   return _info;   }

std::string
LanguagePacks::get_setLanguagesList(const std::unordered_map<std::string, std::string> &list,
                                    const std::string &data) {
    auto j = nlohmann::json::parse(data);
    nlohmann::json& languageJson = j["Language"];

    for (auto& item : languageJson.items()) {
        const std::string& key = item.key();
        if (list.find(key) == list.end()) {
            languageJson[key] = "";
        }
    }

    for (const auto& [key, value] : list) {
        languageJson[key] = value;
    }

    return j.dump();
}