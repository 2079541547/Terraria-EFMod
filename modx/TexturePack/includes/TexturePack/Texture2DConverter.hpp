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

#include "../../BNM-Android-master/include/BNM/UserSettings/GlobalSettings.hpp"

#include "../../BNM-Android-master/include/BNM/UnityStructures.hpp"
#include "../../BNM-Android-master/include/BNM/Class.hpp"
#include "../../BNM-Android-master/include/BNM/Method.hpp"
#include "../../BNM-Android-master/include/BNM/Field.hpp"

#include "PNGDecoder.hpp"

#include <GLES/gl.h>

class Texture2DConverter{
private:
    static void ProcessPixels(unsigned width, unsigned height,
                              const std::vector<unsigned char>& image,
                              BNM::UnityEngine::Object* texture);

public:
    inline static BNM::Class texture2d_unity_Class;
    inline static BNM::Class texture2d_xna_Class;

    inline static BNM::Method<void> SetPixel;
    inline static BNM::Method<void> Apply;

    inline static BNM::Method<void> set_filterMode;
    inline static BNM::Method<void> set_wrapMode;

    inline static BNM::Field<bool> SharedBatching;
    inline static BNM::Field<bool> NonSharedHeadInsert;

    static BNM::UnityEngine::Object* ApplyTextureData(const ImageData& pngData, BNM::UnityEngine::Object* texture);

    static BNM::UnityEngine::Object* FastLoad(const std::string& path);

    static BNM::UnityEngine::Object* ConvertToXnaTexture(BNM::UnityEngine::Object* unityTexture);
};