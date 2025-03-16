//
// Created by EternalFuture on 2025/1/25.
//

#include <Item.hpp>
#include <log.hpp>

void Item::SetDefaults3(void *instance, int t) {
    auto i = (BNM::UnityEngine::Object*)instance;

    if (t == 2881) {
        useStyle->SetInstance(i);
        useStyle->Set(5);

        useAnimation->SetInstance(i);
        useAnimation->Set(20);

        useTime->SetInstance(i);
        useTime->Set(80);

        shootSpeed->SetInstance(i);
        shootSpeed->Set(500.0f);

        width->SetInstance(i);
        width->Set(20);

        height->SetInstance(i);
        height->Set(12);

        damage->SetInstance(i);
        damage->Set(550);

        shoot->SetInstance(i);
        shoot->Set(79);

        mana->SetInstance(i);
        mana->Set(100);

        rare->SetInstance(i);
        rare->Set(8);

        value->SetInstance(i);

        value->Set(buyPrice->Call(0, 10, 0, 0));

        noMelee->SetInstance(i);
        noMelee->Set(true);

        channel->SetInstance(i);
        channel->Set(true);
    }
}

void Item::SetDefaults4(void *instance, int t) {
    auto i = static_cast<BNM::IL2CPP::Il2CppObject *>(instance);
    switch (t) {
        case 3853:
            useStyle->SetInstance(i);
            useStyle->Set(5);

            useAnimation->SetInstance(i);
            useAnimation->Set(25);

            useAmmo->SetInstance(i);
            useAmmo->Set(40);

            width->SetInstance(i);
            width->Set(20);

            height->SetInstance(i);
            height->Set(34);

            shoot->SetInstance(i);
            shoot->Set(1);

            damage->SetInstance(i);
            damage->Set(42);

            shootSpeed->SetInstance(i);
            shootSpeed->Set(50.0f);

            noMelee->SetInstance(i);
            noMelee->Set(true);

            value->SetInstance(i);
            value->Set(180000);

            ranged->SetInstance(i);
            ranged->Set(true);

            useTime->SetInstance(i);
            useTime->Set(12);

            channel->SetInstance(i);
            channel->Set(true);

            rare->SetInstance(i);
            rare->Set(6);

            autoReuse->SetInstance(i);
            autoReuse->Set(true);
            break;

        case 3850:
            useStyle->SetInstance(i);
            useStyle->Set(1);

            shootSpeed->SetInstance(i);
            shootSpeed->Set(55.5f);

            shoot->SetInstance(i);
            shoot->Set(680);

            damage->SetInstance(i);
            damage->Set(71);

            width->SetInstance(i);
            width->Set(0);

            height->SetInstance(i);
            height->Set(30);

            useAnimation->SetInstance(i);
            useAnimation->Set(17);

            useTime->SetInstance(i);
            useTime->Set(17);

            noUseGraphic->SetInstance(i);
            noUseGraphic->Set(true);

            noMelee->SetInstance(i);
            noMelee->Set(true);

            value->SetInstance(i);
            value->Set(buyPrice->Call(0, 5, 0, 0));

            ranged->SetInstance(i);
            ranged->Set(true);
            break;
    }
}

void Item::SetDefaults5(void *instance, int t) {
    auto i = (BNM::UnityEngine::Object*)instance;
    switch (t) {
        case 4058:
            damage->SetInstance(i);
            damage->Set(30);

            SetWeaponValues->SetInstance(i);
            SetWeaponValues->Call(30, 5.0f, 0);
            break;

        case 4722:
            noMelee->SetInstance(i);
            noMelee->Set(false);

            damage->SetInstance(i);
            damage->Set(130);
            break;
    }
}

void Item::SetFoodDefaults(void *instance, int t) {
    auto i = (BNM::UnityEngine::Object*)instance;
    if (t == 4010) {
        DefaultToFood->SetInstance(i);
        DefaultToFood->Call(22, 22, 207, 14400, false, 17);

        rare->SetInstance(i);
        rare->Set(3);

        value->SetInstance(i);
        int buyPriceResult = buyPrice->Call(0, 0, 70, 5);
        value->Set(buyPriceResult);
    }
}

void Item::Register(EFModAPI* mod) {
    for (auto hook: Hooks) {
        mod->registerModFuncDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            hook.first,
            hook.second.first.first,
            hook.second.second.first,
            hook.second.first.second,
            { hook.second.second.second }
        });
    }

    for (auto field: Fields) {
        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            field,
            "Field"
        });
    }

    for (auto method: Methods) {
        mod->registerModApiDescriptor({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            method.first,
            "Method",
            method.second
        });
    }
}

