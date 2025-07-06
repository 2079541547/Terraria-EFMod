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

#include <TexturePack/LoadTexture2D.hpp>
#include <TexturePack/Log.hpp>
#include <TexturePack/TexturePack.hpp>
#include <TexturePack/PNGDecoder.hpp>
#include <TexturePack/Texture2DConverter.hpp>

#include <jni.h>

#include <BNM/Loading.hpp>

void cctor(BNM::UnityEngine::Object* i) {
    LOGD("开始执行自定义构造函数(cctor)");
    old_cctor(i);

    redirectMap_unity.reserve(decodeResults.size());

    for (auto &result : decodeResults) {
        if (!result.success) {
            LOGE("跳过创建纹理 %s: %s", result.contentKey.c_str(), result.error.c_str());
            failCount++;
            continue;
        }

        try {
            LOGD("创建Texture2D对象: %s", result.contentKey.c_str());

            auto *texture = (BNM::UnityEngine::Object *) Texture2DConverter::texture2d_unity_Class.CreateNewObjectParameters(
                    result.imageData.width, result.imageData.height);

            if (!texture) {
                LOGE("创建Texture2D失败: %s", result.contentKey.c_str());
                failCount++;
                continue;
            }

            {
                static int logCounter = 0;
                if (++logCounter % 10 == 0) {
                    LOGD("批量处理纹理中... (%d/%zu)", logCounter, decodeResults.size());
                }

                Texture2DConverter::ApplyTextureData(result.imageData, texture);

                std::vector<unsigned char>().swap(result.imageData.pixels);
            }

            redirectMap_unity.emplace(result.contentKey, texture);
            successCount++;

            LOGI("成功加载纹理: %s (尺寸: %ux%u)",
                 result.contentKey.c_str(), result.imageData.width, result.imageData.height);

        } catch (const std::exception &e) {
            failCount++;
            LOGE("处理纹理 %s 失败: %s", result.contentKey.c_str(), e.what());
        }
    }

    decodeResults.clear();
    decodeResults.shrink_to_fit();

    LOGI("纹理加载完成 (成功: %lu, 失败: %lu)", successCount.load(), failCount.load());
}

BNM::UnityEngine::Object* LoadTexture2D(BNM::UnityEngine::Object* i, BNM::Structures::Mono::String* path) {
    std::string originalPath = path->str();

    auto cachedIt = redirectMap.find(originalPath);
    if (cachedIt != redirectMap.end()) {
        LOGI("%s -> (使用缓存)", originalPath.c_str());
        return (BNM::UnityEngine::Object*)cachedIt->second;
    }

    auto cachedUIt = redirectMap_unity.find(originalPath);
    if (cachedUIt != redirectMap_unity.end() && cachedIt == redirectMap.end()) {
        auto newTexture = Texture2DConverter::ConvertToXnaTexture((BNM::UnityEngine::Object*)redirectMap_unity[cachedUIt->first]);
        redirectMap[originalPath] = newTexture;
        LOGI("%s -> %s (新建缓存)", originalPath.c_str(), originalPath.c_str());

        return newTexture;
    }

    auto r = old_LoadTexture2D(i, path);
    Texture2DConverter::SharedBatching[r].Set(false);
    Texture2DConverter::NonSharedHeadInsert[r].Set(false);

    return r;
}

void hookAssets() {
    auto texture_class = BNM::Class("UnityEngine", "Texture2D");
    Texture2DConverter::set_filterMode = texture_class.GetMethod("set_filterMode", 1);
    Texture2DConverter::set_wrapMode = texture_class.GetMethod("set_wrapMode", 1);

    Texture2DConverter::texture2d_unity_Class = BNM::Class("UnityEngine", "Texture2D");
    Texture2DConverter::texture2d_xna_Class = BNM::Class("Microsoft.Xna.Framework.Graphics", "Texture2D");
    Texture2DConverter::SetPixel = Texture2DConverter::texture2d_unity_Class.GetMethod("SetPixel", 3);
    Texture2DConverter::Apply = Texture2DConverter::texture2d_unity_Class.GetMethod("Apply", 0);

    Texture2DConverter::SharedBatching = Texture2DConverter::texture2d_xna_Class.GetField("SharedBatching");
    Texture2DConverter::NonSharedHeadInsert = Texture2DConverter::texture2d_xna_Class.GetField("NonSharedHeadInsert");

    auto ContentManager = BNM::Class("Microsoft.Xna.Framework.Content", "ContentManager");

    HOOK(ContentManager
                 .GetMethod("LoadTexture2D", 1),
         LoadTexture2D, old_LoadTexture2D);


    HOOK(ContentManager
                 .GetMethod(".cctor", 0),
         cctor, old_cctor);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);


    jclass stateClass = env->FindClass("eternal/future/State");
    if (stateClass != nullptr) {
        jfieldID efmodcFieldID = env->GetStaticFieldID(stateClass, "EFMod_c", "Ljava/lang/String;");
        auto efmodcValue = (jstring) env->GetStaticObjectField(stateClass, efmodcFieldID);
        std::string modDataPath = env->GetStringUTFChars(efmodcValue, nullptr);

        loadTexturePack(modDataPath);
    }

    BNM::Loading::TryLoadByJNI(env);
    BNM::Loading::AddOnLoadedEvent(hookAssets);

    return JNI_VERSION_1_6;
}