/*******************************************************************************
 * 创建时间: 2025/5/4
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
#include <link.h>

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/Field.hpp>
#include <BNM/Method.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Loading.hpp>

#include "TerraFormats/LanguageManager.hpp"

#include "TerraFormats/Log.hpp"

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

void init() {
    LanguageManager::GameCulture::FromName = new BNM::Method<BNM::UnityEngine::Object*>(BNM::Class("Terraria.Localization", "GameCulture").GetMethod("FromName"));
    LanguageManager::LoadLanguageFromFileText = new BNM::Method<void>(BNM::Class("Terraria.Localization", "LanguageManager").GetMethod("LoadLanguageFromFileText"));
    LanguageManager::ProcessCopyCommandsInTexts = new BNM::Method<void>(BNM::Class("Terraria.Localization", "LanguageManager").GetMethod("ProcessCopyCommandsInTexts"));
    LanguageManager::PrepareAliases = new BNM::Method<void>(BNM::Class("Terraria.Initializers", "ChatInitializer").GetMethod("PrepareAliases"));
    HOOK(BNM::Class("Terraria.Localization", "LanguageManager").GetMethod("LoadLanguage"), LanguageManager::LoadLanguage, LanguageManager::old_LoadLanguage);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    jclass stateClass = env->FindClass("eternal/future/State");
    if (stateClass != nullptr) {
        jfieldID efmodcFieldID = env->GetStaticFieldID(stateClass, "EFMod_c", "Ljava/lang/String;");
        auto efmodcValue = (jstring) env->GetStaticObjectField(stateClass, efmodcFieldID);
        std::string modDataPath = env->GetStringUTFChars(efmodcValue, nullptr);

        std::string pathStr = modDataPath;
        size_t typoPos = pathStr.find("TEFModLoaLoader");
        if (typoPos != std::string::npos) {
            pathStr.replace(typoPos, 15, "TEFModLoader");
            LOGW("检测到路径拼写错误，已自动修正：%s -> %s",
                 modDataPath.c_str(), pathStr.c_str());
        }

        const auto fullPath = std::filesystem::path(modDataPath)
                              / getLibraryParentDirectory() / "private";

        LanguageManager::init(fullPath);
    }

    BNM::Loading::TryLoadByJNI(env);

    BNM::Loading::AddOnLoadedEvent(init);

    return JNI_VERSION_1_6;
}