void Item::Initialize(EFModAPI *mod) {

    for (auto hook: Hooks) {
        auto r = mod->getAPI({
            "Assembly-CSharp.dll",
            "Terraria",
            "Item",
            hook.first,
            "old_fun",
            1
        });

        LOGI("获取原始函数: %s, 地址: %p", hook.first.c_str(), r);
        if (hook.first == "SetDefaults3") old_SetDefaults3 = r;
        else if(hook.first == "SetDefaults4") old_SetDefaults4 = r;
        else if(hook.first == "SetDefaults5") old_SetDefaults5 = r;
        else if(hook.first == "SetFoodDefaults") old_SetFoodDefaults = r;
        else LOGE("未知函数: %s", hook.first.c_str());
    }

    for (auto field: Fields) {
        auto ptr = mod->getAPI({
                                       "Assembly-CSharp.dll",
                                       "Terraria",
                                       "Item",
                                       field,
                                       "Field"
                               });

        LOGD("字段 %s 获取到的地址: %p", field.c_str(), ptr);
        if (field == "useStyle") useStyle = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "useAnimation") useAnimation = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "useTime") useTime = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "width") width = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "height") height = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "shoot") shoot = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "useAmmo") useAmmo = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "damage") damage = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "shootSpeed") shootSpeed = static_cast<BNM::Field<float>*>(ptr);
        else if (field == "noMelee") noMelee = static_cast<BNM::Field<bool>*>(ptr);
        else if (field == "value") value = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "ranged") ranged = static_cast<BNM::Field<bool>*>(ptr);
        else if (field == "channel") channel = static_cast<BNM::Field<bool>*>(ptr);
        else if (field == "rare") rare = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "autoReuse") autoReuse = static_cast<BNM::Field<bool>*>(ptr);
        else if (field == "mana") mana = static_cast<BNM::Field<int>*>(ptr);
        else if (field == "magic") magic = static_cast<BNM::Field<bool>*>(ptr);
        else if (field == "noUseGraphic") noUseGraphic = static_cast<BNM::Field<bool>*>(ptr);
        else LOGE("未知字段: %s", field.c_str());
    }

    for (auto method: Methods) {
        auto ptr = mod->getAPI({
                                       "Assembly-CSharp.dll",
                                       "Terraria",
                                       "Item",
                                       method.first,
                                       "Method"
                               });

        LOGD("方法 %s 获取到的地址: %p", method.first.c_str(), ptr);
        if (method.first == "DefaultToBow") DefaultToBow = static_cast<BNM::Method<void>*>(ptr);
        else if (method.first == "SetWeaponValues") SetWeaponValues = static_cast<BNM::Method<void>*>(ptr);
        else if (method.first == "DefaultToFood") DefaultToFood = static_cast<BNM::Method<void>*>(ptr);
        else if (method.first == "buyPrice") buyPrice = static_cast<BNM::Method<int>*>(ptr);
        else LOGE("未知方法: %s", method.first.c_str());
    }

    LOGD("Item 部分初始化完成");
}

void Item::SetDefaults3_Template(BNM::UnityEngine::Object* i, int t) {
    ((void(*)(BNM::UnityEngine::Object*, int))old_SetDefaults3)(i, t);

    for (auto fun: T_SetDefaults3.FunArray) {
        if(fun) ((void(*)(void*, int))fun)(i, t);
    }

}

void Item::SetDefaults4_Template(BNM::UnityEngine::Object* i, int t) {
    ((void(*)(BNM::UnityEngine::Object*, int))old_SetDefaults4)(i, t);
    for (auto fun: T_SetDefaults4.FunArray) {
        if(fun) ((void(*)(void*, int))fun)(i, t);
    }
}

void Item::SetDefaults5_Template(BNM::UnityEngine::Object* i, int t) {
    ((void(*)(BNM::UnityEngine::Object*, int))old_SetDefaults5)(i, t);

    for (auto fun: T_SetDefaults5.FunArray) {
        if(fun) ((void(*)(void*, int))fun)(i, t);
    }

 }

void Item::SetFoodDefaults_Template(BNM::UnityEngine::Object* i, int t) {
    ((void(*)(BNM::UnityEngine::Object*, int))old_SetFoodDefaults)(i, t);

    for (auto fun: T_SetFoodDefaults.FunArray) {
        if(fun) ((void(*)(void*, int))fun)(i, t);
    }
}