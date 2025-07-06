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

#include <TexturePack/PNGDecoder.hpp>
#include <TexturePack/Log.hpp>

#include <lodepng.h>

inline void LogDecodeResult(const std::string& context, const ImageData& result, bool success, unsigned error = 0) {
    if (success) {
        LOGD("PNG解码成功 [%s] 尺寸: %ux%u (%.2fMB)",
             context.c_str(), result.width, result.height,
             result.pixels.size() / (1024.0f * 1024.0f));
    } else {
        LOGE("PNG解码失败 [%s] 错误: %d - %s",
             context.c_str(), error, lodepng_error_text(error));
    }
}

ImageData PNGDecoder::Decode(const uint8_t* pngData, size_t dataSize) {
    ImageData result;
    const auto error = lodepng::decode(result.pixels, result.width, result.height,
                                       pngData, dataSize, LCT_RGBA);

    LogDecodeResult("内存数据", result, !error, error);
    if (error) throw std::runtime_error("PNG解码失败: " + std::string(lodepng_error_text(error)));

    return result;
}

ImageData PNGDecoder::Decode(const std::string& path) {
    LOGD("开始解码PNG文件: %s", path.c_str());

    ImageData result;
    const auto error = lodepng::decode(result.pixels, result.width, result.height, path, LCT_RGBA);

    LogDecodeResult(path, result, !error, error);
    if (error) throw std::runtime_error("PNG解码失败: " + path);

    return result;
}

ImageData PNGDecoder::Decode(const std::vector<uint8_t>& pngData) {
    return Decode(pngData.data(), pngData.size());
}