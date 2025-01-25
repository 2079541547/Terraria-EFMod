//
// Created by EternalFuture on 2025/1/25.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Loading.hpp>
#include <ItemID.hpp>
#include <Recipe.hpp>
#include <Item.hpp>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, [[maybe_unused]] void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    BNM::Loading::TryLoadByJNI(env);
    BNM::Loading::AddOnLoadedEvent(ItemID::initialize);
    BNM::Loading::AddOnLoadedEvent(Recipe::initialize);
    BNM::Loading::AddOnLoadedEvent(Item::initialize);


    return JNI_VERSION_1_6;
}