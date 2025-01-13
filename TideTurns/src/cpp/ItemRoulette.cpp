#include "ItemRoulette.hpp"
#include <random>
#include <set>


ItemRoulette::ItemRoulette(EFModAPI *e, const nlohmann::json &j) {
    if (!j["ItemRoulette"]["enabled"]) return;
    API = e;
    Deprecated = nullptr;
    number = j["ItemRoulette"]["number"];
    index = j["ItemRoulette"]["index"];
    API->registerModFuncDescriptor(
            {
                    "Assembly-CSharp.dll",
                    "Terraria.ID",
                    "ItemID.Sets",
                    ".cctor",
                    "hook>>void",
                    0,
                    {
                            (void*)New_ItemIDSets
                    }
            });
    API->registerModApiDescriptor({
        "Assembly-CSharp.dll",
        "Terraria.ID",
        "ItemID.Sets",
        "Deprecated",
        "Field"
    });
}


void redirectPointer(uintptr_t originalPtrAddress, uintptr_t newPtrAddress) {
    void** originalPtr = reinterpret_cast<void**>(originalPtrAddress);
    *originalPtr = reinterpret_cast<void*>(newPtrAddress);
}


void ItemRoulette::New_ItemIDSets(BNM::UnityEngine::Object *i) {
    if (Deprecated == nullptr) Deprecated = (BNM::Field<BNM::Structures::Mono::Array<bool>>*)API->getAPI({"Assembly-CSharp.dll", "Terraria.ID", "ItemID.Sets", "Deprecated", "Field"});
    auto *newDeprecated = new BNM::Structures::Mono::Array<bool>;
    bool *trueArray = new bool[index];
    for (size_t _ = 0; _ < index; ++_) {
        trueArray[_] = false;
    }

    if (number > index) number = index;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, index);
    std::set<int> selectedIndices;
    for (int _ = 0; _ < number; ++_) {
        int newitem;
        do {
            newitem = dis(gen);
        } while (selectedIndices.find(newitem) != selectedIndices.end());
        selectedIndices.insert(newitem);
        trueArray[newitem] = true;
        BNM_LOG_DEBUG("被禁用的物品(id):%d", newitem);

    }
    selectedIndices.clear();

    newDeprecated->CopyFrom(trueArray, index);
    delete[](trueArray);
    redirectPointer((uintptr_t)Deprecated->GetPointer(), (uintptr_t)trueArray);
}