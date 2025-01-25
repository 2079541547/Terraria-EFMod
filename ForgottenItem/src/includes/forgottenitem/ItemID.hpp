//
// Created by EternalFuture on 2025/1/25.
//

#include <BNM/UserSettings/GlobalSettings.hpp>
#include <BNM/UnityStructures.hpp>
#include <BNM/Field.hpp>

namespace ItemID {
    inline BNM::Class ItemID_Class;
    inline BNM::Class ItemID_Set;
    inline BNM::Field<int> Count;
    inline BNM::Field <BNM::Structures::Mono::Array<bool>> Deprecated;
    inline BNM::Field <BNM::Structures::Mono::Array<bool>> ItemsThatShouldNotBeInInventory;
    inline BNM::Field <BNM::Structures::Mono::Array<int>> ShimmerTransformToItem;


    void initialize();

    inline void (*old_cctor)(BNM::UnityEngine::Object*);
    void cctor(BNM::UnityEngine::Object* i);
}