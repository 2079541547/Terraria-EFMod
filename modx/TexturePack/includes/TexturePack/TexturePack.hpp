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

#pragma once

#include <string>
#include <vector>
#include <unordered_map>


struct TextureData {
    std::vector<uint8_t> data;
    std::string entry_name;
};

class TexturePack {
private:
    std::vector<TextureData> textures;
    static bool ParseTLConfig(const std::vector<uint8_t>& jsonData, TextureData& texture);
    int _mode;
    std::string _path;

public:

    explicit TexturePack(const std::string& path, int mode): _path(std::move(path)), _mode(mode) {
        if (_mode == 0) LoadZipFormat(); else if(_mode == 1) LoadTLFormat();
    }

    void LoadZipFormat();

    void LoadTLFormat();

    [[nodiscard]] const std::vector<TextureData>& GetTextures() const;

    [[nodiscard]] const TextureData* FindTexture(const std::string& name) const;

    void Clear();
};

inline std::unordered_map<std::string, void*> redirectMap;
inline std::unordered_map<std::string, void*> redirectMap_unity;
inline std::vector<TexturePack> TexturePacks;

void loadTexturePack(const std::string& path);