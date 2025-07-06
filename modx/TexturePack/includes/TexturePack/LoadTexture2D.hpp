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
#include "Texture2DConverter.hpp"

#include <future>

struct DecodeResult {
    std::string contentKey;
    ImageData imageData;
    bool success{};
    std::string error;
};

inline std::vector<std::future<DecodeResult>> decodeFutures;
inline std::vector<DecodeResult> decodeResults;
inline std::atomic<size_t> successCount{0};
inline std::atomic<size_t> failCount{0};


inline void (*old_cctor)(BNM::UnityEngine::Object*);
void cctor(BNM::UnityEngine::Object* i);

inline BNM::UnityEngine::Object* (*old_LoadTexture2D)(BNM::UnityEngine::Object*, BNM::Structures::Mono::String*);
BNM::UnityEngine::Object* LoadTexture2D(BNM::UnityEngine::Object* i, BNM::Structures::Mono::String* path);

void hookAssets();


