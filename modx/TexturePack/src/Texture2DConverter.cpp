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

#include <TexturePack/Texture2DConverter.hpp>
#include <TexturePack/Log.hpp>

BNM::UnityEngine::Object *
Texture2DConverter::ApplyTextureData(const ImageData &pngData, BNM::UnityEngine::Object *texture) {
    if (!texture) {
        LOGE("应用纹理数据失败: 纹理对象为空");
        return nullptr;
    }

    LOGD("开始处理纹理数据到Texture2D");

    set_filterMode.SetInstance(texture);
    set_wrapMode.SetInstance(texture);
    set_filterMode.Call(0);
    set_wrapMode.Call(1);

    ProcessPixels(pngData.width, pngData.height, pngData.pixels, texture);

    LOGD("所有像素处理完成, 应用纹理更改");

    Apply.SetInstance(texture);
    Apply.Call();

    LOGD("纹理数据应用完成");
    return texture;
}

BNM::UnityEngine::Object *Texture2DConverter::FastLoad(const std::string &path) {
    auto pngData = PNGDecoder::Decode(path);

    auto texture = (BNM::UnityEngine::Object*)texture2d_unity_Class.CreateNewObjectParameters(pngData.width, pngData.height);
    if (!texture) {
        LOGE("创建Texture2D对象失败");
        return nullptr;
    }

    set_filterMode.SetInstance(texture);
    set_wrapMode.SetInstance(texture);
    set_filterMode.Call(0);
    set_wrapMode.Call(1);

    LOGD("Texture2D对象创建成功");

    return ApplyTextureData(pngData, texture);
}

BNM::UnityEngine::Object *
Texture2DConverter::ConvertToXnaTexture(BNM::UnityEngine::Object *unityTexture) {
    if (!unityTexture) {
        LOGE("转换到XNA纹理失败: 输入纹理为空");
        return nullptr;
    }

    LOGD("开始转换到XNA纹理");

    auto xnaTexture = (BNM::UnityEngine::Object*)texture2d_xna_Class.CreateNewObjectParameters(unityTexture);
    if (!xnaTexture) {
        LOGE("创建XNA纹理对象失败");
        return nullptr;
    }

    SharedBatching[xnaTexture].Set(false);
    NonSharedHeadInsert[xnaTexture].Set(false);

    LOGD("XNA纹理转换完成");
    return xnaTexture;
}

void Texture2DConverter::ProcessPixels(unsigned int width, unsigned int height,
                                       const std::vector<unsigned char> &image,
                                       BNM::UnityEngine::Object *texture) {
    try {
        SetPixel.SetInstance(texture);

        for (unsigned y = 0; y < height; ++y) {
            unsigned unity_y = (height - 1) - y;

            for (unsigned x = 0; x < width; ++x) {
                unsigned index = (y * width + x) * 4;

                float r = static_cast<float>(image[index]) / 255.0f;
                float g = static_cast<float>(image[index + 1]) / 255.0f;
                float b = static_cast<float>(image[index + 2]) / 255.0f;
                float a = static_cast<float>(image[index + 3]) / 255.0f;

                SetPixel.Call(x, unity_y, BNM::Structures::Unity::Color(r, g, b, a));
            }
        }
    } catch (const std::exception& e) {
        LOGE("像素处理异常: %s", e.what());
        throw;
    }
}

