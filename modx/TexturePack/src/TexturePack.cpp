/*******************************************************************************
 * 创建时间: 2025/4/30
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

#include <jni.h>

#include <TexturePack/TexturePack.hpp>
#include <TexturePack/Log.hpp>

#include <miniz.h>
#include <json.hpp>

#include <future>

#include <fstream>

using json = nlohmann::json;

void TexturePack::LoadZipFormat() {
    LOGI("开始加载ZIP格式纹理包: %s", _path.c_str());
    mz_zip_archive zip_archive = {};

    if (!mz_zip_reader_init_file(&zip_archive, _path.c_str(), 0)) {
        LOGE("无法初始化ZIP读取器，文件可能损坏或不存在: %s", _path.c_str());
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

        if (filename.find("Content/") == std::string::npos) {
            LOGD("跳过非Content目录文件: %s", filename.c_str());
            continue;
        }

        size_t uncomp_size;
        void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncomp_size, 0);
        if (!p) {
            LOGE("提取文件失败: %s", filename.c_str());
            continue;
        }

        LOGD("成功提取文件: %s (原始大小: %lu 字节, 解压后大小: %zu 字节)",
             filename.c_str(), file_stat.m_uncomp_size, uncomp_size);

        textures.push_back({
            std::vector<uint8_t>(static_cast<uint8_t*>(p), static_cast<uint8_t*>(p) + uncomp_size),
                           filename
        });

        total_loaded++;
        total_size += uncomp_size;

        mz_free(p);
        LOGD("已释放文件内存: %s", filename.c_str());
    }

    mz_zip_reader_end(&zip_archive);

    LOGI("完成ZIP包加载: %s (共加载 %zu 个纹理文件, 总大小: %.2f MB)",
         _path.c_str(), total_loaded, total_size / (1024.0f * 1024.0f));

    LOGD("当前纹理列表中共有 %zu 个纹理", textures.size());
}

void TexturePack::LoadTLFormat() {
    LOGI("开始加载TL格式文件: %s", _path.c_str());
    mz_zip_archive zip_archive = {};

    if (!mz_zip_reader_init_file(&zip_archive, _path.c_str(), 0)) {
        LOGE("无法初始化ZIP读取器: %s", _path.c_str());
        return;
    }

    int file_count = (int)mz_zip_reader_get_num_files(&zip_archive);
    LOGD("ZIP文件中包含 %d 个文件", file_count);

    std::unordered_map<std::string, std::string> json_entries;
    for (int i = 0; i < file_count; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            continue;
        }

        std::string filename(file_stat.m_filename);
        if (filename.find("Modified/") == std::string::npos) {
            continue;
        }

        if (filename.size() >= 5 &&
            filename.compare(filename.size() - 5, 5, ".json") == 0) {
            size_t last_slash = filename.find_last_of('/');
        std::string id = filename.substr(last_slash + 1, filename.size() - last_slash - 6);
        json_entries[id] = filename;
        LOGD("发现JSON配置文件: %s (ID: %s)", filename.c_str(), id.c_str());
            }
    }

    for (int i = 0; i < file_count; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            continue;
        }

        std::string filename(file_stat.m_filename);
        if (filename.find("Modified/") == std::string::npos) {
            continue;
        }

        if (filename.size() >= 5 &&
            filename.compare(filename.size() - 5, 5, ".json") == 0) {
            continue;
            }

            size_t last_slash = filename.find_last_of('/');
        size_t last_dot = filename.find_last_of('.');
        if (last_slash == std::string::npos || last_dot == std::string::npos) {
            LOGW("无法解析文件名: %s", filename.c_str());
            continue;
        }

        std::string id = filename.substr(last_slash + 1, last_dot - last_slash - 1);
        LOGD("处理纹理文件: %s (ID: %s)", filename.c_str(), id.c_str());

        auto json_it = json_entries.find(id);
        if (json_it == json_entries.end()) {
            LOGW("找不到纹理 %s 的JSON配置文件", filename.c_str());
            continue;
        }

        size_t texture_size;
        void* texture_data = mz_zip_reader_extract_file_to_heap(&zip_archive, filename.c_str(), &texture_size, 0);
        if (!texture_data) {
            LOGE("无法提取纹理文件: %s", filename.c_str());
            continue;
        }

        size_t json_size;
        void* json_data = mz_zip_reader_extract_file_to_heap(&zip_archive, json_it->second.c_str(), &json_size, 0);
        if (!json_data) {
            LOGE("无法提取JSON文件: %s", json_it->second.c_str());
            mz_free(texture_data);
            continue;
        }

        TextureData texture;
        texture.data.resize(texture_size);
        memcpy(texture.data.data(), texture_data, texture_size);

        if (ParseTLConfig(
            std::vector<uint8_t>(static_cast<uint8_t*>(json_data), static_cast<uint8_t*>(json_data) + json_size),
                          texture)) {
            textures.push_back(std::move(texture));
        LOGI("成功加载纹理: %s (大小: %zu 字节)", filename.c_str(), texture_size);
                          }

                          mz_free(texture_data);
                          mz_free(json_data);
    }

    mz_zip_reader_end(&zip_archive);
    LOGI("完成TL格式文件加载: %s (共加载 %zu 个纹理)", _path.c_str(), textures.size());
}

const TextureData *TexturePack::FindTexture(const std::string &name) const {
    for (const auto& tex : textures) {
        if (tex.entry_name ==name) {
            return &tex;
        }
    }
    return nullptr;
}

const std::vector<TextureData> &TexturePack::GetTextures() const {
    return textures;
}

void TexturePack::Clear() {
    textures.clear();
}

bool TexturePack::ParseTLConfig(const std::vector<uint8_t> &jsonData, TextureData &texture) {
    try {
        json config = json::parse(jsonData.begin(), jsonData.end());
        texture.entry_name = config.value("entry_name", "");
        return true;
    } catch (...) {
        return false;
    }
}

#include <link.h>
#include <TexturePack/LoadTexture2D.hpp>


std::string getLibraryParentDirectory() {
    Dl_info dl_info;
    std::filesystem::path r;

    if (dladdr((void*)getLibraryParentDirectory, &dl_info)) {
        if (dl_info.dli_fname) {
            std::filesystem::path d(dl_info.dli_fname);
            r = d.parent_path().filename();
        }
    }
    return r;
}

std::future<TexturePack> asyncLoadTexturePack(const std::filesystem::path& fullPath, const json& item) {
    return std::async(std::launch::async, [=]() {
        if (!item["enable"].get<bool>()) {
            throw std::runtime_error("Texture pack disabled");
        }
        TexturePack pack(fullPath / item["file"].get<std::string>(),
                         item["type"].get<int>());
        LOGI("异步加载材质：%s", item["file"].get<std::string>().c_str());
        return pack;
    });
}

void loadTexturePack(const std::string& path) {
    std::string pathStr = path;

    size_t typoPos = pathStr.find("TEFModLoaLoader");
    if (typoPos != std::string::npos) {
        pathStr.replace(typoPos, 15, "TEFModLoader");
        LOGW("检测到路径拼写错误，已自动修正：%s -> %s",
             path.c_str(), pathStr.c_str());
    }

    const auto fullPath = std::filesystem::path(pathStr) / getLibraryParentDirectory() / "private";

    const auto configPath = fullPath / "config.json";
    if (!std::filesystem::exists(configPath) || std::filesystem::file_size(configPath) == 0) {
        LOGE("配置文件不存在或为空: %s", configPath.string().c_str());
        return;
    }
    json data = json::parse(std::ifstream(configPath));

    std::vector<std::future<TexturePack>> textureFutures;
    std::transform(data.begin(), data.end(), std::back_inserter(textureFutures),
                   [&fullPath](const auto &item) { return asyncLoadTexturePack(fullPath, item); });

    std::for_each(textureFutures.begin(), textureFutures.end(), [](auto &future) {
        try {
            TexturePacks.emplace_back(future.get());
        } catch (const std::exception &e) {
            LOGW("加载材质失败: %s", e.what());
        }
    });

    LOGD("开始收集纹理解码任务...");

    size_t totalTextures = std::accumulate(TexturePacks.begin(), TexturePacks.end(), 0,
                                           [](size_t sum, const auto &pack) {
                                               return sum + pack.GetTextures().size();
                                           });

    decodeFutures.reserve(totalTextures);
    redirectMap_unity.reserve(totalTextures);

    for (const auto &pack: TexturePacks) {
        for (const auto &tex: pack.GetTextures()) {
            std::string cleanKey = tex.entry_name.substr(0, tex.entry_name.find_last_of('.'));

            LOGD("准备解码纹理: %s (原始名称: %s, 大小: %zu 字节)",
                 cleanKey.c_str(), tex.entry_name.c_str(), tex.data.size());

            decodeFutures.emplace_back(std::async(std::launch::async, [data = tex.data,
                                                  name = tex.entry_name,
                                                  cleanKey = std::move(cleanKey)]() {
                                                      LOGD("开始异步解码纹理: %s", name.c_str());
                                                      try {
                                                          auto result = PNGDecoder::Decode(data);
                                                          LOGD("成功解码纹理: %s (尺寸: %ux%u)", name.c_str(), result.width,
                                                               result.height);
                                                          return DecodeResult{cleanKey, std::move(result), true, ""};
                                                      } catch (const std::exception &e) {
                                                          LOGE("解码纹理 %s 失败: %s", name.c_str(), e.what());
                                                          return DecodeResult{cleanKey, {}, false, e.what()};
                                                      }
                                                  }));
        }
    }

    LOGI("总共收集到 %zu 个纹理解码任务", decodeFutures.size());
    if (decodeFutures.empty()) {
        LOGW("没有发现需要加载的纹理");
        return;
    }

    LOGI("等待所有解码任务完成...");

    decodeResults.resize(decodeFutures.size());
    std::transform(std::make_move_iterator(decodeFutures.begin()),
                   std::make_move_iterator(decodeFutures.end()),
                   decodeResults.begin(),
                   [](auto &&future) {
                       try {
                           auto result = future.get();
                           result.success ? ++successCount : ++failCount;
                           return result;
                       } catch (const std::exception &e) {
                           LOGE("获取解码结果时发生异常: %s", e.what());
                           ++failCount;
                           return DecodeResult{"", {}, false, e.what()};
                       }
                   });

    LOGI("所有解码任务已完成 (成功: %zu, 失败: %zu)", successCount.load(), failCount.load());
}
