//
// Created by EternalFuture on 2025/1/25.
//

#pragma once

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>
#include "EFMod.hpp"

namespace ItemID {
    inline BNM::Field <BNM::Structures::Mono::Array<bool>*>* Deprecated;
    inline BNM::Field <BNM::Structures::Mono::Array<bool>*>* ItemsThatShouldNotBeInInventory;
    inline BNM::Field <BNM::Structures::Mono::Array<int>*>* ShimmerTransformToItem;
    inline BNM::Field <BNM::Structures::Mono::List<int>*>* ItemsThatAreProcessedAfterNormalContentSample;

    void cctor(void* i);
    inline void* old_cctor = nullptr;

    void Template(BNM::UnityEngine::Object* i);
    inline HookTemplate T_cctor = {
            (void*)Template,
            {}
    };

    inline std::vector<std::string> Fields = {
            "Deprecated",
            "ItemsThatShouldNotBeInInventory",
            "ShimmerTransformToItem",
            "ItemsThatAreProcessedAfterNormalContentSample"
    };
}